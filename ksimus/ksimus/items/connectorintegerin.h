/***************************************************************************
                          connectorintegerin.h  -  description
                             -------------------
    begin                : Sat Jul 12 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef CONNECTORINTEGERIN_H
#define CONNECTORINTEGERIN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorbase.h"


// Forward declaration

// Project-Includes


/**Input connector for integer values
  *@author Rasmus Diekenbrock
  */

class ConnectorIntegerIn : public ConnectorInputBase
{
	
	Q_OBJECT

public:
	/**
	 * Constructs a integer input connector.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorIntegerIn(Component * comp,
	                   const QString & name,
	                   const QString & i18nName,
	                   const QPoint & pos = QPoint()	);
	
	/**
	 * Constructs a integer input connector. Like the constructor above, but creates also a
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
	ConnectorIntegerIn(Component * comp,
	                   const QString & name,
	                   const QString & i18nName,
	                   const QString & descr,
	                   const QPoint & pos = QPoint()	);
	
	
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
	int getInput() const;
	
	/** Creates the property widget */
//	virtual QWidget* propertyWidget(QWidget * parent);

	/** Add menu items depend on connetor properties */
//	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Returns a text which represents the current value. */
	virtual QString getValueText() const;

	/** Returns a @ref WatchItemBooleanConnector object. */
	virtual WatchItemBase * makeWatchItem();

protected:
	/**
	 * Constructs a integer input connector. Use this constructor if you derive this class.
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
	ConnectorIntegerIn(Component * comp,
	                   const QString & name,
	                   const QString & i18nName,
	                   const QPoint & pos,
	                   ConnOrientationType orient,
	                   const ConnectorInfo * ci);
	
	
	
protected slots:
	/** Display a status help message for popup menu entries, if highlighted */
//	virtual void popupMenuHighlighted(int msg) const;
private slots:
	/** Toggles the neg Type */
//	void slotToggleNegType();
	
	
private:
	/** Internal init function. */
	void init();
	
	int m_data;


// Some statics:
public:
	static const ConnectorInfo * getStaticInfo();
protected:
	static ConnectorBase * create(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos);


};

#endif
