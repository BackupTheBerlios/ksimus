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

extern const ConnectorInfo ConnectorBoolOutInfo;

class ConnectorBoolOut : public ConnectorOutputBase
{
	Q_OBJECT

friend class WirePropertyBoolean;

public:	
	enum eResetMode { ResetDefault = 0, ResetTrue, ResetFalse };
	
	ConnectorBoolOut(	Component * comp,
						const char * name,
						const QPoint & pos	);
	
	
    /** Resets the connector
     */
	virtual void reset();
	
	void setOutput(bool out);
						
	// Setup the colors, brushs, and fills for the connector
	virtual void setupColorScheme (QPainter * p) const;
	
	/** Load properties
		Returns true if successful */
	virtual bool load(KSimpleConfig & file);
	/** Save properties */
	virtual void save(KSimpleConfig & file) const;

	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Creates the property widget */
	virtual QWidget* propertyWidget(QWidget * parent);

	// ### Reset mode functions
	/** Set the reset mode.
		* The reset mode won't be saved in document, if init is true.
	  * The signal signalProperty() is emitted.
	  */
	void setResetMode(eResetMode mode, bool init = false);
	/** Returns the reset mode */
	eResetMode getResetMode() const;
	/** Returns the reset mode at init time */
	eResetMode getInitResetMode() const;
		



protected:
	/** The function copyData() has to copy data to the output variable
	  * The default implementation doest nothing
	  * Reimplementations is required for all output connectors  */
	virtual void copyData(const void * pData);
    /** Returns a pointer to the data of this output connector */
	virtual const void * getData() const;
	
	bool m_data;
	eResetMode m_resetMode;
	eResetMode m_resetModeInit;

private slots:
	/** Display a status help message for popup menu entries, if highlighted */
	virtual void popupMenuHighlighted(int msg) const;
	/** Toggles the neg Type */
	void slotToggleNegType();
	
	
private:
	int idNegate;
};

#endif
