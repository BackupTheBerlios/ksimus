/***************************************************************************
                          ksimiojoin.h  -  description
                             -------------------
    begin                : Fri Apr 4 2003
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

#ifndef KSIMIOJOIN_H
#define KSIMIOJOIN_H

// C/C++ includes

// QT includes
#include <qobject.h>
#include <qlist.h>

// KDE includes

// Project includes
#include "types.h"
#include "componentitem.h"

// Forward declaration
class Component;
class ConnectorBase;
class KSimIoDevice;
class KSimIoPin;
class KSimIoJoinInfo;
class KSimIoComponent;
class KSimData;


/**Base classes for joins from io device to io connector.
  *@author Rasmus Diekenbrock
  */

class KSimIoJoin : public QObject, public ComponentItem
{
   Q_OBJECT
public:
	class List;

	virtual ~KSimIoJoin();

	/** Copies data from connector to device or from device to connector.
	 *
	 *  You have to implement in a sub class. */
//	virtual void execute() = 0;

	KSimIoDevice * getDevice() const { return m_device; };

	void setPin(const KSimIoPin * pin);
	void setPin(const QString & deviceName, int pinID);
	const KSimIoPin * getPin() const { return m_pin; };
	
	const KSimIoJoinInfo * getInfo() const { return m_info; };

	ConnectorBase * getConnector() const { return m_connectorBase; };

	KSimIoComponent * getIoComponent() const { return (KSimIoComponent*) ComponentItem::getComponent();}

	QString getDefaultPinName() const;
	QString getDeviceName() const;

	void setOrientation(ConnOrientationType orient);
	ConnOrientationType getOrientation() const { return m_orient; };

	virtual ConnectorBase * createConnector() = 0;
	
	/** save join properties */
	virtual void save(KSimData & file) const;
	/** load join properties
	*   Returns true if successful */
	virtual bool load(KSimData & file);

	/** resets the join */
	virtual void reset();

	/** executes the join */
	virtual void calculate() const = 0;

	unsigned int getSerialID() const { return m_serialID; };
	void setSerialID(unsigned int serial) { m_serialID = serial; };

	/** The IO join may be used only once. Eg. output pins should only controlled by one join.
	  * If isExclusive() is true, only one join can use the io pin.
	  * The default is exclusive. */
	bool isExclusive() const;
	void setExclusive(bool exclusive);

	
protected:
	KSimIoJoin(KSimIoComponent* ioComp, const KSimIoJoinInfo* info);
	void setConnector(ConnectorBase * connector);

private: // Functions
	void updateConnName();

			
private:
	class Private;
	
	const KSimIoPin * m_pin;
	KSimIoDevice * m_device;
	const KSimIoJoinInfo * m_info;
	ConnOrientationType m_orient;
	ConnectorBase * m_connectorBase;
	unsigned int m_serialID;
	Private * m_p;

private slots:
	void slotPinDelete();
	void slotConnDeleteRequest(ConnectorBase *);
	
};

//################################################################################
//################################################################################


class KSimIoJoin::List : public QList<KSimIoJoin>
{
public:
//	List();
//	~List();
	KSimIoJoin * findBySerial(unsigned int serial);
};


#define FOR_EACH_IO_JOIN(_it_,_ioJoinList_)  \
        for(QListIterator<KSimIoJoin> _it_(_ioJoinList_);_it_.current();++_it_)


#endif
