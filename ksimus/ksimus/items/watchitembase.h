/***************************************************************************
                          watchitembase.h  -  description
                             -------------------
    begin                : Sun Jul 14 2002
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

#ifndef WATCHITEMBASE_H
#define WATCHITEMBASE_H

// C-Includes

// QT-Includes
#include <qobject.h>
#include <qbitarray.h>
#include <qarray.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class ConnectorBase;
class WireProperty;
class WatchViewItem;
class WatchWidget;
class QSpinBox;
class QCheckBox;
class PropertyWidget;

/**Base class for all watch items.
  *@author Rasmus Diekenbrock
  */

class WatchItemBase : public QObject
{
friend class WatchWidget;


	Q_OBJECT

public:
	virtual ~WatchItemBase();
	
	/** Return the pointer to the connector, or a null pointer if this watch is owned by
	  * a wire property.
	  */
	ConnectorBase * getConnector() { return m_connector; };
	const ConnectorBase * getConnector() const { return m_connector; };
	/** Return the pointer to the wire property, or a null pointer if this watch is owned by
	  * a connector.
	  */
	WireProperty * getWireProperty() { return m_wireProperty; };
	const WireProperty * getWireProperty() const { return m_wireProperty; };
	
	/** Returns true if watch item is a connector watch. */
	bool isConnectorWatch() const { return getConnector() != (const ConnectorBase *)0; };
	
	/** Returns true if watch item is a connector watch. */
	bool isWirePropertyWatch() const { return getWireProperty() != (const WireProperty *)0; };
	
	/** Return the pointer to the view item.
	  */
	WatchViewItem * getViewItem() { return m_watchViewItem; };
	const WatchViewItem * getViewItem() const { return m_watchViewItem; };
	
	/** Sets the size of the trace buffer.
	  * The function has to be reimplementated in a sub class. It has to resize the buffer. */
	virtual void setTraceSize(unsigned int size);
	/** Returns the size of the trace buffer. */
	unsigned int getTraceSize() const { return m_traceSize; };
	
	/** Resets the item. */
	virtual void reset();
	
	/** Reads the current state of the wire property or the connector.
	  * This function has to be reimplementated in a sub class. See also @ref setIndexUsed.
	  * @param index The index of the history buffer where the data has to be stored.
	  */
	virtual void readData(unsigned int index) = 0;
	
	/** Returns a value from the history buffer as a text.
	  * This function has to be reimplementated in a sub class.
	  * @param index The index of the history buffer where the data has to be fetched from.
	  */
	virtual QString getDataText(unsigned int index) = 0;
	
	/** If the data from the history buffer results in a break situation.
	  * Returns true, if a break situation is present. This will not pause the simulation immediatly because there
	  * is a counter, which counts these events.
	  * This function has to be reimplementated in a sub class.
	  * @param index     The index of the history buffer where the data has to be fetched from.
	  * @param stepCount The number of steps inside the history buffer (never larger than the history buffer).
	  */
	virtual bool testBreak(unsigned int index, unsigned int stepCount) = 0;

	/** Set the value in trace buffer as used.
	  * Do this in a sub class if you store a data in the trace buffer. */
	void setIndexUsed(unsigned int index) { m_traceSet.setBit(index); };
		
	/** Sets the required number of break situations before the simulation is paused. */
	void setBreaksRequired(unsigned int number);
	/** Returns the required number of break situations before the simulation is paused. */
	unsigned int getBreaksRequired() const { return m_breaksRequired; };
	/** Sets the actual number of break situations before the simulation is paused. */
	void setBreaksActual(unsigned int number);
	/** Returns the actual number of break situations before the simulation is paused. */
	unsigned int getBreaksActual() const { return m_breaksActual; };
	
	/** Sets the break function dis-/enabled. */
	void setBreakEnabled(bool ena);
	/** Returns true if the break function is enabled. */
	bool isBreakEnabled() const { return m_breakEnabled; };
	
	
	/** Returns the name of the connector or wire. */
	QString getWatchName() const;
	/** Returns the WatchWidget. */
	WatchWidget * getWatchWidget() const;

	/** Returns the text which is displayed if no value is set. */
	QString getUnsetText();

	/** Reads the data and processes the break situation.
	 *  Returns true, if the break situation is present.*/
	bool execute(unsigned int index, unsigned int stepCount);
	/** Shows the data with is stored at index. */
	void showData(unsigned int index);

	/** Creates and executes the property dialog. */
	void executePropertyDialog(QWidget * parent);


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
	  * The function must be reimplemented in a sub class.
	  * @param parent the parent widget. It is a @ref QVGroupBox */
	virtual void createTriggerPropertyWidget(QWidget * parent) = 0;
	
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
	WatchItemBase(ConnectorBase * connector);
	WatchItemBase(WireProperty * wireProperty);

	/** Sets a new watch view item. */
	void setWatchViewItem(WatchViewItem * item);
	/** Sets the break column to a propery value/pixmap. */
	void showBreakCount(int cnt);
	/** Sets the break column to a propery value/pixmap according to index. */
	void showBreakCountHistory(unsigned int index);
	

private slots:
	void slotNameChanged();
	void slotPropertyOk();
	void slotPropertyDefault();
	void slotPropertyCancel();

private:
	void init();
	
	WireProperty * m_wireProperty;
	ConnectorBase * m_connector;
	WatchViewItem * m_watchViewItem;
	unsigned int m_traceSize;
	unsigned int m_breaksRequired;
	unsigned int m_breaksActual;
	bool m_breakEnabled;
	QBitArray m_traceSet;
	QArray<int> m_traceBreakCnt;
	QSpinBox * m_propertyWidget_cntSpinBox;
	QCheckBox * m_propertyWidget_enaCheckBox;

};

#endif
