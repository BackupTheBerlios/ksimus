/***************************************************************************
                          connectorboolinedgepropertywidget.h  -  description
                             -------------------
    begin                : Fri Nov 23 2001
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

#ifndef CONNECTORBOOLINEDGEPROPERTYWIDGET_H
#define CONNECTORBOOLINEDGEPROPERTYWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorboolinpropertywidget.h"

// Forward declaration
class QCheckBox;
class ConnectorBoolInEdge;

/**Property widget for edge sensitive boolean input connector
  *@author Rasmus Diekenbrock
  */

class ConnectorBoolInEdgePropertyWidget : public ConnectorBoolInPropertyWidget
{
   Q_OBJECT
public: 
	ConnectorBoolInEdgePropertyWidget(ConnectorBoolInEdge * connector, QWidget *parent=0, const char *name=0);
//	~ConnectorBoolInEdgePropertyWidget();

public slots:
	/** The function slotAccept() is called, if changes are accepted.
		You have to reimplement this slot, if you add new properties.
		If you do so, then first emit signalChangeData() and than changed data!
	 */
	virtual void slotAccept();
	/** The function slotDefault() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void slotDefault();

protected:
	ConnectorBoolInEdge * getConn() const;
	ConnectorBoolInEdge * getConn();
	
	
private:
	QCheckBox * m_edgeSensitive;

};

#endif
