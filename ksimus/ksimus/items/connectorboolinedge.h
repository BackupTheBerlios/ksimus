/***************************************************************************
                          connectorboolinedge.h  -  description
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

#ifndef CONNECTORBOOLINEDGE_H
#define CONNECTORBOOLINEDGE_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorboolin.h"

// Forward declaration

/**A edge sensitive boolean input connector 
  *@author Rasmus Diekenbrock
  */

extern const ConnectorInfo ConnectorBoolInInfoEdge;


class ConnectorBoolInEdge : public ConnectorBoolIn
{
	
	Q_OBJECT

public:
	ConnectorBoolInEdge(Component * comp, const char * name, const QPoint & pos = QPoint());
//	~ConnectorBoolInEdge();

	/** True sets the connector edge sensitive. False sets the connector level sensitive.
	 */
	void setEdgeSensitive(bool edge);
	/** Returns true if the connector is edge sensitive. False, if the connector is level sensitive.
	 */
	bool isEdgeSensitive() const;

	/** True sets the init sensitive to edge. False sets the init sensitive to level.
	 */
	void setInitEdgeSensitive(bool edge);
	/** Returns true if the init sensitive is edge sensitive. False, if the init sensitive is level sensitive.
	 */
	bool isInitEdgeSensitive() const;


	/** True enables the user to change the sensitive type. */
	void setEdgeSensitiveChangeEnable(bool enable);
	/** Returns true if the user is able to change the sensitive type. */
	bool isEdgeSensitiveChangeEnable() const;
	
	
	/** Returns the input data */
	bool getInput();
  /** Resets the connector.
     */
	virtual void reset();

	/** Creates the property widget */
	virtual QWidget* propertyWidget(QWidget * parent);

	/** Add menu items depend on connetor properties */
	virtual bool initPopupMenu(QPopupMenu * popup);

	/** Load properties.
		Returns true if successful. */
	virtual bool load(KSimData & file);
	/** Save properties. */
	virtual void save(KSimData & file) const;

private slots:
	/** Display a status help message for popup menu entries, if highlighted */
	virtual void popupMenuHighlighted(int msg) const;
	/** Toggles the sensitive type */
	void slotToggleEdgeSensitive();



private:
	bool m_edgeSensitive;
	bool m_last;
	unsigned int m_flags;
	static int s_idSensitive;	
};

#endif
