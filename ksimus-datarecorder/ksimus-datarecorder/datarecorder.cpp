/***************************************************************************
                          datarecorder.cpp  -  description
                             -------------------
    begin                : Thu May 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// C-Includes

// QT-Includes
#include <qvaluelist.h>
#include <qpopupmenu.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdata.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorbase.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimtimebase.h"
#include "ksimus/ksimtimeserver.h"
#include "ksimus/packageinfo.h"

#include "config.h"

#include "datarecorder.h"
#include "datarecorderview.h"
#include "datarecorderdataview.h"
#include "datarecorderwidget.h"
#include "datarecorderchannelbase.h"
#include "datarecorderchannelboolean.h"
#include "datarecorderchannelfloat.h"
#include "widgetcontrolbutton.h"
#include "zoomwidget.h"
#include "textrec.h"

// Forward declaration


//#################################################################
//#################################################################



namespace KSimLibDataRecorder
{

static KInstance * packageInstance = (KInstance *)0;
static PackageInfo * packageInfo = (PackageInfo *)0;
static bool isPackageInitialized = false;


KInstance * getPackageInstance()
{
	return packageInstance;
}

const PackageInfo * getPackageInfo()
{
	return packageInfo;
}


/************************************************************************************
 ************************************************************************************
 **
 **  Insert pointers to the ComponentInfo for each component you want to distribute.
 **
 ************************************************************************************
 ************************************************************************************/
static const ComponentInfoList & getDistComponents()
{
	static ComponentInfoList * pDistComponents = 0;
	
	if (pDistComponents == 0)
	{
		// Initialize
		pDistComponents = new ComponentInfoList;
		CHECK_PTR(pDistComponents);
		
		// Add your component info here
		pDistComponents->append(DataRecorder::getStaticInfo());
		pDistComponents->append(TextRec::getStaticInfo());
	}
	
	return *pDistComponents;
}

/******************************************************************************************
 ******************************************************************************************
 **
 **  No changes required inside the next function
 **
 ******************************************************************************************
 ******************************************************************************************/

static void initPackage(KLocale * ksimusLocale)
{

//	KSIMDEBUG("Init Package " KSIMUS_PACKAGE_NAME);
	ASSERT(packageInstance == (KInstance *)0);
	ASSERT(packageInfo == (const PackageInfo *)0);
	ASSERT(ksimusLocale != (KLocale *)0);

	packageInstance = new KInstance(KSIMUS_PACKAGE_LOWER_NAME);
	CHECK_PTR(packageInstance);
	// add translation
	ksimusLocale->insertCatalogue(packageInstance->instanceName());

	packageInfo = new PackageInfo( KSIMUS_PACKAGE_NAME,
	                               packageInstance,
	                               VERSION);      // version from config.h
	CHECK_PTR(packageInfo);

	// Add info lists
	packageInfo->insert(getDistComponents());
}


};  //namespace KSimLibDataRecorder



extern "C"
{
	const PackageInfo * init_ksimus_package(KLocale * ksimusLocale)
	{
		if (!KSimLibDataRecorder::isPackageInitialized)
		{
			KSimLibDataRecorder::initPackage(ksimusLocale);
			KSimLibDataRecorder::isPackageInitialized = true;
		}

		return KSimLibDataRecorder::packageInfo;
	}
}







//#################################################################
//#################################################################

namespace KSimLibDataRecorder
{

Component * DataRecorder::create(CompContainer * container, const ComponentInfo * ci)
{
	return new DataRecorder(container, ci);
}


const ComponentInfo * DataRecorder::getStaticInfo()
{
	static const ComponentInfo Info(i18n("DataRecorder-Component", "Data Recorder"),
	                                QString::fromLatin1("Recorder/Data Recorder"),
	                                i18n("DataRecorder-Component", "Recorder/Data Recorder"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("kapp-features"),
	                                QString::fromLatin1("Data Recorder"));
	return &Info;
}


//#################################################################
//#################################################################


static const char * const sNumberChannels = "Number of channels";
static const char * const sChannelX       = "Channel %1/";
static const char * const sChannelType    = "Channel Type";
static const char * const sSerialList     = "Serial List";
//static const char * const sSerialNumber   = "Last Serial Number"; // !!!from component.h !!!

static const QColor defaultColors[] = {Qt::black, Qt::red, Qt::green, Qt::blue,
                                       Qt::cyan, Qt::magenta, Qt::yellow, Qt::gray,
                                       Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkCyan,
                                       Qt::darkMagenta, Qt::darkYellow};


#define NO_OF_COLORS (sizeof(defaultColors)/sizeof(QColor))
								
DataRecorder::DataRecorder(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_widget(0),
		m_dataCount(0),
		m_serialNumberGenerator(0)
{
	readSampleTime();
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new DataRecorderView(this, SHEET_VIEW);
	}
	m_channelList = new DataRecorderChannelList;
	m_channelList->setAutoDelete(true);
	
	m_channelInputList = new ConnectorInputList;


	DataRecorderChannelBoolean * channel;
	
	channel = new DataRecorderChannelBoolean(this);
	newChannel(channel);
	
	m_zoomVar = new ZoomWidgetVar(m_sampleTime * 50); // Todo Zoom have to be located in DataRecorderDataView

}

DataRecorder::~DataRecorder()
{
	delete m_widget;
	delete m_channelInputList;
	delete m_channelList;
	delete m_zoomVar;
}

/** Creates a new channel according to the given type. */
DataRecorderChannelBase * DataRecorder::createChannel(eChannelType type)
{
	DataRecorderChannelBase * newChannel = 0L;
	
	switch (type)
	{
 		case CT_Boolean:
 			newChannel = new DataRecorderChannelBoolean(this);
 			CHECK_PTR(newChannel);
 			break;
  			
 		case CT_Float:
 			newChannel = new DataRecorderChannelFloat(this);
 			CHECK_PTR(newChannel);
 			break;
  			
 		case CT_Unknown:
 			KSIMDEBUG("Unknown channel type");
 			break;
 	}
	return newChannel;
}

	
void DataRecorder::newChannel(DataRecorderChannelBase * channel)
{
	unsigned int c = 0;
	bool found = false;
	QString newName;
	
	channel->setSerialNumber(nextSerialNumber());
	
	// Create a unique connector name
	ConnectorInputBase * inConn = channel->getConnector();
	QString baseName(inConn->getInitName());
	baseName += " %1";
	baseName = baseName.simplifyWhiteSpace();
	
	do
	{
		c++;
		newName = baseName.arg(c);
		found = false;
		FOR_EACH_CONNECTORINPUT(it, *m_channelInputList)
		{
			if(newName == it.current()->getName())
			{
				found = true;
				break;
			}
		}
	}
	while (found);
	
	inConn->setName(newName);
	
	// Set default color
	channel->setLineColor(defaultColors[m_channelInputList->count() % NO_OF_COLORS]);
	
	appendChannel(channel);
}	

void DataRecorder::appendChannel(DataRecorderChannelBase * channel)
{
	
	ConnectorInputBase * inConn = channel->getConnector();
	
	m_channelList->inSort(channel);
	m_channelInputList->append(inConn);

	
	arrangeChannelInputs();	

	// Property widget
	if(getDataRecoderWidget())
	{
		getDataRecoderWidget()->createChannelButton(channel);
	}
}

void DataRecorder::removeChannel(DataRecorderChannelBase * channel)
{
	bool res;

//	KSIMDEBUG_VAR("",getConnList()->count());
//	KSIMDEBUG_VAR("",m_channelInputList->count());
//	KSIMDEBUG_VAR("",m_channelList->count());
	
	res = getConnList()->removeRef(channel->getConnector());
	if (!res)
		KSIMDEBUG("getConnList() remove failed");
	
	res = m_channelInputList->removeRef(channel->getConnector());
	if (!res)
		KSIMDEBUG("m_channelInputList remove failed");
	
	res = m_channelList->removeRef(channel);
	if (!res)
		KSIMDEBUG("m_channelList remove failed");
		
//	KSIMDEBUG_VAR("",getConnList()->count());
//	KSIMDEBUG_VAR("",m_channelInputList->count());
//	KSIMDEBUG_VAR("",m_channelList->count());
	
	arrangeChannelInputs();	
}


DataRecorderChannelList * DataRecorder::getChannelList() const
{
	return m_channelList;
}

void DataRecorder::arrangeChannelInputs()
{
	int c = 0;
	
	FOR_EACH_CHANNEL(it, *m_channelList)
	{
		it.current()->getConnector()->setGridPos(QPoint(0, 2*c+1));
		c++;
	}

	
	if (getSheetView())
	{
		int newHeight;
	
	  if (m_channelList->count() < 3)
	  {
			newHeight = 5 * gridY;
		}
		else
		{
			newHeight = (m_channelList->count()*2+1) * gridY;
		}
	
		QRect place = getSheetView()->getPlace();
		place.setHeight(newHeight);
		getSheetView()->setPlace(place, true);
		
	}
	
	refresh();
}

/** Save DataRecorder properties */
void DataRecorder::save(KSimData & file) const
{
	const QString baseGroup(file.group());

	// Save last serial number
	file.writeEntry(sSerialNumber, m_serialNumberGenerator);
	
	// Write serial numbers
	QValueList<int> serialNumberList;
	FOR_EACH_CHANNEL(it, *getChannelList())
	{
		serialNumberList.append(it.current()->getSerialNumber());
	}
	file.writeEntry(sSerialList, serialNumberList);
	
	FOR_EACH_CHANNEL(it, *getChannelList())
	{
		file.setGroup(baseGroup + QString(sChannelX).arg(it.current()->getSerialNumber()));
		file.writeEntry(sChannelType, it.current()->getChannelTypeString());
		it.current()->save(file);
	}

	file.setGroup(baseGroup);
	
	file.writeEntry(sNumberChannels, getChannelList()->count());
		
	getZoomVar().save(file);
	
	file.setGroup(baseGroup);
	
	Component::save(file);	
	
}

/** Load DataRecorder properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool DataRecorder::load(KSimData & file, bool copyLoad)
{
	bool ok = true;
	const QString baseGroup(file.group());
	
	// Read last serial number
	m_serialNumberGenerator = file.readUnsignedNumEntry(sSerialNumber, 0);
	
	QValueList<int> serialNumberList;
	serialNumberList = file.readIntListEntry(sSerialList);
	
	// First we remove all unused channels
	FOR_EACH_CHANNEL(it, *getChannelList())
	{
		int cnt = serialNumberList.contains(it.current()->getSerialNumber());
		if (cnt == 0)
		{
			removeChannel(it.current());
		}
		else if (cnt != 1)
		{
			KSIMDEBUG_VAR("Bad serial number count", cnt);
		}
		// else cnt == 1 all okay
	}
	
  // Now we create/load the channels
  QValueList<int>::Iterator it;
  for( it = serialNumberList.begin(); it != serialNumberList.end(); ++it )
  {
		file.setGroup(baseGroup + QString(sChannelX).arg(*it));
//  	KSIMDEBUG(QString("Load Channel ") +  QString(sChannelX).arg(*it));
  	bool create = false;
  	DataRecorderChannelBase * channel = getChannelList()->findSerial(*it);
  	
  	if (!channel)
  	{
//      KSIMDEBUG("Create");
      // channel doesn't exist ==> create it
  		create = true;
  		QString typeStr(file.readEntry(sChannelType));
  		eChannelType type;
  		type = DataRecorderChannelBase::str2ChannelType(typeStr);
  	
  		channel = createChannel(type);
  	}
  	
  	channel->load(file, copyLoad);
/*  	KSIMDEBUG_VAR("", channel->getConnector()->getName());
  	KSIMDEBUG_VAR("", channel->getConnector()->getInitName());
  	KSIMDEBUG_VAR("", channel->getConnector()->getWireName());*/
  	
  	if (create)
  	{
  		appendChannel(channel);
  	}
  }
		
	file.setGroup(baseGroup);
	
	ok &= getZoomVar().load(file, copyLoad);
	if (getDataRecoderWidget() && getDataRecoderWidget()->getZoomWidget())
	{
		getZoomVar().copyTo(*getDataRecoderWidget()->getZoomWidget());	
	}
	
	file.setGroup(baseGroup);
	
	ok &= Component::load(file, copyLoad);	
	
	return ok;
}


void DataRecorder::calculate()
{
	FOR_EACH_CHANNEL(it, *m_channelList)
	{
		it.current()->fetchData();
	}
	m_dataCount++;
	executeNext();
}
	

void DataRecorder::reset()
{
	Component::reset();

	readSampleTime();
	
	FOR_EACH_CHANNEL(it, *m_channelList)
	{
		it.current()->reset();
	}
	m_dataCount = 0;
	if (getDataRecoderWidget() != 0)
	{
		getDataRecoderWidget()->getDataView()->reset();
	}
}

bool DataRecorder::initPopupMenu(QPopupMenu * popup)
{
	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	popup->insertItem(i18n("DataRecorder", "&Open graph"), this, SLOT(slotOpenWidget()));
	popup->insertItem(i18n("DataRecorder", "Add &boolean connector"), this, SLOT(slotAddBoolChannel()));
	popup->insertItem(i18n("DataRecorder", "Add &floating point connector"), this, SLOT(slotAddFloatChannel()));
	
	return true;
}
	

void DataRecorder::slotOpenWidget()
{
	if (m_widget == 0)
	{
		m_widget = new DataRecorderWidget(this, 0, getName());
		connect(m_widget,SIGNAL(signalDeleted()),SLOT(widgetDestroyed()));
//		KSIMDEBUG("Created");
	}
//	updateWidget();
	m_widget->show();
	m_widget->raise();
}

void DataRecorder::slotAddBoolChannel()
{
	DataRecorderChannelBoolean * channel;
	
	undoChangeProperty(i18n("DataRecorder", "Add channel"));
	channel = new DataRecorderChannelBoolean(this);
	newChannel(channel);
	setModified();
	
}

void DataRecorder::slotAddFloatChannel()
{
	DataRecorderChannelFloat * channel;
	
	undoChangeProperty(i18n("DataRecorder", "Add channel"));
	channel = new DataRecorderChannelFloat(this);
	newChannel(channel);
	setModified();
}


void DataRecorder::slotRemoveChannelConn(ConnectorBase * conn)
{
	bool found = false;
	
//	KSIMDEBUG("Remove Connector");
	FOR_EACH_CHANNEL(it, *getChannelList())
	{
		if (it.current()->getConnector() == conn)
		{
			undoChangeProperty(i18n("DataRecorder", "Remove channel"));
			found = true;
			removeChannel(it.current());
			setModified();
			break;
		}
	}
	
	if(!found)
	{
		KSIMDEBUG("Connector not found.");
	}
}

void DataRecorder::updateRequest()
{
	if (getDataRecoderWidget())
	{
		if (isRunning())
		{
			getDataRecoderWidget()->getDataView()->updateNext();
		}
		else
		{
			getDataRecoderWidget()->getDataView()->updateNow();
		}
	}
}

void DataRecorder::widgetDestroyed()
{
//	KSIMDEBUG("Destroyed");
	m_widget = 0;
}

unsigned int DataRecorder::getDataCount() const
{
	return m_dataCount;
}

DataRecorderWidget * DataRecorder::getDataRecoderWidget()
{
	return m_widget;
}

const DataRecorderWidget * DataRecorder::getDataRecoderWidget() const
{
	return m_widget;
}

/** Returns a unique number for the channels. */
int DataRecorder::nextSerialNumber()
{
	bool found;
	
	m_serialNumberGenerator++;
	
	do
	{
		found = false;
		FOR_EACH_CHANNEL(it, *getChannelList())
		{
			if (it.current()->getSerialNumber() == m_serialNumberGenerator)
			{
				m_serialNumberGenerator++;
				found = true;
				break;
			}
		}
	} while (found);
	
	return m_serialNumberGenerator;
}

double DataRecorder::getSampleTime() const
{
	return m_sampleTime;
}

void DataRecorder::readSampleTime()
{
	m_sampleTime = getTimeServer().getTick().getValue(unit_sec);
	
	if (getDataRecoderWidget() && getDataRecoderWidget()->getZoomWidget())
	{
		getDataRecoderWidget()->getZoomWidget()->updateMinZoom();
	}
}

void DataRecorder::undoZoom()
{

	if (getDataRecoderWidget() && getDataRecoderWidget()->getZoomWidget())
	{
		
		ZoomWidgetVar temp(*getDataRecoderWidget()->getZoomWidget());
		
		if (temp != getZoomVar())
		{
			// Todo: Is undo required ?
			//undoChangeProperty(i18n("DataRecorder", "Change time division"));
	
			getZoomVar().copyFrom(*getDataRecoderWidget()->getZoomWidget());
			setModified();

		}
	}
}


};  //namespace KSimLibDataRecorder

#include "datarecorder.moc"
