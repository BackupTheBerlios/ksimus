/***************************************************************************
                          connectorfloatout.h  -  description
                             -------------------
    begin                : Fri Dec 28 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#ifndef CONNECTORFLOATOUT_H
#define CONNECTORFLOATOUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorbase.h"

// Forward declaration


/**Output connector for floating point values.
  *@author Rasmus Diekenbrock
  */

extern const ConnectorInfo ConnectorFloatOutInfo;

class ConnectorFloatOut : public ConnectorOutputBase
{
	Q_OBJECT

friend class WirePropertyFloatingPoint;

public:	
	/**
	 * Constructs a boolean output connector.
	 *
	 * @param comp Component which contains this connector.
	 * @param name The name of the connector. This name is shown at the property view
	 *             or the status bar.
	 * @param pos  Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorFloatOut(Component * comp,
	                  const QString & name,
	                  const QPoint & pos = QPoint()	);
	
	/**
	 * Constructs a boolean output connector. Like the constructor above, but creates also a
	 * connector label (@ref ConnectorLabel).
	 *
	 * @param comp  Component which contains this connector.
	 * @param name  The name of the connector. This name is shown at the property view
	 *              or the status bar.
	 * @param descr Sets the description of the connector label.
	 * @param pos   Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorFloatOut(Component * comp,
	                  const QString & name,
	                  const QString & descr,
	                  const QPoint & pos = QPoint()	);
	
	
	
	void setOutput(double out);
						
	// Setup the colors, brushs, and fills for the connector
	virtual void setupColorScheme (QPainter * p) const;
	
	/** Returns a text which represents the current value. */
	virtual QString getValueText() const;


protected:
	/**
	 * Constructs a boolean output connector. Use this constructor if you derive this class.
	 *
	 * @param comp Component which contains this connector.
	 * @param name The name of the connector. This name is shown at the property view
	 *             or the status bar.
	 * @param pos  Sets the position of the connctor. The position has to be given in grids.
	 * @param orient Sets the orientation of the connector.
	 * @param ci     Sets the connector info (@ref ConnectorInfo):
	 */
	ConnectorFloatOut(Component * comp,
	                  const QString & name,
	                  const QPoint & pos,
	                  ConnOrientationType orient,
	                  const ConnectorInfo * ci);
						
	/** The function copyData() has to copy data to the output variable
	  * The default implementation doest nothing
	  * Reimplementations is required for all output connectors  */
	virtual void copyData(const void * pData);
    /** Returns a pointer to the data of this output connector */
	virtual const void * getData() const;
	
	double m_data;

	
	
private:
	/** Internal init function. */
	void init();
	
};



#endif
