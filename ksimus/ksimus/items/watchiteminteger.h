/***************************************************************************
                          watchiteminteger.h  -  description
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

#ifndef WATCHITEMINTEGER_H
#define WATCHITEMINTEGER_H

// C-Includes

// QT-Includes
#include <qmemarray.h>

// KDE-Includes

// Project-Includes
#include "watchitembase.h"
#include "ksimbaseint.h"
#include "ksimbaseintedit.h"
#include "ksimbaseintbaseselection.h"

// Forward declaration
class QComboBox;


/**Base watch item for integer data types.
  *@author Rasmus Diekenbrock
  */

class WatchItemIntegerBase : public WatchItemBase
{
	Q_OBJECT

	enum eTriggerType { eRising, eFalling, eNaN, ePosInf, eNegInf };


public: 
	~WatchItemIntegerBase();
	
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

	/** This function is called if the user press the cancel button.
	  * It calls the @ref triggerPropertyCancelPressed function. */
	virtual void propertyCancelPressed();

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
	WatchItemIntegerBase(ConnectorBase * connector);
	WatchItemIntegerBase(WireProperty * wireProperty);
	
	QMemArray<int> m_traceBuffer;

private:
	void init();
	QComboBox * m_triggerBox;
	eTriggerType m_triggerType;
	KSimBaseInt m_limit;
	KSimBaseIntEdit * m_limitEdit;
	KSimBaseInt::eBaseType m_viewBase;
	KSimBaseIntBaseSelection * m_baseSelector;

private slots:
	void slotTriggerBoxChanged();

};


//############################################################################################
//############################################################################################

/**Watch item for integer connectors.
  *@author Rasmus Diekenbrock
  */

class WatchItemIntegerConnector : public WatchItemIntegerBase
{
public:
	WatchItemIntegerConnector(ConnectorBase * connector);
	~WatchItemIntegerConnector();
	
	/** Reads the current state of the wire property or the connector.
	  * This function has to be reimplementated in a sub class.
	  * @param index The index of the history buffer where the data has to be stored.
	  */
	virtual void readData(unsigned int index);
};

//############################################################################################
//############################################################################################

/**Watch item for integer wire property.
  *@author Rasmus Diekenbrock
  */

class WatchItemIntegerWireProperty : public WatchItemIntegerBase
{
public:
	WatchItemIntegerWireProperty(WireProperty * wireProperty);
	~WatchItemIntegerWireProperty();
	
	/** Reads the current state of the wire property or the connector.
	  * This function has to be reimplementated in a sub class.
	  * @param index The index of the history buffer where the data has to be stored.
	  */
	virtual void readData(unsigned int index);
};

//############################################################################################
//############################################################################################


#endif
