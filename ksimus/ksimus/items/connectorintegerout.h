/***************************************************************************
                          connectorintegerout.h  -  description
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

#ifndef CONNECTORINTEGEROUT_H
#define CONNECTORINTEGEROUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorbase.h"

// Forward declaration


/**Output connector for integer values.
  *@author Rasmus Diekenbrock
  */

class ConnectorIntegerOut : public ConnectorOutputBase
{
	Q_OBJECT

friend class WirePropertyInteger;

public:
	/**
	 * Constructs a integer output connector.
	 *
	 * @param comp      Component which contains this connector.
	 * @param name      The *untranslated* connector name. This name is used as internal
	 *                  identifier and should not be translated.
	 * @param i18nName  The *translated* connector name. This name is used in the status bar and
	 *                  the property widget.
	 * @param pos       Sets the position of the connctor. The position has to be given in grids.
	 */
	ConnectorIntegerOut(Component * comp,
	                    const QString & name,
	                    const QString & i18nName,
	                    const QPoint & pos = QPoint()	);
	
	/**
	 * Constructs a integer output connector. Like the constructor above, but creates also a
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
	ConnectorIntegerOut(Component * comp,
	                    const QString & name,
	                    const QString & i18nName,
	                    const QString & descr,
	                    const QPoint & pos = QPoint()	);
	
	
/** Set the current output.
	* @param out                      The new output value.
	* @param executeWirePropertyNext  If set the wire property is execute next. The parameter is used
	*                                 with zero delay components.
	*/
	void setOutput(int out, bool executeWirePropertyNext = true);
/** Return the current output */
	int getOutput() const;
	
	// Get the colors for the connector
	virtual const WireColorScheme & getColorScheme() const;
	
	/** Resets the connector.
	  */
	virtual void reset();
	
	/** Returns a text which represents the current value. */
	virtual QString getValueText() const;

	/** Returns a @ref WatchItemIntegerConnector object. */
	virtual WatchItemBase * makeWatchItem();

protected:
	/**
	 * Constructs a integer output connector. Use this constructor if you derive this class.
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
	ConnectorIntegerOut(Component * comp,
	                    const QString & name,
	                    const QString & i18nName,
	                    const QPoint & pos,
	                    ConnOrientationType orient,
	                    const ConnectorInfo * ci);
	
	/** The function copyData() has to copy data to the output variable
	  * The default implementation doest nothing
	  * Reimplementations is required for all output connectors  */
	virtual void copyData(const void * pData);
	
	/** Returns a pointer to the data that's read from the component. */
	virtual const void * readoutData() const;
	/** Returns a pointer to the data of this output connector */
	virtual const void * getData() const;
	
	int m_data;

	
	
private:
	/** Internal init function. */
	void init();
	

// Some statics:
public:
	static const ConnectorInfo * getStaticInfo();
protected:
	static ConnectorBase * create(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos);

};



#endif
