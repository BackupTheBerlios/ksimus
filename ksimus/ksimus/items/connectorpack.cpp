/***************************************************************************
                          connectorpack.cpp  -  description
                             -------------------
    begin                : Tue Nov 27 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

// C-Includes

// QT-Includes
#include <qpopupmenu.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdebug.h"
#include "connectorbase.h"
#include "connectorinfo.h"
#include "component.h"
#include "ksimdata.h"
#include "connectorpack.h"


// Forward declaration


#define USE_LETTER        0x0001L
#define DELETE_LAST_ONLY  0x0002L


static const char * sGroup          = "connPack/%1/";
static const char * sConnectorCount = "Connector Count";
static const char * sConn           = "Conn%1/";




ConnectorPack::ConnectorPack(Component * comp, const QString & name, const ConnectorInfo * connInfo,
                              unsigned int minConn = 1, unsigned int maxConn = 10)
	:	ComponentAddOn(comp, name),
		m_name(name),
		m_connInfo(connInfo),
		m_minConn(minConn),
		m_maxConn(maxConn),
		m_connName(),
		m_orientation(CO_LEFT),
		m_flags(USE_LETTER | DELETE_LAST_ONLY)
{
	m_connList = new ConnectorList();
	CHECK_PTR(m_connList);
//	m_connList->setAutoDelete(true);   ConnectorList of the Component deletes it
	getAction().disable(KSimAction::ALL);
	getAction().enable(KSimAction::eAction(KSimAction::STORAGE | KSimAction::INITPOPUPMENU));
}

ConnectorPack::~ConnectorPack()
{
}

ConnectorBase * ConnectorPack::internalAddConnector()
{
	ConnectorBase * conn;
	
	if(getConnectorMaximum() > getConnectorCount())
	{
		conn = getConnectorInfo()->create(getComponent(), "dummy", QPoint());
		createNewName(conn);
		conn->getAction().disable(KSimAction::STORAGE);
		conn->setOrientation(getOrientation());
		
		getConnList()->append(conn);
		emit signalAddConnector(conn);
	}
	else
	{
		conn = 0;
	}
	
	return conn;
}
	
bool ConnectorPack::internalDeleteConnector()
{
	ConnectorBase * conn = getDeletableConnector();
	bool res;
	
	if ((getConnectorMinimum() < getConnectorCount()) && (conn))
	{
		emit signalDeleteConnector(conn);
		getConnList()->removeRef(conn);
		getComponent()->getConnList()->removeRef(conn);
		emit signalDeletedConnector();
		res = true;
	}
	else
	{
		res = false;
	}
	
	return res;
}

void ConnectorPack::setConnectorCount(unsigned int connCount)
{
	bool deleted = true;
	
//	KSIMDEBUG_VAR("setConnectorCount (1)",getConnectorCount());
	
	// Limit connector count
	if (connCount > getConnectorMaximum())
	{
		KSIMDEBUG(QString("ConnectorPack::setConnectorCount greater than max (%1>%2)")
								.arg(connCount).arg(getConnectorMaximum()));
		connCount = getConnectorMaximum();
	}
	if (connCount < getConnectorMinimum())
	{
		KSIMDEBUG(QString("ConnectorPack::setConnectorCount lesser than min (%1<%2)")
								.arg(connCount).arg(getConnectorMinimum()));
		connCount = getConnectorMinimum();
	}
	
	while((connCount < getConnectorCount()) && (deleted))
	{
		deleted = internalDeleteConnector();
	};
	
	while(connCount > getConnectorCount())
	{
		internalAddConnector();
	};
	
	getComponent()->refresh();
}

unsigned int  ConnectorPack::getConnectorCount() const
{
	return getConnList()->count();
}
	
void ConnectorPack::setConnectorMinimum(unsigned int minConn)
{
	m_minConn = minConn;	
}

void ConnectorPack::setConnectorMaximum(unsigned int maxConn)
{
	m_maxConn = maxConn;	
}

void ConnectorPack::slotAddConnector()
{
	getComponent()->undoChangeProperty(i18n("Add connector"));
	internalAddConnector();
	
	getComponent()->refresh();
}

void ConnectorPack::slotDeleteConnector()
{
	getComponent()->undoChangeProperty(i18n("Delete connector"));
	internalDeleteConnector();

	getComponent()->refresh();

}

void ConnectorPack::slotDeleteConnector(ConnectorBase * conn)
{
	if ((getConnList()->findRef(conn) != 0) && (conn->getWire() == 0))
	{
		getComponent()->undoChangeProperty(i18n("Delete connector"));
		internalDeleteConnector();
		getComponent()->refresh();
	}		
}
	
void ConnectorPack::createNewName(ConnectorBase * conn)
{
	unsigned int i = 0;
	bool found;
	QString name;
	QString wireName;
	QString i18nName;
	
	do
	{
		found = false;
		i++;
		
		if (getConnectorName().isEmpty())
		{
			name = getName() + " %1";
		}
		else
		{
			name = getConnectorName();
		}
		
		
		if (isLetter())
		{
			wireName = name.arg(QChar((char)i + 'A' - 1));
			i18nName = i18n(name.latin1()).arg(QChar((char)i + 'A' - 1));
		}
		else
		{
			wireName = name.arg(i);
			i18nName = i18n(name.latin1()).arg(i);
		}
		
		
		// Exist name ?
		FOR_EACH_CONNECTOR(it, *getConnList())
		{
			if (it.current()->getWireName() == wireName)
			{
				found = true;
				break;
			}
		}
	} while (found);
	
	conn->setWireName(wireName);
	conn->setName(i18nName, true);
}

ConnectorBase * ConnectorPack::getDeletableConnector() const
{
	QListIterator<ConnectorBase> it(*getConnList());
	
	ConnectorBase * conn = it.toLast();
	
	// No connectors
	if (!conn) return 0;
	
  if (isDeleteLastOnly())
  {
  	if (conn->getWire() == 0)
  	{
			return conn;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		while ((it.current() != 0) && (it.current()->getWire() != 0))
		{
			--it;
		}
		return it.current();
	}
}

unsigned int ConnectorPack::getNumberOfDeletableConnectors() const
{
	unsigned int cnt = 0;
	
	QListIterator<ConnectorBase> it(*getConnList());
	
	for(it.toLast(); it.current() != 0; --it)
	{
		if(it.current()->getWire() != 0)
		{
			if (isDeleteLastOnly())
			{
				break;
			}
		}
		else
		{
			cnt++;
		}
	}
	
	return QMIN(cnt, getConnectorCount() - getConnectorMinimum());
}

unsigned int ConnectorPack::getNumberOfNotDeletableConnectors() const
{
	return getConnectorCount() - getNumberOfDeletableConnectors();
}


bool ConnectorPack::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	ComponentAddOn::initPopupMenu(popup);
//	KSIMDEBUG_VAR("",getConnectorCount());
	
	popup->insertSeparator();
		
	i = popup->insertItem(i18n("Add %1").arg(i18n(getName().latin1())), this, SLOT(slotAddConnector()));
	if(getConnectorCount() >= getConnectorMaximum())
		popup->setItemEnabled(i, false);
		
	i = popup->insertItem(i18n("Delete %2").arg(i18n(getName().latin1())), this, SLOT(slotDeleteConnector()));
	if((getConnectorCount() <= getConnectorMinimum()) || (getDeletableConnector() == 0))
		popup->setItemEnabled(i, false);

	return true;
}

void ConnectorPack::save(KSimData & file) const
{
	QString oldGroup(file.group());
	QString group((oldGroup + sGroup).arg(getName()));	
	file.setGroup(group);
	
	file.writeEntry(sConnectorCount,getConnectorCount());
	
	unsigned int c = 0;
	FOR_EACH_CONNECTOR(it, *getConnList())
	{
		QString connGroup(QString(sConn).arg(c));
		
		connGroup = group + connGroup;
		file.setGroup(connGroup);
		it.current()->save(file);
		c++;
	}
	
	file.setGroup(oldGroup);
}

bool ConnectorPack::load(KSimData & file)
{
	unsigned int c;
	QString oldGroup(file.group());
	QString group((oldGroup + sGroup).arg(getName()));	
	file.setGroup(group);
	
	c = file.readUnsignedNumEntry(sConnectorCount);
	setConnectorCount(c);
		
	c = 0;
	FOR_EACH_CONNECTOR(it, *getConnList())
	{
		QString connGroup(QString(sConn).arg(c));
		
		connGroup = group + connGroup;
		file.setGroup(connGroup);
		it.current()->load(file);
		c++;
	}
	
	file.setGroup(oldGroup);
	
	return true;
}

void ConnectorPack::setConnectorName(const QString & connName)
{
	m_connName = connName;

	// #### rename connectors

	// First remove all names
	FOR_EACH_CONNECTOR(it, *getConnList())
	{
		it.current()->setWireName(QString::null);
		it.current()->setName(QString::null, true);
	}

	// Now make new names
	FOR_EACH_CONNECTOR(it, *getConnList())
	{
		createNewName(it.current());
	}
}

QString ConnectorPack::getConnectorName() const
{
	return m_connName;
}

void ConnectorPack::setLetter(bool letter)
{
	if (letter)
	{
		m_flags |= USE_LETTER;
	}
	else
	{
		m_flags &= ~USE_LETTER;
	}
}
	
bool ConnectorPack::isLetter() const
{
	return m_flags & USE_LETTER;
}

/** Sets the orientation of the connectors. */
void ConnectorPack::setOrientation(ConnOrientationType orient)
{
	m_orientation = orient;
	
	FOR_EACH_CONNECTOR(it, *getConnList())
	{
		it.current()->setOrientation(orient);
	}
}

void ConnectorPack::setDeleteLastOnly(bool lastOnly)
{
	if (lastOnly)
	{
		m_flags |= DELETE_LAST_ONLY;
	}
	else
	{
		m_flags &= ~DELETE_LAST_ONLY;
	}
}


bool ConnectorPack::isDeleteLastOnly() const
{
	return m_flags & DELETE_LAST_ONLY;
}


