/***************************************************************************
                          extconnfloatin.h  -  description
                             -------------------
    begin                : Sun Feb 10 2002
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

#ifndef EXTCONNFLOATIN_H
#define EXTCONNFLOATIN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/externalconnector.h"

// Forward declaration
class KSimDoubleEdit;
class QLabel;

namespace KSimLibFloatingPoint
{

const ComponentInfo * getExtConnFloatInInfo();

/**A external floating point input connector
  *@author Rasmus Diekenbrock
  */

class ExtConnFloatIn : public ExternalConnector
{
	Q_OBJECT

public: 
	ExtConnFloatIn(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnFloatIn();

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

	/* Sets the default value. */
	void setDefaultValue(double value) { m_defaultValue = value; };
	/** Returns the default value. */
	double getDefaultValue() const { return m_defaultValue; };

protected:
	double m_defaultValue;
};


//###############################################################

class ExtConnFloatInPropertyGeneralWidget : public ExternalConnectorPropertyGeneralWidget
{
	Q_OBJECT

public:
	ExtConnFloatInPropertyGeneralWidget(ExtConnFloatIn * comp, QWidget *parent=0, const char *name=0);
//	~ExtConnFloatInPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ExtConnFloatIn * getExtConnIn() { return (ExtConnFloatIn *)getComponent();	};

private:
	KSimDoubleEdit * m_defaultValue;
	QLabel * m_defaultValueLabel;
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
