/***************************************************************************
                          wireproperty.h  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#ifndef WIREPROPERTY_H
#define WIREPROPERTY_H

// C includes

// QT includes
#include <qobject.h>
#include <qlist.h>

// KDE includes

// Project includes
#include "componentitem.h"

class Wire;
class WirePropertyInfo;
class WireColorScheme;
class QPopupMenu;
class KSimData;
class QPainter;
class ConnectorBase;
class ConnectorList;
class ComponentList;
class WatchItemBase;

/**Base class for all wire properties
  *@author Rasmus Diekenbrock
  */

class WireProperty : public QObject, public ComponentItem
{

	Q_OBJECT

public:
	virtual ~WireProperty();
	
	/** Save wire properties. */
	virtual void save(KSimData & file) const;
	/** Load wire properties.
		*	Returns true if successful. */
	virtual bool load(KSimData & file);

	/** Checks the connected component.
	*   eg. No output connected or more than one connected.
	*   Returns the number of errors.
	*/
	virtual int checkCircuit();
	
	/** Setup the Wire property for a new circuit execution.
	  * The sub class has to implement this function.
	  */
	virtual void setupCircuit() = 0;
	
	/** Executes the WireProperty. This means copies the data from the output connector
	  * to the input connector.
	  * The sub class has to implement this function. */
	virtual void execute() = 0;
	
	/** Returns a pointer to the data that's read from the wire.
	  * Reimplementations is required. */
	virtual const void * readoutData() const = 0;
	
	/** Add menu items depend on wire properties.
	  *	Return true, if items are added. */
	virtual bool initPopupMenu(QPopupMenu * popup) const;
	
	/** Returns the associated WirePropertyInfo. */
	const WirePropertyInfo * getInfo() const;

	/** Returns the wire. */
	Wire * getWire() const { return (Wire *)getComponent(); };
		
	/** Get the colors for the wire property. */
	virtual const WireColorScheme & getColorScheme() const = 0;

	/** Returns a watch item.
	*   The default implementation return a null pointer. Must reimplemented in a sub class.
	*/
	virtual WatchItemBase * makeWatchItem();

protected:	
	WireProperty(Wire * wire, const WirePropertyInfo * wirePropertyInfo);

	const WirePropertyInfo * m_wirePropertyInfo;
	int m_inputCounter;
	int m_outputCounter;
	int m_tristateCounter;
	
};

//##################################################################################
//##################################################################################


class WirePropertySingleOutput : public WireProperty
{

	Q_OBJECT

public:	
	virtual ~WirePropertySingleOutput();
	
	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
	virtual int checkCircuit();
	
	/** Setup the Wire property for a new circuit execution.
	  * Copies this wire property into the connected connectors.
	  */
	virtual void setupCircuit();
	
	/** Executes the WireProperty. This means copies the data from the output connector
	  * to the input connector. */
	virtual void execute();
	
	/** Returns a pointer to the data that's read from the wire.
	  * Reads the data of the output. If no output connector is present a null pointer is returned. */
	virtual const void * readoutData() const;


protected:	
	WirePropertySingleOutput(Wire * wire, const WirePropertyInfo * wirePropertyInfo);

	ConnectorBase * m_outConnector;
	ConnectorList * m_inConnectorList;
	ConnectorList * m_inZeroDelayConnectorList;
	ComponentList * m_zeroDelayList;

};


//##################################################################################
//##################################################################################

class WirePropertyInvalidBase : public WireProperty
{

	Q_OBJECT

public:
//	virtual ~WirePropertyInvalidBase();

	/** Setup the Wire property for a new circuit execution.
	  * Copies this wire property into the connected connectors.
	  */
	virtual void setupCircuit();

	/** Executes the WireProperty. This means copies the data from the output connector
	  * to the input connector. */
	virtual void execute();

	/** Returns a pointer to the data that's read from the wire.
	  * Reads the data of the output. If no output connector is present a null pointer is returned. */
	virtual const void * readoutData() const;

	/** Get the colors for the wire property. */
	virtual const WireColorScheme & getColorScheme() const;

protected:
	WirePropertyInvalidBase(Wire * wire, const WirePropertyInfo * wirePropertyInfo);
};


//##################################################################################
//##################################################################################

const WirePropertyInfo * getWirePropertyInvalidDifferentOutputsInfo();

class WirePropertyInvalidDifferentOutputs : public WirePropertyInvalidBase
{

	Q_OBJECT

public:
	WirePropertyInvalidDifferentOutputs(Wire * wire);
//	virtual ~WirePropertyInvalidDifferentOutputs();

	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
	virtual int checkCircuit();

	/** Returns the error message. */
	static QString getErrorMsg();

private:
	static QString m_errorMsg;
};


//##################################################################################
//##################################################################################

const WirePropertyInfo * getWirePropertyInvalidIncompatibleInputsInfo();

class WirePropertyInvalidIncompatibleInputs : public WirePropertyInvalidBase
{

	Q_OBJECT

public:
	WirePropertyInvalidIncompatibleInputs(Wire * wire);
//	virtual ~WirePropertyInvalidIncompatibleInputs();

	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
	virtual int checkCircuit();

	/** Returns the error message. */
	static QString getErrorMsg();

private:
	static QString m_errorMsg;
};


//##################################################################################
//##################################################################################

const WirePropertyInfo * getWirePropertyInvalidDifferentInputsNoOutputInfo();

class WirePropertyInvalidDifferentInputsNoOutput : public WirePropertyInvalidBase
{

	Q_OBJECT

public:
	WirePropertyInvalidDifferentInputsNoOutput(Wire * wire);
//	virtual ~WirePropertyInvalidDifferentInputsNoOutput();

	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
	virtual int checkCircuit();

	/** Returns the error message. */
	static QString getErrorMsg();

private:
	static QString m_errorMsg;
};


//##################################################################################
//##################################################################################


class WirePropertyList : public QList<WireProperty>
{
public:
//	WirePropertyList();
//	~WirePropertyList();
};

#define FOR_EACH_WIREPROPERTY(_it_,_wirePropertyList_)	\
		for(QListIterator<WireProperty> _it_(_wirePropertyList_);_it_.current();++_it_)


#endif
