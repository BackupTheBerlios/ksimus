/***************************************************************************
                          connectorlabel.h  -  description
                             -------------------
    begin                : Fri Nov 2 2001
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

#ifndef CONNECTORLABEL_H
#define CONNECTORLABEL_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "componentaddon.h"

// Forward declaration
class ConnectorBase;
class ConnectorBoolInEdge;

/**Adds a label to a connector.
  *@author Rasmus Diekenbrock
  */


class ConnectorLabel : public ComponentAddOn
{
Q_OBJECT

public:
	/** Constructs a new ConnnectorLabel.
		*	The objects will be deleted if the associated connector is deleted. Construct it and forget it :).
		*/
	ConnectorLabel(ConnectorBase * conn, const QString & descr);
	
	/** Constructs a new ConnnectorLabel.
		*	Like the constructor above but accepts a edge sensitive boolean input connector. In this case
		* the triangle is drawn, which marks a edge sensitive connector.
		*/
	ConnectorLabel(ConnectorBoolInEdge * conn, const QString & descr);
	
	/** Draw the sheet view.
		* The implementation draws the text depending on the connectors direction.
		*/
	virtual void drawSheetView (QPainter *p) const;
	
	/** Set the spacing between the component frame and the label.
		*	@param spacing Distance given in pixel
		*/
	void setSpacing(int spacing);
	/** Return the spacing between the component frame and the label.
		*	@return spacing Distance given in pixel
		*/
	int getSpacing() const;
	

private slots:
	void slotConnDeleted();

private:
	/** Internal init function. */
	void init();
	
	ConnectorBase * m_conn;
	QString m_descr;
	int m_spacing;
	Q_UINT32 m_flags;

};

#endif
