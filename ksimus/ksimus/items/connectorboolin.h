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

const ConnectorInfo * getConnectorBoolInInfo();


class ConnectorBoolIn : public ConnectorInputBase
{
	
	Q_OBJECT

public:
	/**
	 * Constructs a boolean input connector.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos        Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolIn(Component * comp,
	                const QString & name,
	                const QString & i18nName,
	                const QPoint & pos = QPoint());
	
	/**
	 * Constructs a boolean input connector. Like the constructor above, but creates also a
	 * connector label (@ref ConnectorLabel).
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param descr     Sets the description of the connector label.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolIn(Component * comp,
	                const QString & name,
	                const QString & i18nName,
	                const QString & descr,
	                const QPoint & pos = QPoint());
	
	
	// Get the colors for the connector
	virtual const WireColorScheme & getColorScheme() const;

	/** Resets the connector.
	  */
	virtual void reset();
	
	/** Returns a pointer to the data that's read from the component. */
	virtual const void * readoutData() const;
	
	/** Copies the data where the p points to into a local variable.
	  * The function must be implemented by a sub class. */
	virtual void copyData(const void * p);

	/** Returns the input data */
	bool getInput() const;
	
	/** Creates the property widget */
	virtual PropertyWidget* propertyWidget(QWidget * parent);

	/** Add menu items depend on connetor properties */
	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Returns a text which represents the current value. */
	virtual QString getValueText() const;

	/** Returns a @ref WatchItemBooleanConnector object. */
	virtual WatchItemBase * makeWatchItem();

protected:
	/**
	 * Constructs a boolean input connector. Use this constructor if you derive this class.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 * @param orient    Sets the orientation of the connector.
	 * @param ci        Sets the connector info (@ref ConnectorInfo):
	 */
	ConnectorBoolIn(Component * comp,
	                const QString & name,
	                const QString & i18nName,
	                const QPoint & pos,
	                ConnOrientationType orient,
	                const ConnectorInfo * ci);
	
	
	
protected slots:
	/** Display a status help message for popup menu entries, if highlighted */
	virtual void popupMenuHighlighted(int msg) const;
private slots:
	/** Toggles the neg Type */
	void slotToggleNegType();
	
	
private:
	/** Internal init function. */
	void init();

	bool m_data;
	int idNegate;
};


#endif
