/***************************************************************************
                          textrec.cpp  -  description
                             -------------------
    begin                : Sat Mar 9 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
    email                : ksimus@gmx.de
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
#include <qpainter.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qvbuttongroup.h>
#include <qvbox.h>
#include <qlineedit.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdata.h"
#include "ksimus/ksimusdoc.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorbase.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimtimebase.h"
#include "ksimus/ksimtimeserver.h"
#include "ksimus/packageinfo.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/library.h"
#include "ksimus/connectorlibrary.h"
#include "ksimus/optionalconnector.h"

#include "config.h"

#include "textrec.h"


// Forward declaration


namespace KSimLibDataRecorder
{
		
#define FLAG_APPEND        0x0001
#define FLAG_HEADER_DATE   0x0002
#define FLAG_CONN_NAMES    0x0004
#define FLAG_LINE_NO       0x0008
#define FLAG_TIME_STAMP    0x0010

//#define DEFAULT_FLAGS      (FLAG_LINE_NO | FLAG_TIME_STAMP)
#define DEFAULT_FLAGS      (FLAG_HEADER_DATE | FLAG_CONN_NAMES | FLAG_LINE_NO | FLAG_TIME_STAMP)

#define DEFAULT_FILENAME   "text-recorder.log"
#define DEFAULT_PATH_TYPE  KSimFilename::PATH_RELATIVE_DOCUMENT
#define DEFAULT_SEPARATOR  QString::fromLatin1(";")




static const char * sNumberChannels   = "Number of channels";
static const char * sChannelX         = "Channel %1/";
static const char * sChannelType      = "Channel Type";
static const char * sSerialList       = "Serial List";
static const char * sLastSerialNumber = "Last Serial Number";
static const char * sSerialNumber     = "Serial Number";
static const char * sFile             = "File/";
static const char * sAppendFile       = "Append Data";
static const char * sAddHeaderDate    = "Add Header Date";
static const char * sAddConnNames     = "Add Connector Names";
static const char * sAddLineNo        = "Add Line Numbers";
static const char * sAddTimeStamp     = "Add Time Stamp";
static const char * sSeparator        = "Column Separator";


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new TextRec(container, ci);
}

const ComponentInfo * getTextRecInfo()
{
	static const ComponentInfo Info(i18n("DataRecorder-Component", "Text Recorder"),
	                                QString::fromLatin1("Recorder/Text Recorder"),
	                                i18n("DataRecorder-Component", "Recorder/Text Recorder"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-datarecorder-textrecorder"));
	return &Info;
}
//#############################################################################
//#############################################################################

ConnectorContainer::ConnectorContainer(ConnectorBase * connector, unsigned int serialNumber)
	:	m_conn(connector),
		m_serialNumber(serialNumber)
{
}
		
ConnectorContainer::~ConnectorContainer()
{
}

	
ConnectorBase * ConnectorContainer::getConnector() const
{
	return m_conn;
}

  	
void ConnectorContainer::setSerialNumber(unsigned int serial)
{
	m_serialNumber = serial;
}
unsigned int ConnectorContainer::getSerialNumber() const
{
	return m_serialNumber;
}

void ConnectorContainer::save(KSimData & file) const
{
	file.writeEntry(sSerialNumber, getSerialNumber());
	getConnector()->save(file);
}

bool ConnectorContainer::load(KSimData & file, bool /*copyLoad*/)
{
	setSerialNumber(file.readUnsignedNumEntry(sSerialNumber, 0));
	return getConnector()->load(file);
}


ConnectorContainer * ConnectorContainerList::findSerial(unsigned int no) const
{
	ConnectorContainer * connCont = 0;
	
	FOR_EACH_CONNECTOR_CONTAINER(it, *this)
	{
		if (it.current()->getSerialNumber() == no)
		{
		  connCont = it.current();
		  break;
		}
	}
	return connCont;
}

int ConnectorContainerList::compareItems(QCollection::Item di1, QCollection::Item di2)
{
	int serNo1 = ((ConnectorContainer *)di1)->getSerialNumber();
	int serNo2 = ((ConnectorContainer *)di2)->getSerialNumber();
	
	if(serNo1 < serNo2)
	{
		return -1;
	}
	if(serNo1 > serNo2)
	{
		return 1;
	}
	return 0;
}


//#################################################################
//#################################################################



TextRecView::TextRecView(Component * comp, eViewType viewType)
	: CompView(comp,viewType)
{
	setPlace(QRect(0, 0, 6*gridX, 5*gridY));
	enableRotation(true);
	enableConnectorSpacingTop(false);
	enableConnectorSpacingRight(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
}

TextRecView::~TextRecView()
{
}

void TextRecView::draw(QPainter * p)
{
	drawFrame(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Text\nRec");

	CompView::draw(p);
}


//###############################################################
//###############################################################



								
TextRec::TextRec(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_serialNumberGenerator(0),
		m_file(0),
		m_stream(0),
		m_filename(this, DEFAULT_FILENAME, DEFAULT_PATH_TYPE),
		m_separator(DEFAULT_SEPARATOR),
		m_flags(DEFAULT_FLAGS)
{
	
	m_trigger = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Trigger"),
	                             i18n("DataRecorder-Connector", "Trigger"),
	                             i18n("DataRecorder-Connector Trigger", "T"));
	CHECK_PTR(m_trigger);
	getTrigger()->setEdgeSensitive(false, true);
	// make Trigger Input optional
	new OptionalConnector(getTrigger(),
	                      QString::fromLatin1("Trigger Input"),
	                      i18n("DataRecorder", "Trigger input:"));
	
	connect(getTrigger(),SIGNAL(signalProperty()),SLOT(slotTriggerProperty()));
	
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new TextRecView(this, SHEET_VIEW);
	}
	
	m_connectorContainerList = new ConnectorContainerList;
	CHECK_PTR(m_connectorContainerList);
	m_connectorContainerList->setAutoDelete(true);

	connect(getDoc(), SIGNAL(signalStop()), this, SLOT(slotStopSim()));

}

TextRec::~TextRec()
{
	if (m_stream)
	{
		delete m_stream;
	}
	if (m_file)
	{
		m_file->close();
		delete m_file;
	}
	delete m_connectorContainerList;
}


ConnectorContainer * TextRec::newConnector(const QString connLibName)
{
	unsigned int c = 0, serialNo;
	bool found = false, ok;
	QString newName;
	
	ConnectorBase * conn;
	ConnectorContainer * connCont = 0;
	
	
	ok = g_library->getConnectorLib()->create(&conn, this, connLibName, connLibName, QPoint(0,0), connLibName);
	if (ok && conn)
	{		
		conn->setErasable(true);
		conn->getAction().disable(KSimAction::STORAGE);
		serialNo = nextSerialNumber();
		connCont = new ConnectorContainer(conn, serialNo);
		
		CHECK_PTR(connCont);
		
		connect(conn, SIGNAL(signalDeleteRequest(ConnectorBase *)),
		        this, SLOT(slotRemoveChannelConn(ConnectorBase *)));
		
		conn->setWireName(QString("Input %1").arg(serialNo));
	
		// Create a unique connector name
		QString baseName(conn->getInitName());
		baseName += " %1";
		baseName = baseName.simplifyWhiteSpace();
	
		do
		{
			c++;
			newName = baseName.arg(c);
			found = false;
			FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
			{
				if(newName == it.current()->getConnector()->getName())
				{
					found = true;
					break;
				}
			}
		}
		while (found);
	
		connCont->getConnector()->setName(newName);
	}
	else
	{
		logError(i18n("Creation of Connector %1 failed!").arg(connLibName));
	}

	return connCont;
}	

void TextRec::appendConnector(ConnectorContainer * connCont)
{
	getConnectorContainerList()->inSort(connCont);
	
	arrangeConnectors();	
}

void TextRec::removeConnector(ConnectorContainer * connCont)
{
	bool res;

	res = getConnList()->removeRef(connCont->getConnector());
	if (!res)
	{
		KSIMDEBUG("getConnList() remove failed");
	}
	
	res = getConnectorContainerList()->removeRef(connCont);
	if (!res)
	{
		KSIMDEBUG("m_channelInputList remove failed");
	}
	
	arrangeConnectors();	
}


ConnectorContainerList * TextRec::getConnectorContainerList() const
{
	return m_connectorContainerList;
}

void TextRec::arrangeConnectors()
{
	int c = 0;
	
	if(!getTrigger()->isHidden())
	{
		getTrigger()->setGridPos(QPoint(0, 2*c+1));
		c++;
	}
	
	FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
	{
		it.current()->getConnector()->setGridPos(QPoint(0, 2*c+1));
		c++;
	}

	
	if (getSheetView())
	{
		int newHeight;
	
	  if (/*getConnectorContainerList()->count()*/ c < 3)
	  {
			newHeight = 5 * gridY;
		}
		else
		{
			newHeight = (/*getConnectorContainerList()->count()*/c *2+1) * gridY;
		}
	
		QRect place = getSheetView()->getPlace();
		place.setHeight(newHeight);
		getSheetView()->setPlace(place, true);
		
	}
	
	refresh();	
}

/** Save TextRec properties */
void TextRec::save(KSimData & file) const
{
	const QString baseGroup(file.group());

	// Save last serial number
	file.writeEntry(sLastSerialNumber, m_serialNumberGenerator);
	
	// Write connector names
	QValueList<int> serialNumberList;
	FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
	{
		serialNumberList.append(it.current()->getSerialNumber());
	}
	file.writeEntry(sSerialList, serialNumberList);
	
	FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
	{
		file.setGroup(baseGroup + QString(sChannelX).arg(it.current()->getSerialNumber()));
		file.writeEntry(sChannelType, it.current()->getConnector()->getConnType());
		it.current()->save(file);
	}

	file.setGroup(baseGroup);
	
	file.writeEntry(sNumberChannels, getConnectorContainerList()->count());
		
	m_filename.save(file, sFile);
	
	if (isAppendEnabled())
	{
		file.writeEntry(sAppendFile, true);
	}
	if (isLineNoEnabled())
	{
		file.writeEntry(sAddLineNo, true);
	}
	if (isTimeStampEnabled())
	{
		file.writeEntry(sAddTimeStamp, true);
	}
	if (isHeaderDateEnabled())
	{
		file.writeEntry(sAddHeaderDate, true);
	}
	if (isConnectorNamesEnabled())
	{
		file.writeEntry(sAddConnNames, true);
	}
	if (getSeparator() != DEFAULT_SEPARATOR)
	{
		file.writeEntry(sSeparator, getSeparator());
	}
	
	Component::save(file);	
	
}

/** Load TextRec properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool TextRec::load(KSimData & file, bool copyLoad)
{
	bool ok = true;
	const QString baseGroup(file.group());
	
	// Read last serial number
	m_serialNumberGenerator = file.readUnsignedNumEntry(sLastSerialNumber, 0);
	
	QValueList<int> serialNumberList;
	serialNumberList = file.readIntListEntry(sSerialList);
	
	// First we remove all unused channels
	FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
	{
		int cnt = serialNumberList.contains(it.current()->getSerialNumber());
		if (cnt == 0)
		{
			removeConnector(it.current());
		}
		else if (cnt != 1)
		{
			KSIMDEBUG_VAR("Bad serial number count", cnt);
		}
		// else cnt == 1 all okay
	}
	
  // Now we create/load the channels
  QValueList<int>::Iterator it;
  for(it = serialNumberList.begin(); it != serialNumberList.end(); ++it)
  {
		file.setGroup(baseGroup + QString(sChannelX).arg(*it));
//  	KSIMDEBUG(QString("Load Channel ") +  QString(sChannelX).arg(*it));
  	ConnectorContainer * connCont = getConnectorContainerList()->findSerial(*it);
  	
  	if (!connCont)
  	{
//      KSIMDEBUG("Create");
      // connector doesn't exist ==> create it
  		const QString typeStr(file.readEntry(sChannelType));
  	
  		connCont = newConnector(typeStr);
	  	
	  	if (connCont)
	  	{
		  	connCont->load(file, copyLoad);
				connCont->getConnector()->setWireName(QString("Input %1").arg(*it));
  			appendConnector(connCont);
  		}
  	}
  	else
  	{
	  	connCont->load(file, copyLoad);
		}
/*  	KSIMDEBUG_VAR("", channel->getConnector()->getName());
  	KSIMDEBUG_VAR("", channel->getConnector()->getInitName());
  	KSIMDEBUG_VAR("", channel->getConnector()->getWireName());*/
  }
		
	file.setGroup(baseGroup);
	
	ok &= m_filename.load(file, sFile);

	setAppendEnabled(file.readBoolEntry(sAppendFile, false));
	setLineNoEnabled(file.readBoolEntry(sAddLineNo, false));
	setTimeStampEnabled(file.readBoolEntry(sAddTimeStamp, false));
	setHeaderDateEnabled(file.readBoolEntry(sAddHeaderDate, false));
	setConnectorNamesEnabled(file.readBoolEntry(sAddConnNames, false));
	setSeparator(file.readEntry(sSeparator, DEFAULT_SEPARATOR));
	
	ok &= Component::load(file, copyLoad);	
	
	return ok;
}


void TextRec::calculate()
{
	Component::calculate();
	
	if (!m_stream) return;
	
	if(getTrigger()->isHidden())
	{
		executeNext();
	}
	else
	{
		if(!getTrigger()->getInput())
		{
			// Input is not active
			return;
		}
	}
	
	if (isLineNoEnabled())
	{
		*m_stream << m_lineCnt++ << m_separator;
	}
	
	if (isTimeStampEnabled())
	{
		*m_stream << getTimeServer().getTime().getValue(unit_sec) << m_separator;
	}
	
	FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
	{
		*m_stream << it.current()->getConnector()->getValueText() << m_separator;
	}
	*m_stream << "\n";
}
	

void TextRec::reset()
{
	Component::reset();
	
	m_lineCnt = 1;
}

int TextRec::checkCircuit()	
{
	int err = Component::checkCircuit();	
	
	// File and Stream should be deleted!
	if (m_stream)
	{
		KSIMDEBUG("Stream is opened???");
		delete m_stream;
		m_stream = (QTextStream *)0;
	}
	if (m_file)
	{
		KSIMDEBUG("File is opened???");
		delete m_file;
		m_file = (QFile *)0;
	}
	
	
	if (!m_filename.getFilename().isEmpty())
	{
	
		// Check file
		QFileInfo fi(m_filename.getFilename());
	
		if (!m_filename.isPathValid())
		{
			KSIMDEBUG("Filename is not valid. Document not saved?");
			logError(i18n("Filename is not valid. Document not saved?"));
			err++;
		}
		else if (fi.exists())
		{
			//File exists!
			if (!fi.isWritable())
			{
				KSIMDEBUG("Not Writable");
				logError(i18n("File %1 is not writable").arg(m_filename.getFilename()));
				err++;
			}
		}
		else
		{
			//File does not exists!
			QFileInfo dir(fi.dirPath(true));
			if (!dir.exists())
			{
				// Directory does not exists
				KSIMDEBUG("Directory does not exists");
				logError(i18n("Directory %1 not exists").arg(fi.dirPath(true)));
				err++;
			}
		}	
	
		if (err == 0)
		{
			// Do not if error is detected
			m_file = new QFile(m_filename.getFilename());
			CHECK_PTR(m_file);
	
			int mode = IO_WriteOnly;
			if (isAppendEnabled())
			{
				mode |= IO_Append;
			}
			
			if (m_file->open(mode))
			{
				m_stream = new QTextStream(m_file);
				if (isHeaderDateEnabled())
				{
					*m_stream << QDateTime::currentDateTime().toString() << "\n";
				}
				if (isConnectorNamesEnabled())
				{
					if(isLineNoEnabled())
					{
						*m_stream << "Line" << m_separator;
					}
					if(isTimeStampEnabled())
					{
						*m_stream << "Time" << m_separator;
					}
					FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
					{
						*m_stream << it.current()->getConnector()->getName() << m_separator;
					}
					*m_stream << "\n";
				}
			}
			else
			{
				KSIMDEBUG("File open failed");
				logError(i18n("File open failed (Filename %1)").arg(m_filename.getFilename()));
				err++;
			}
		}
	}
	else
	{
		KSIMDEBUG("No filename");
		logError(i18n("No filename"));
		err++;
	}

	return err;
}

bool TextRec::initPopupMenu(QPopupMenu * popup)
{

	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	popup->insertItem(i18n("Add &boolean connector"), this, SLOT(slotAddBoolChannel()));
	popup->insertItem(i18n("Add &floating point connector"), this, SLOT(slotAddFloatChannel()));
	
	return true;
}
	
	
void TextRec::slotAddBoolChannel()
{
	ConnectorContainer * connCont;
	undoChangeProperty(i18n("Add channel"));
	connCont = newConnector("Boolean Input");
	if (connCont)
	{
		appendConnector(connCont);
		setModified();
	}
}

void TextRec::slotAddFloatChannel()
{
	ConnectorContainer * connCont;
	undoChangeProperty(i18n("Add channel"));
	connCont = newConnector("Floating Point Input");
	if (connCont)
	{
		appendConnector(connCont);
		setModified();
	}
}


void TextRec::slotRemoveChannelConn(ConnectorBase * conn)
{
	bool found = false;
	
	KSIMDEBUG("Remove Connector");
	FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
	{
		if (it.current()->getConnector() == conn)
		{
			undoChangeProperty(i18n("Remove channel"));
			found = true;
			removeConnector(it.current());
			setModified();
			break;
		}
	}
	
	if(!found)
	{
		KSIMDEBUG("Connector not found.");
	}
}


/** Returns a unique number for the channels. */
int TextRec::nextSerialNumber()
{
	bool found;
	
	m_serialNumberGenerator++;
	
	do
	{
		found = false;
		FOR_EACH_CONNECTOR_CONTAINER(it, *getConnectorContainerList())
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

void TextRec::slotStopSim()
{
	if (m_stream)
	{
		delete m_stream;
		m_stream = (QTextStream *)0;
	}
	if (m_file)
	{
		m_file->close();
		delete m_file;
		m_file = (QFile *)0;
	}
}

ComponentPropertyBaseWidget * TextRec::createGeneralProperty(QWidget *parent)
{
	TextRecPropertyGeneralWidget * wid;
	wid = new TextRecPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

void TextRec::setAppendEnabled(bool append)
{
	if (append)
	{
		m_flags |= FLAG_APPEND;
	}
	else
	{
		m_flags &= ~FLAG_APPEND;
	}
}

bool TextRec::isAppendEnabled() const
{
	return m_flags & FLAG_APPEND;
}
	
void TextRec::setLineNoEnabled(bool enabled)
{
	if (enabled)
	{
		m_flags |= FLAG_LINE_NO;
	}
	else
	{
		m_flags &= ~FLAG_LINE_NO;
	}
}

bool TextRec::isLineNoEnabled() const
{
	return m_flags & FLAG_LINE_NO;
}
	
void TextRec::setTimeStampEnabled(bool enabled)
{
	if (enabled)
	{
		m_flags |= FLAG_TIME_STAMP;
	}
	else
	{
		m_flags &= ~FLAG_TIME_STAMP;
	}
}

bool TextRec::isTimeStampEnabled() const
{
	return m_flags & FLAG_TIME_STAMP;
}
	
void TextRec::setHeaderDateEnabled(bool enabled)
{
	if (enabled)
	{
		m_flags |= FLAG_HEADER_DATE;
	}
	else
	{
		m_flags &= ~FLAG_HEADER_DATE;
	}
}

bool TextRec::isHeaderDateEnabled() const
{
	return m_flags & FLAG_HEADER_DATE;
}

void TextRec::setConnectorNamesEnabled(bool enabled)
{
	if (enabled)
	{
		m_flags |= FLAG_CONN_NAMES;
	}
	else
	{
		m_flags &= ~FLAG_CONN_NAMES;
	}
}

bool TextRec::isConnectorNamesEnabled() const
{
	return m_flags & FLAG_CONN_NAMES;
}

void TextRec::setSeparator(QString separator)
{
	m_separator = separator;
}

QString TextRec::getSeparator() const
{
	return m_separator;
}

void TextRec::slotTriggerProperty()
{
	arrangeConnectors();
}

//############################################################################################
	
TextRecPropertyGeneralWidget::TextRecPropertyGeneralWidget(TextRec * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_filenameLabel = new QLabel(i18n("File:"), this, "m_filenameLabel");
	CHECK_PTR(m_filenameLabel);
	
	m_filename = new KSimFilenameWidget(comp->getFilename(),
	                                    KSimFilename::PATH_ABSOLUTE | KSimFilename::PATH_RELATIVE_DOCUMENT
	                                    | KSimFilename::PATH_RELATIVE_HOME,
	                                    this, "m_filename");
	CHECK_PTR(m_filename);
	
	m_separatorLabel = new QLabel(i18n("Column separator:"), this, "m_separatorLabel");
	CHECK_PTR(m_separatorLabel);
	m_separator = new QLineEdit(comp->getSeparator(), this, "m_separator");
	CHECK_PTR(m_separator);
	tip = i18n("Edit the column separator here.");
	addToolTip(tip, m_separator, m_separatorLabel);
	addWhatsThis(tip, m_separator, m_separatorLabel);


	QVBox * vbox = newRowVBox("vbox");
	CHECK_PTR(vbox);
	
	QVButtonGroup * butGrp = new QVButtonGroup(i18n("Switches:"), vbox, "butGrp");
	CHECK_PTR(butGrp);

	m_append = new QCheckBox(i18n("Append data to existing file"), butGrp, "m_append");
	CHECK_PTR(m_append);
	m_append->setChecked(comp->isAppendEnabled());
	tip = i18n("If checked an exisiting file will not be deleted before a new simulations is started.");
	addToolTip(tip, m_append);
	addWhatsThis(tip, m_append);
	
	m_headerDate = new QCheckBox(i18n("Add date and time at the top"), butGrp, "m_headerDate");
	CHECK_PTR(m_headerDate);
	m_headerDate->setChecked(comp->isHeaderDateEnabled());
	tip = i18n("If checked the date and time of the simulation start is added top of the new data.");
	addToolTip(tip, m_headerDate);
	addWhatsThis(tip, m_headerDate);
	
	m_connectorNames = new QCheckBox(i18n("Add the connector names at the top"), butGrp, "m_connectorNames");
	CHECK_PTR(m_connectorNames);
	m_connectorNames->setChecked(comp->isConnectorNamesEnabled());
	tip = i18n("If checked the the connector names are added top of the new data.");
	addToolTip(tip, m_connectorNames);
	addWhatsThis(tip, m_connectorNames);
	

	m_lineNo = new QCheckBox(i18n("Add line numbers"), butGrp, "m_lineNo");
	CHECK_PTR(m_lineNo);
	m_lineNo->setChecked(comp->isLineNoEnabled());
	tip = i18n("If checked the lines in the log file are numbered.");
	addToolTip(tip, m_lineNo);
	addWhatsThis(tip, m_lineNo);
	
	m_timeStamp = new QCheckBox(i18n("Add time stamp"), butGrp, "m_timeStamp");
	CHECK_PTR(m_timeStamp);
	m_timeStamp->setChecked(comp->isTimeStampEnabled());
	tip = i18n("If checked a time stamp is added in front of each line (after the optional line number).");
	addToolTip(tip, m_timeStamp);
	addWhatsThis(tip, m_timeStamp);
}

void TextRecPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();

	if (getComponent()->getFilename() != getFilenameWidget()->getFileInfo())
	{
		changeData();
		getComponent()->getFilename() = getFilenameWidget()->getFileInfo();
	}

	if (getComponent()->isAppendEnabled() != getAppendFileCheckBox()->isChecked())
	{
		changeData();
		getComponent()->setAppendEnabled(getAppendFileCheckBox()->isChecked());
	}
		
	if (getComponent()->isLineNoEnabled() != getLineNumberCheckBox()->isChecked())
	{
		changeData();
		getComponent()->setLineNoEnabled(getLineNumberCheckBox()->isChecked());
	}
		
	if (getComponent()->isTimeStampEnabled() != getTimeStampCheckBox()->isChecked())
	{
		changeData();
		getComponent()->setTimeStampEnabled(getTimeStampCheckBox()->isChecked());
	}
		
	if (getComponent()->isHeaderDateEnabled() != getHeaderDateCheckBox()->isChecked())
	{
		changeData();
		getComponent()->setHeaderDateEnabled(getHeaderDateCheckBox()->isChecked());
	}
	
	if (getComponent()->isConnectorNamesEnabled() != getConnectorNamesCheckBox()->isChecked())
	{
		changeData();
		getComponent()->setConnectorNamesEnabled(getConnectorNamesCheckBox()->isChecked());
	}
	
	if (getComponent()->getSeparator() != getSeparatorEdit()->text())
	{
		changeData();
		getComponent()->setSeparator(getSeparatorEdit()->text());
	}
}


void TextRecPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	getFilenameWidget()->setPathType(DEFAULT_PATH_TYPE);
	getFilenameWidget()->setFilename(DEFAULT_FILENAME);


	getAppendFileCheckBox()->setChecked(DEFAULT_FLAGS & FLAG_APPEND);
	getLineNumberCheckBox()->setChecked(DEFAULT_FLAGS & FLAG_LINE_NO);
	getTimeStampCheckBox()->setChecked(DEFAULT_FLAGS & FLAG_TIME_STAMP);
	getHeaderDateCheckBox()->setChecked(DEFAULT_FLAGS & FLAG_HEADER_DATE);
	getConnectorNamesCheckBox()->setChecked(DEFAULT_FLAGS & FLAG_CONN_NAMES);
	getSeparatorEdit()->setText(DEFAULT_SEPARATOR);

}

};  //namespace KSimLibDataRecorder

