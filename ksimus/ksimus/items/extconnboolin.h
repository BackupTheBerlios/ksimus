/***************************************************************************
                          extconnboolin.h  -  description
                             -------------------
    begin                : Sat Jul 8 2000
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

#ifndef EXTCONNBOOLIN_H
#define EXTCONNBOOLIN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "externalconnector.h"

// Forward declaration
class KSimBooleanBox;


/**A external boolean input connector
  *@author Rasmus Diekenbrock
  */

const ComponentInfo * getExtConnBoolInInfo();

class ExtConnBoolIn : public ExternalConnector
{
	Q_OBJECT

public: 
	ExtConnBoolIn(CompContainer * container, const ComponentInfo * ci);
	~ExtConnBoolIn();
	
	/** Reset all simulation variables */
	virtual void reset();

	/** Executes the simulation of this component */
	virtual void calculate();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Creates the general property page for the property dialog.
	  * This function creates a @ref BooleanButtonPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/* Sets the default state. */
	void setDefaultState(bool state) { m_defaultState = state; };
	/** Returns the default state. */
	bool isDefaultState() const { return m_defaultState; };

protected:
	bool m_defaultState;
};

//###############################################################

class ExtConnBoolInPropertyGeneralWidget : public ExternalConnectorPropertyGeneralWidget
{
	Q_OBJECT

public:
	ExtConnBoolInPropertyGeneralWidget(ExtConnBoolIn * comp, QWidget *parent=0, const char *name=0);
//	~ExtConnBoolInPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ExtConnBoolIn * getExtConnIn() { return (ExtConnBoolIn *)getComponent();	};

private:
	KSimBooleanBox * m_defaultState;
};

#endif
