/***************************************************************************
                          connectorbooltristate.h  -  description
                             -------------------
    begin                : Sat Jun 29 2002
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

#ifndef CONNECTORBOOLTRISTATE_H
#define CONNECTORBOOLTRISTATE_H

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus Includes
#include "connectorbase.h"
#include "ksimbooltristate.h"
#include "wirepropertybooltristate.h"


// Forward declaration

/**A boolean tristate connector.
  *@author Rasmus Diekenbrock
  */

const ConnectorInfo * getConnectorBoolTristateInfo();

class ConnectorBoolTristate : public ConnectorTristateBase
{
	Q_OBJECT

public:
	/**
	 * Constructs a boolean tristate connector.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolTristate(Component * comp,
	                      const QString & name,
	                      const QString & i18nName,
	                      const QPoint & pos = QPoint());
	
	/**
	 * Constructs a boolean tristate connector. Like the constructor above, but creates also a
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
	ConnectorBoolTristate(Component * comp,
	                      const QString & name,
	                      const QString & i18nName,
	                      const QString & descr,
	                      const QPoint & pos = QPoint());
	
	
	
/** Set the current output.
	* @param out                      The new output value.
	* @param active                   Ture, if out is active.
	* @param executeWirePropertyNext  If set the wire property is execute next. The parameter is used
	*                                 with zero delay components.
	*/
	void setOutput(bool out, bool active, bool executeWirePropertyNext = true);
/** Set the current output.
	* @param state                    The new output state.
	* @param executeWirePropertyNext  If set the wire property is execute next. The parameter is used
	*                                 with zero delay components.
	*/
	void setOutput(KSimBoolTristate state, bool executeWirePropertyNext = true);
/** Return the current output */
	KSimBoolTristate getOutput() const;
/** Return the current wire state */
	KSimBoolTristate getInput() const;
	
	// Get the colors for the connector
	virtual const WireColorScheme & getColorScheme() const;
	
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
	
	/** Returns a pointer to the data that's read from the wire. */
	const void * readoutInData() const;
	
	/** Returns a @ref WatchItemBooleanConnector object. */
	virtual WatchItemBase * makeWatchItem();

protected:
	/**
	 * Constructs a boolean tristate connector. Use this constructor if you derive this class.
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
	ConnectorBoolTristate(Component * comp,
	                      const QString & name,
	                      const QString & i18nName,
	                      const QPoint & pos,
	                      ConnOrientationType orient,
	                      const ConnectorInfo * ci);
	

private:
	/** Internal init function. */
	void init();
	
	WireStateBoolTristate m_inData;
	WireStateBoolTristate m_outData;
};


//#################################################################
//#################################################################



/**A boolean tristate connector for special cases as External Connectors.
  *@author Rasmus Diekenbrock
  *
  * Used in External Connector
  */

const ConnectorInfo * getConnectorBoolTristateSpecialInfo();

class ConnectorBoolTristateSpecial : public ConnectorBoolTristate
{
	Q_OBJECT

public:
	/**
	 * Constructs a boolean tristate connector.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorBoolTristateSpecial(Component * comp,
	                             const QString & name,
	                             const QString & i18nName,
	                             const QPoint & pos = QPoint());

	/**
	 * Constructs a boolean tristate connector. Like the constructor above, but creates also a
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
	ConnectorBoolTristateSpecial(Component * comp,
	                             const QString & name,
	                             const QString & i18nName,
	                             const QString & descr,
	                             const QPoint & pos = QPoint());



	/** Returns a @ref WatchItemBooleanConnector object. */
	virtual WatchItemBase * makeWatchItem();

protected:
	/**
	 * Constructs a boolean tristate connector. Use this constructor if you derive this class.
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
	ConnectorBoolTristateSpecial(Component * comp,
	                             const QString & name,
	                             const QString & i18nName,
	                             const QPoint & pos,
	                             ConnOrientationType orient,
	                             const ConnectorInfo * ci);

private:
	/** Internal init function. */
	void init();

};

#endif
