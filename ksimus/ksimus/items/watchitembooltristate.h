/***************************************************************************
                          watchitembooltristate.h  -  description
                             -------------------
    begin                : Thu Aug 15 2002
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

#ifndef WATCHITEMBOOLTRISTATE_H
#define WATCHITEMBOOLTRISTATE_H

// C-Includes

// QT-Includes
#include <qmemarray.h>

// KDE-Includes

// Project-Includes
#include "watchitembase.h"
#include "wirepropertybooltristate.h"

// Forward declaration
class QComboBox;
class QCheckBox;
class ConnectorBoolTristateSpecial;


/**Base watch item for boolean data types.
  *@author Rasmus Diekenbrock
  */

class WatchItemBoolTristateBase : public WatchItemBase
{
	Q_OBJECT

class StatePropertyWidget;
class ActivePropertyWidget;
enum eStateProperty { eInactive = 0, eFalse, eTrue, eInactiveOrFalse, eInactiveOrTrue, eFalseOrTrue, eDontCare };
enum eActiveProperty { eLesser = 0, eEqual, eLarger, eDontCareCount };


class ActiveProperty
{
public:
	ActiveProperty(eActiveProperty activeProperty = eDontCareCount, unsigned int activeCount = 0)
	:	m_activeProperty(activeProperty),
		m_activeCount(activeCount)
	{};

	void setProperty(eActiveProperty activeProperty)
	{
		m_activeProperty = activeProperty;
	};
	
	eActiveProperty getProperty() const
	{
		return m_activeProperty;
	};

	void setCount(unsigned int activeCount)
	{
		m_activeCount = activeCount;
	};

	unsigned int getCount() const
	{
		return m_activeCount;
	};

private:
	eActiveProperty m_activeProperty;
	unsigned int m_activeCount;
};


public:
	~WatchItemBoolTristateBase();
	
	/** Returns a value from the history buffer as a text.
	  * This function has to be reimplementated in a sub class.
	  * @param index The index of the history buffer where the data has to be fetched from.
	  */
	virtual QString getDataText(unsigned int index);
	
	/** If the data from the history buffer results in a break situation.
	  * Returns true, if a break situation is present. This will not pause the simulation immediatly because there
	  * is a counter, which counts these events.
	  * This function has to be reimplementated in a sub class.
	  * @param index     The index of the history buffer where the data has to be fetched from.
	  * @param stepCount The number of steps inside the history buffer (never larger than the history buffer).
	  */
	virtual bool testBreak(unsigned int index, unsigned int stepCount);

	/** Resets the item. */
	virtual void reset();

	/** Sets the size of the trace buffer.
	  * The function has to be reimplementated in a sub class. It has to resize the buffer. */
	virtual void setTraceSize(unsigned int size);

	/** Creates the property widget.
	  * This widget is the base of the trigger dialog. */
	virtual PropertyWidget * createPropertyWidget(QWidget * parent);

	/** This function is called if the user press the ok button.
	  * It calls the @ref triggerPropertyOkPressed function
	  * and stores the changed data created by @ref createPropertyWidget. */
	virtual void propertyOkPressed();

	/** This function is called if the user press the default button.
	  * It calls the @ref triggerPropertyDefaultPressed function
	  * and ste the data created by @ref createPropertyWidget to defualt vales. */
	virtual void propertyDefaultPressed();

	/** Creates the trigger property widget.
	  * This widget will be inserted into the trigger dialog.
	  * Inside this widget the user defines the type of the trigger event,
	  * e.g. a high to low edge.
	  * Don't forget to initialize the widget.
	  * The function must be reimplemented in a sub class. */
	virtual void createTriggerPropertyWidget(QWidget * parent);
	/** This function is called if the user press the ok button.
	  * This is the right time to store the changed trigger property.
	  * In most case you have to reimplement this function.
	  * The default implementation does nothing. */
	virtual void triggerPropertyOkPressed();

	/** This function is called if the user press the default button.
	  * Set the trigger property to default.
	  * In most case you have to reimplement this function.
	  * The default implementation does nothing. */
	virtual void triggerPropertyDefaultPressed();

	/** This function is called if the user press the cancel button.
	  * Set the trigger property to default.
	  * In most case there is no need to reimplement this function.
	  * The default implementation does nothing. */
	virtual void triggerPropertyCancelPressed();


protected:
	WatchItemBoolTristateBase(ConnectorBase * connector, bool inOnly);
	WatchItemBoolTristateBase(WireProperty * wireProperty, bool inOnly);
	
	QMemArray<WireStateBoolTristate> m_traceInBuffer;
	QMemArray<WireStateBoolTristate> m_traceOutBuffer;

	struct
	{
		unsigned int inOnly :1;
		unsigned int detailed :1;
	} m_flags;


private:
	void init(bool inOnly);
	unsigned int getMaxConnectors() const;
	static bool testActive(ActiveProperty & property, const WireStateBoolTristate & state);
	static bool testState(eStateProperty property, const WireStateBoolTristate & state);

	eStateProperty         m_triggerOutStateT1;
	eStateProperty         m_triggerOutStateT0;
	StatePropertyWidget  * m_triggerOutStateT1Widget;
	StatePropertyWidget  * m_triggerOutStateT0Widget;

	eStateProperty         m_triggerInStateT1;
	ActiveProperty         m_triggerInActiveT1;
	eStateProperty         m_triggerInStateT0;
	ActiveProperty         m_triggerInActiveT0;
	StatePropertyWidget  * m_triggerInStateT1Widget;
	ActivePropertyWidget * m_triggerInActiveT1Widget;
	StatePropertyWidget  * m_triggerInStateT0Widget;
	ActivePropertyWidget * m_triggerInActiveT0Widget;

	QCheckBox * m_detailed;
};


//############################################################################################
//############################################################################################

/**Watch item for boolean tri state connectors.
  *@author Rasmus Diekenbrock
  */

class WatchItemBoolTristateConnector : public WatchItemBoolTristateBase
{
	Q_OBJECT

public:
	WatchItemBoolTristateConnector(ConnectorBase * connector);
	~WatchItemBoolTristateConnector();

	/** Reads the current state of the wire property or the connector.
	  * This function has to be reimplementated in a sub class.
	  * @param index The index of the history buffer where the data has to be stored.
	  */
	virtual void readData(unsigned int index);
};

//############################################################################################
//############################################################################################

/**Watch item for boolean tri state wire property.
  *@author Rasmus Diekenbrock
  */

class WatchItemBoolTristateWireProperty : public WatchItemBoolTristateBase
{
	Q_OBJECT

public:
	WatchItemBoolTristateWireProperty(WireProperty * wireProperty);
	~WatchItemBoolTristateWireProperty();

	/** Reads the current state of the wire property or the connector.
	  * This function has to be reimplementated in a sub class.
	  * @param index The index of the history buffer where the data has to be stored.
	  */
	virtual void readData(unsigned int index);
};

//############################################################################################
//############################################################################################

/**Watch item for boolean tri state connectors used in External Connector.
  *@author Rasmus Diekenbrock
  */

class WatchItemBoolTristateConnectorSpecial : public WatchItemBoolTristateBase
{
	Q_OBJECT

public:
	WatchItemBoolTristateConnectorSpecial(ConnectorBoolTristateSpecial * connector);
	~WatchItemBoolTristateConnectorSpecial();

	/** Reads the current state of the wire property or the connector.
	  * This function has to be reimplementated in a sub class.
	  * @param index The index of the history buffer where the data has to be stored.
	  */
	virtual void readData(unsigned int index);
};

//############################################################################################
//############################################################################################


#endif
