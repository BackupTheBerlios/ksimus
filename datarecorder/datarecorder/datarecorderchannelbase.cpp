/***************************************************************************
                          datarecorderchannelbase.cpp  -  description
                             -------------------
    begin                : Fri May 18 2001
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
#include <qpainter.h>
#include <qpoint.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "datarecorder.h"
#include "datarecorderchannelbase.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/enumdict.h"
#include "ksimus/connectorbase.h"
// Forward declaration


namespace KSimLibDataRecorder
{

// Statics

static const char * sColor    = "Color";
static const char * sVertGain = "Vertical Gain";
static const char * sVertOfs  = "Vertical Offset";
static const char * sSerial   = "Serial Number";




//#######################################################################
//#######################################################################

static EnumDict<eChannelType> channelTypeDict;

EnumDict<eChannelType>::tData EnumDict<eChannelType>::data[]
			= { {"Unknown", KSimLibDataRecorder::CT_Unknown},
          {"Boolean",  KSimLibDataRecorder::CT_Boolean},
          {"Floating Point",  KSimLibDataRecorder::CT_Float},
          {0,(KSimLibDataRecorder::eChannelType)0}};

/** Converts the channel type */
eChannelType DataRecorderChannelBase::str2ChannelType(const char * str)
{
	return channelTypeDict.find(str, CT_Unknown);
}

const char * DataRecorderChannelBase::channelType2Str(eChannelType ct)
{
	return channelTypeDict.find(ct);
}

//#######################################################################
//#######################################################################


DataRecorderChannelBase::DataRecorderChannelBase(DataRecorder * recorder)
	: QObject(recorder),
		m_recorder(recorder),
		m_lineColor(black),
		m_vertOffset(0.0),
		m_vertGain(1),
		m_channelType(CT_Unknown),
		m_serialNumber(0)
{
}

DataRecorderChannelBase::~DataRecorderChannelBase()
{
}

DataRecorder * DataRecorderChannelBase::getDataRecorder() const
{
	return m_recorder;
}

ConnectorInputBase * DataRecorderChannelBase::getConnector() const
{
	return m_input;
}

/** save channel properties */
void DataRecorderChannelBase::save(KSimData & file) const
{
	file.writeEntry(sColor, getLineColor());
	file.writeEntry(sVertGain, getVerticalGain());
	file.writeEntry(sVertOfs, getVerticalOffset());
	file.writeEntry(sSerial, getSerialNumber());
	m_input->save(file);
}

/** load channel properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
bool DataRecorderChannelBase::load(KSimData & file, bool copyLoad)
{
	setLineColor(file.readColorEntry(sColor, &getLineColor()));
	setVerticalGain(file.readDoubleNumEntry(sVertGain, getVerticalGain()));
	setVerticalOffset(file.readDoubleNumEntry(sVertOfs, getVerticalOffset()));
	setSerialNumber(file.readUnsignedNumEntry(sSerial, 0));
	return m_input->load(file);
}


void DataRecorderChannelBase::setLineColor(const QColor & color)
{
	if (m_lineColor != color)
	{
		m_lineColor = color;
		emit lineColorChanged(color);
		getDataRecorder()->setModified();
		getDataRecorder()->updateRequest();
	}
}

const QColor & DataRecorderChannelBase::getLineColor() const
{
	return m_lineColor;
}

void DataRecorderChannelBase::setVerticalOffset(double offset)
{
	if (m_vertOffset != offset)
	{
		m_vertOffset = offset;
		getDataRecorder()->setModified();
		getDataRecorder()->updateRequest();
	}
}

double DataRecorderChannelBase::getVerticalOffset() const
{
	return m_vertOffset;
}

void DataRecorderChannelBase::setVerticalGain(double gain)
{
	if (m_vertGain != gain)
	{
		m_vertGain = gain;
		getDataRecorder()->setModified();
		getDataRecorder()->updateRequest();
	}
}

double DataRecorderChannelBase::getVerticalGain() const
{
	return m_vertGain;
}

/** Set channel name.
	*	The channel name is the same as the name of the according connector. */
void DataRecorderChannelBase::setChannelName(const QString & name)
{
	QString simpStr(name.simplifyWhiteSpace());
	if (simpStr != getConnector()->getName())
	{
		getDataRecorder()->undoChangeProperty(i18n("Change Channel Name"));
		
		getConnector()->setName(simpStr);
		getDataRecorder()->setModified();
		getDataRecorder()->executePropertyCheck();
		
	}
}


/** Returns the channel type */
eChannelType DataRecorderChannelBase::getChannelType() const
{
	return m_channelType;
}

/** Returns the channel type string */
const char * DataRecorderChannelBase::getChannelTypeString() const
{
	return channelType2Str(getChannelType());
}

/** Sets the channel type */
void DataRecorderChannelBase::setChannelType(eChannelType ct)
{
	m_channelType = ct;
}

void DataRecorderChannelBase::setSerialNumber(unsigned int no)
{
	m_serialNumber = no;
	getConnector()->setWireName(QString("Input %1").arg(no));
};

//#############################################################################
//#############################################################################



DataRecorderChannelBase * DataRecorderChannelList::findSerial(unsigned int no) const
{
	DataRecorderChannelBase * channel = 0;
	
	FOR_EACH_CHANNEL(it, *this)
	{
		if (it.current()->getSerialNumber() == no)
		{
		  channel = it.current();
		  break;
		}
	}
	return channel;
}

int DataRecorderChannelList::compareItems(QCollection::Item di1, QCollection::Item di2)
{
	int serNo1 = ((DataRecorderChannelBase *)di1)->getSerialNumber();
	int serNo2 = ((DataRecorderChannelBase *)di2)->getSerialNumber();
	
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



};  //namespace KSimLibDataRecorder



