/***************************************************************************
                          datarecorderchannelbase.h  -  description
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

#ifndef DATARECORDERCHANNELBASE_H
#define DATARECORDERCHANNELBASE_H

// C-Includes

// QT-Includes
#include <qobject.h>
#include <qcolor.h>
#include <qlist.h>

// KDE-Includes

// KSimus-Includes
//#include <ksimus/componentaddon.h>

// Project-Includes

// Forward declaration
class KSimData;
class QPaintDevice;
class ConnectorInputBase;


namespace KSimLibDataRecorder
{

class DataRecorder;


/**Base class for all data recorder channels
  *@author Rasmus Diekenbrock
  */


enum eChannelType
{
	CT_Unknown = 0,
	CT_Boolean = 1,
	CT_Float = 2
};



class DataRecorderChannelBase : public QObject
{
   Q_OBJECT
public:
	virtual ~DataRecorderChannelBase();

	/** Returns the parent data recorder.
		*/
	DataRecorder * getDataRecorder() const;
	/** Returns the connector. */
	ConnectorInputBase * getConnector() const;
	
	/** Fetches a data. This function is called if a single data is to record.
		*/
	virtual void fetchData() = 0;
	/** Draws a data set to the map.
	    startSample:      index of the first sample to draw.
	    stopSample:       index of the last sample to draw.
	    horizontalOffset: horizontal position of thze first sample
	    height:           height of the paint device.
	    samplePerPixel:   Number of samples to draw per pixel.
	    verticalDivs:     Number of vertical divisions.
	    Returns the index of the last drawn sample. */
	virtual int drawData(QPaintDevice * paintDev,
	                     int startSample, int stopSample,
	                     int horizontalOffset, int maxHeight,
	                     double samplePerPixel, int verticalDivs) = 0;
	/** Resets channel. Delete data. */
	virtual void reset() = 0;

	/** save channel properties */
	virtual void save(KSimData & file) const;
	/** load channel properties
	*   copyLoad is true, if the load function is used as a copy function
	*   Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Returns the current line color. */
	const QColor & getLineColor() const;

	/** Returns the vertical offset. */
	double getVerticalOffset() const;

	/** Returns the vertical gain. */
	double getVerticalGain() const;

	/** Returns the channel type */
	eChannelType getChannelType() const;

	/** Returns the channel type string */
	const char * getChannelTypeString() const;

	/** Converts the channel type */
	static eChannelType str2ChannelType(const char * str);
	static const char * channelType2Str(eChannelType ct);

	/** Returns the property widget.
	    This function has to create a widget at the frist call.
	 */
	virtual QWidget * getPropertyWidget(QWidget * parent = 0, const char * name = 0) = 0;


	void setSerialNumber(unsigned int no);
	unsigned int getSerialNumber() const { return m_serialNumber; };


public slots:
	/** Set the vertical offset. */
	void setVerticalOffset(double offset);

	/** Set the vertical gain. */
	void setVerticalGain(double gain);

	/** Set the line color. */
	void setLineColor(const QColor & color);

	/** Set channel name.
	    The channel name is the same as the name of the according connector. */
	void setChannelName(const QString & name);

protected:
	/** Sets the channel type */
	void setChannelType(eChannelType ct);
	
	
	DataRecorderChannelBase(DataRecorder * recorder);
	ConnectorInputBase * m_input;

private:
	DataRecorder * m_recorder;
	QColor m_lineColor;
	double m_vertOffset;
	double m_vertGain;
	eChannelType m_channelType;
	unsigned int m_serialNumber;

	static const char * const sColor;
	static const char * const sVertGain;
	static const char * const sVertOfs;
	static const char * const sSerial;

signals:
	void lineColorChanged(const QColor & newColor);


};

//#############################################################################
//#############################################################################



class DataRecorderChannelList : public QList<DataRecorderChannelBase>
{
public:
	DataRecorderChannelBase * findSerial(unsigned int no) const;

protected:
	virtual int compareItems(QCollection::Item di1, QCollection::Item di2);
};

#define FOR_EACH_CHANNEL(_it_,_channelList_)	\
		for(QListIterator<DataRecorderChannelBase> _it_(_channelList_);_it_.current();++_it_)




};  //namespace KSimLibDataRecorder


#endif
