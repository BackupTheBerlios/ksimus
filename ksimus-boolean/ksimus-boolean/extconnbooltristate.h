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

class ExtConnBoolTriStateBase : public ExternalConnector
{
	Q_OBJECT
	
public: 
	ExtConnBoolTriStateBase(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTriStateBase();

	/** Resets the component state. */
	virtual void reset();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Sets the reset state. The component gets this state each call @ref reset. */
	void setResetState(KSimBoolTriState resetState);

	/** Returns the reset state. The component gets this state each call @ref reset. */
	KSimBoolTriState getResetState() const;

	/** Creates the general property page for the property dialog.
	  * This function creates a @ref ExtConnBoolTriStateInPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Returns true, if the connector is connected to a ConnectorBoolTriState. */
	static bool isConnectedWithBoolTriState(ConnectorBase * conn);

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	/** Creates an external connector.
	  * @param input       True, if the external connector is an input (on the left side of the module)
	  * @param multiOutput True, if the data type of the external connector supports multiple outputs
	  */
	ExtConnBoolTriStateBase(CompContainer * container, const ComponentInfo * ci, bool input, bool multiOutput = false);

private:
	KSimBoolTriState m_resetState;
};



//###############################################################

class ExtConnBoolTriStateBasePropertyGeneralWidget : public ExternalConnectorPropertyGeneralWidget
{
	Q_OBJECT

public:
	ExtConnBoolTriStateBasePropertyGeneralWidget(ExtConnBoolTriStateBase * comp, QWidget *parent=0, const char *name=0);
//	~ExtConnBoolTriStateBasePropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ExtConnBoolTriStateBase * getExtConn() { return (ExtConnBoolTriStateBase *)getComponent(); };

private:
	KSimBoolTristateBox * m_defaultState;
};

//###############################################################

/**A external boolean tristate input connector
  *@author Rasmus Diekenbrock
  */

class ExtConnBoolTriStateIn : public ExtConnBoolTriStateBase
{
	Q_OBJECT

public:
	ExtConnBoolTriStateIn(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTriStateIn();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A external boolean tristate output connector
  *@author Rasmus Diekenbrock
  */

class ExtConnBoolTriStateOut : public ExtConnBoolTriStateBase
{
	Q_OBJECT

public:
	ExtConnBoolTriStateOut(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTriStateOut();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
