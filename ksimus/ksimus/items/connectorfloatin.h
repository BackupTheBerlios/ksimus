/***************************************************************************
                          connectorfloatin.h  -  description
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

#ifndef CONNECTORFLOATIN_H
#define CONNECTORFLOATIN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorbase.h"

// Forward declaration

// Project-Includes


extern const ConnectorInfo ConnectorFloatInInfo;



/**Input connector for floating point values
  *@author Rasmus Diekenbrock
  */

class ConnectorFloatIn : public ConnectorInputBase
{
	
	Q_OBJECT

public:
	/**
	 * Constructs a floating point input connector.
	 *
	 * @param comp Component which contains this connector.
	 * @param name The name of the connector. This name is shown at the property view
	 *             or the status bar.
	 * @param pos  Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorFloatIn(Component * comp,
	                 const QString & name,
	                 const QPoint & pos = QPoint()	);
	
	/**
	 * Constructs a floating point input connector. Like the constructor above, but creates also a
	 * connector label (@ref ConnectorLabel).
	 *
	 * @param comp  Component which contains this connector.
	 * @param name  The name of the connector. This name is shown at the property view
	 *              or the status bar.
	 * @param descr Sets the description of the connector label.
	 * @param pos   Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorFloatIn(Component * comp,
	                 const QString & name,
	                 const QString & descr,
	                 const QPoint & pos = QPoint()	);
	
	
	// Setup the colors, brushs, and fills for the connector
	virtual void setupColorScheme (QPainter * p) const;

	/** Returns the input data */
	double getInput() const;
	
	/** Creates the property widget */
//	virtual QWidget* propertyWidget(QWidget * parent);

	/** Add menu items depend on connetor properties */
//	virtual bool initPopupMenu(QPopupMenu * popup);
	
protected:
	/**
	 * Constructs a floating point input connector. Use this constructor if you derive this class.
	 *
	 * @param comp Component which contains this connector.
	 * @param name The name of the connector. This name is shown at the property view
	 *             or the status bar.
	 * @param pos  Sets the position of the connctor. The position has to be given in grids.
	 * @param orient Sets the orientation of the connector.
	 * @param ci     Sets the connector info (@ref ConnectorInfo):
	 */
	ConnectorFloatIn(Component * comp,
	                 const QString & name,
	                 const QPoint & pos,
	                 ConnOrientationType orient,
	                 const ConnectorInfo * ci);
	
	
	
	/** Returns a pointer to the data that's read from the component
	  * The default implementation calls the function getWireData()
	  * Reimplementations is required if the connector has to modify this data (e.g. a neg. boolean input */
//	virtual const void * readoutData() const;

protected slots:
	/** Display a status help message for popup menu entries, if highlighted */
//	virtual void popupMenuHighlighted(int msg) const;
private slots:
	/** Toggles the neg Type */
//	void slotToggleNegType();
	
	
private:
	/** Internal init function. */
	void init();

};

#endif
