/***************************************************************************
                          connectorboolin.h  -  description
                             -------------------
    begin                : Sun Oct 1 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef CONNECTORBOOLIN_H
#define CONNECTORBOOLIN_H

#include "connectorbase.h"

/**The class for boolean input connector
  *@author Rasmus Diekenbrock
  */

extern const ConnectorInfo ConnectorBoolInInfo;


class ConnectorBoolIn : public ConnectorInputBase
{
	
	Q_OBJECT

public:
	ConnectorBoolIn(	Component * comp,
						const char * name,
						const QPoint & pos	);
	
	// Setup the colors, brushs, and fills for the connector
	virtual void setupColorScheme (QPainter * p) const;

	/** Returns the input data */
	bool getInput() const;
	
	/** Creates the property widget */
	virtual QWidget* propertyWidget(QWidget * parent);

	virtual bool initPopupMenu(QPopupMenu * popup);
	
protected:
	/** Returns a pointer to the data that's read from the component
	  * The default implementation calls the function getWireData()
	  * Reimplementations is required if the connector has to modify ths data (e.g. a neg. boolean input */
	virtual const void * readoutData() const;

private slots:
	/** Display a status help message for popup menu entries, if highlighted */
	virtual void popupMenuHighlighted(int msg) const;
	/** Toggles the neg Type */
	void slotToggleNegType();
	
	
private:
	int idNegate;
};


#endif
