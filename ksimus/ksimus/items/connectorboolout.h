/***************************************************************************
                          connectorboolout.h  -  description
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

#ifndef CONNECTORBOOLOUT_H
#define CONNECTORBOOLOUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorbase.h"

// Forward declaration


/**The boolean output connector
  *@author Rasmus Diekenbrock
  */

extern const ConnectorInfo ConnectorBoolOutInfo;

class ConnectorBoolOut : public ConnectorOutputBase
{
	Q_OBJECT

friend class WirePropertyBoolean;

public:	
	/**
	 * Constructs a boolean output connector.
	 *
	 * @param comp Component which contains this connector.
	 * @param name The name of the connector. This name is shown at the property view
	 *             or the status bar.
	 * @param pos  Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolOut(	Component * comp,
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
	ConnectorBoolOut(	Component * comp,
						const QString & name,
						const QString & descr,
						const QPoint & pos = QPoint()	);
	
	
	
	void setOutput(bool out);
						
	// Setup the colors, brushs, and fills for the connector
	virtual void setupColorScheme (QPainter * p) const;
	

	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Creates the property widget */
	virtual QWidget* propertyWidget(QWidget * parent);

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
	ConnectorBoolOut(	Component * comp,
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
	
	bool m_data;

private slots:
	/** Display a status help message for popup menu entries, if highlighted */
	virtual void popupMenuHighlighted(int msg) const;
	/** Toggles the neg Type */
	void slotToggleNegType();
	
	
private:
	/** Internal init function. */
	void init();
	
	int idNegate;
};

#endif
