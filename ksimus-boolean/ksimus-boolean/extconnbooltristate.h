/***************************************************************************
                          extconnbooltrisate.h  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

#ifndef EXTCONNBOOLTRISTATE_H
#define EXTCONNBOOLTRISTATE_H

#include "ksimus/externalconnector.h"
#include "ksimus/ksimbooltristate.h"


class KSimBoolTristateBox;

namespace KSimLibBoolean
{

/**Base class of external boolean tristate connector
  *@author Rasmus Diekenbrock
  */

class ExtConnBoolTristateBase : public ExternalConnector
{
	Q_OBJECT
	
public: 
	ExtConnBoolTristateBase(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTristateBase();

	/** Resets the component state. */
	virtual void reset();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Sets the reset state. The component gets this state each call @ref reset. */
	void setResetState(KSimBoolTristate resetState);

	/** Returns the reset state. The component gets this state each call @ref reset. */
	KSimBoolTristate getResetState() const;

	/** Creates the general property page for the property dialog.
	  * This function creates a @ref ExtConnBoolTristateInPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Returns true, if the connector is connected to a ConnectorBoolTristate. */
	static bool isConnectedWithBoolTristate(ConnectorBase * conn);

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	/** Creates an external connector.
	  * @param input       True, if the external connector is an input (on the left side of the module)
	  * @param multiOutput True, if the data type of the external connector supports multiple outputs
	  */
	ExtConnBoolTristateBase(CompContainer * container, const ComponentInfo * ci, bool input, bool multiOutput = false);

private:
	KSimBoolTristate m_resetState;
};



//###############################################################

class ExtConnBoolTristateBasePropertyGeneralWidget : public ExternalConnectorPropertyGeneralWidget
{
	Q_OBJECT

public:
	ExtConnBoolTristateBasePropertyGeneralWidget(ExtConnBoolTristateBase * comp, QWidget *parent=0, const char *name=0);
//	~ExtConnBoolTristateBasePropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ExtConnBoolTristateBase * getExtConn() { return (ExtConnBoolTristateBase *)getComponent(); };

private:
	KSimBoolTristateBox * m_defaultState;
};

//###############################################################

/**A external boolean tristate input connector
  *@author Rasmus Diekenbrock
  */

class ExtConnBoolTristateIn : public ExtConnBoolTristateBase
{
	Q_OBJECT

public:
	ExtConnBoolTristateIn(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTristateIn();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A external boolean tristate output connector
  *@author Rasmus Diekenbrock
  */

class ExtConnBoolTristateOut : public ExtConnBoolTristateBase
{
	Q_OBJECT

public:
	ExtConnBoolTristateOut(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTristateOut();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
