/***************************************************************************
                          datarecorderchannelfloat.h  -  description
                             -------------------
    begin                : Sun Feb 3 2002
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

#ifndef DATARECORDERCHANNELFLOAT_H
#define DATARECORDERCHANNELFLOAT_H


/**A floating point channel.
  *@author Rasmus Diekenbrock
  */

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "datarecorderchannelbase.h"

// Forward declaration

namespace KSimLibDataRecorder
{

class FloatStorage;


/**a boolean channel
  *@author Rasmus Diekenbrock
  */

		
class DataRecorderChannelFloat : public KSimLibDataRecorder::DataRecorderChannelBase
{
   Q_OBJECT
public:
	DataRecorderChannelFloat(DataRecorder * recorder);
	virtual ~DataRecorderChannelFloat();

	/** Fetches a data. This function is called if a single data is to record.
		*/
	virtual void fetchData();
	/** Draws a data set to the map.
			startSample:      index of the first sample to draw.
			stopSample:       index of the last sample to draw.
			horizontalOffset: horizontal position of thze first sample
			height:	          height of the paint device.
			samplePerPixel:   Number of samples to draw per pixel.
			verticalDivs:			Number of vertical divisions.
			Returns the index of the last drawn sample. */
	virtual int drawData(QPaintDevice * paintDev,
											 int startSample, int stopSample,
											 int horizontalOffset, int maxHeight,
											 double samplePerPixel, int verticalDivs);
  /** Resets channel. Delete data. */
  virtual void reset();

	/** Returns the property widget.
			This function has to create a widget at the frist call.
	 */
	virtual QWidget * getPropertyWidget(QWidget * parent = 0, const char * name = 0);

protected:
	double getData(unsigned int number);

	
private:
	FloatStorage * m_data;
	QWidget * m_propertyWidget;
	static unsigned int counter;
};




};  //namespace KSimLibDataRecorder

#endif
