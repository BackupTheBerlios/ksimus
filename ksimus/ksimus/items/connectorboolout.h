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

const ConnectorInfo * getConnectorBoolOutInfo();

class ConnectorBoolOut : public ConnectorOutputBase
{
	Q_OBJECT

friend class WirePropertyBoolean;

public:
	/**
	 * Constructs a boolean output connector.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolOut(Component * comp,
	                 const QString & name,
	                 const QString & i18nName,
	                 const QPoint & pos = QPoint());
	
	/**
	 * Constructs a boolean output connector. Like the constructor above, but creates also a
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
	ConnectorBoolOut(Component * comp,
	                 const QString & name,
	                 const QString & i18nName,
	                 const QString & descr,
	                 const QPoint & pos = QPoint());
	
	
	
/** Set the current output.
	* @param out                      The new output value.
	* @param executeWirePropertyNext  If set the wire property is execute next. The parameter is used
	*                                 with zero delay components.
	*/
	void setOutput(bool out, bool executeWirePropertyNext = true);
/** Return the current output */
	bool getOutput() const;
	
	// Get the colors for the connector
	virtual const WireColorScheme & getColorScheme() const;
	

	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Creates the property widget */
	virtual PropertyWidget* propertyWidget(QWidget * parent);

	/** Returns a text which represents the current value. */
	virtual QString getValueText() const;

	/** Resets the connector.
	  */
	virtual void reset();
	
	/** The function copyData() has to copy data to the output variable
	  * The default implementation doest nothing
	  * Reimplementations is required for all output connectors  */
	virtual void copyData(const void * pData);
	
	/** Returns a pointer to the data that's read from the component. */
	virtual const void * readoutData() const;
	
	/** Returns a @ref WatchItemBooleanConnector object. */
	virtual WatchItemBase * makeWatchItem();

protected:
	/**
	 * Constructs a boolean output connector. Use this constructor if you derive this class.
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
	ConnectorBoolOut(Component * comp,
	                 const QString & name,
	                 const QString & i18nName,
	                 const QPoint & pos,
	                 ConnOrientationType orient,
	                 const ConnectorInfo * ci);
	

private slots:
	/** Display a status help message for popup menu entries, if highlighted */
	virtual void popupMenuHighlighted(int msg) const;
	/** Toggles the neg Type */
	void slotToggleNegType();
	
	
private:
	/** Internal init function. */
	void init();
	
	int idNegate;
	bool m_data;
};

#endif
