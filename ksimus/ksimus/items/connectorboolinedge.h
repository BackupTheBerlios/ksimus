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

extern const ConnectorInfo ConnectorBoolInEdgeInfo;


class ConnectorBoolInEdge : public ConnectorBoolIn
{
	
	Q_OBJECT

public:
	/**
	 * Constructs a edge sensitive boolean input connector.
	 *
	 * @param comp Component which contains this connector.
	 * @param name The name of the connector. This name is shown at the property view
	 *             or the status bar.
	 * @param pos  Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolInEdge(Component * comp, const char * name, const QPoint & pos = QPoint());

	/**
	 * Constructs a edge sensitive boolean input connector. Like the constructor above, but creates also a
	 * connector label (@see ConnectorLabel).
	 *
	 * @param comp  Component which contains this connector.
	 * @param name  The name of the connector. This name is shown at the property view
	 *              or the status bar.
	 * @param descr Sets the description of the connector label.
	 * @param pos   Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolInEdge(Component * comp,
	                    const char * name,
	                    const QString & descr,
	                    const QPoint & pos = QPoint()	);
	
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
	
	
	/** Returns the input data.
	  *
	  * WARNING: Call this function only one time during one simulation cycle!  */
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

protected:
	/**
	 * Constructs a boolean input connector. Use this constructor if you derive this class.
	 *
	 * @param comp Component which contains this connector.
	 * @param name The name of the connector. This name is shown at the property view
	 *             or the status bar.
	 * @param pos  Sets the position of the connctor. The position has to be given in grids.
	 * @param orient Sets the orientation of the connector.
	 * @param ci     Sets the connector info (@see ConnectorInfo):
	 */
	ConnectorBoolInEdge(Component * comp,
	                    const char * name,
	                    const QPoint & pos,
	                    ConnOrientationType orient,
	                    const ConnectorInfo * ci);


private slots:
	/** Display a status help message for popup menu entries, if highlighted */
	virtual void popupMenuHighlighted(int msg) const;
	/** Toggles the sensitive type */
	void slotToggleEdgeSensitive();



private:
	bool m_edgeSensitive;
	bool m_last;
	Q_UINT32 m_flags;
	static int s_idSensitive;	
};

#endif
