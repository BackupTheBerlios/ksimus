/***************************************************************************
                          optionalconnector.h  -  description
                             -------------------
    begin                : Sun Jun 2 2002
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

#ifndef OPTIONALCONNECTOR_H
#define OPTIONALCONNECTOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "optionalfunction.h"

// Forward declaration
class ConnectorBase;
class ConnectorList;
class PropertyWidget;

/**Manages one or more optional connectors.
  *@author Rasmus Diekenbrock
  */

class OptionalConnector : public OptionalFunction
{
class Private;
class PropertyEntry;

   Q_OBJECT
public:
	OptionalConnector(Component * component, const QString & addOnName);
	OptionalConnector(ConnectorBase * conn, const QString & addOnName);
	OptionalConnector(ConnectorBase * conn, const QString & addOnName, const QString & i18nLabelText);
	OptionalConnector(bool enabled, ConnectorBase * conn, const QString & addOnName, const QString & i18nLabelText);
	~OptionalConnector();
	
	void addConnector(ConnectorBase * conn);
	
	/** Enables the optional connector. This means the connector will be shown.
	  * If ena is false and one or more connectors are connected the function
	  * call will be failed. */
	virtual void setEnabled(bool ena);
	
	/** Returns the number of connectors. */
	unsigned int count() const;

	
	/** Returns the number of connected connectors. */
	unsigned int countConnected() const;
	
	/** Returns true if one or more optional connectors managed by this container is
	  * connected. */
	bool isConnected() const;
	
	/** Returns the connector list. */
	ConnectorList * getConnList();
	/** Returns the connector list. */
	ConnectorList * getConnList() const;
	
	/** Adds an entry to a property widget.
	  * @param widget     The @ref PropertyWidget to which the entry is added.
	  * */
	virtual void addToPropertyWidget(PropertyWidget * widget);
	
protected:
	/** Returns the text getLabel() if it is not empty, or creates a more or less useful label
	  * (works only with one connector).
	 */
	QString getLabelInternal();

	/** Creates a generic tool tip. */
	QString makeToolTip() const;
	
	/** Creates some useful hints. Add it to the tool tip or whats this. */
	QString makeHints() const;
	
	/** Returns the tool tip. Uses getToolTip() and appends some usefull hints.
			If getToolTip() is empty the function creates a more or less useful tool tip.
	 */
	QString getToolTipInternal();

	/** Returns the what's this. Uses getWhatsThis() and appends some usefull hints.
			If getWhatsThis() is empty the function creates a more or less useful what's this.
			The current implementation returns getToolTipInternal().
	 */
	QString getWhatsThisInternal();

private slots:
	void slotConnProperty();
	void slotConnDestroyed();

private:
	void init();
	Private * m_p;
};

#endif
