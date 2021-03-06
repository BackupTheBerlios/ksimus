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


static const char * const sGroup          = "connPack/%1/";
static const char * const sConnectorCount = "Connector Count";
static const char * const sConn           = "Conn%1/";




ConnectorPack::ConnectorPack(Component * comp,
	                           const QString & name,
	                           const QString & i18nConnName,
	                           const ConnectorInfo * connInfo,
	                           unsigned int minConn, unsigned int maxConn)
	:	ComponentAddOn(comp, name),
		m_name(name),
		m_i18nConnName(i18nConnName),
		m_connInfo(connInfo),
		m_minConn(minConn),
		m_maxConn(maxConn),
		m_storeName(name),
		m_orientation(CO_LEFT),
		m_flags(USE_LETTER | DELETE_LAST_ONLY)
{
	m_connList = new ConnectorList();
	Q_CHECK_PTR(m_connList);
//	m_connList->setAutoDelete(true);   ConnectorList of the Component deletes it
	getAction().setAction(KSimAction::eAction(KSimAction::STORAGE | KSimAction::INITPOPUPMENU));
}

ConnectorPack::~ConnectorPack()
{
}

ConnectorBase * ConnectorPack::internalAddConnector()
{
	ConnectorBase * conn;
	
	if(getConnectorMaximum() > getConnectorCount())
	{
		static const QString dummyStr(QString::fromLatin1("dummy"));  // only a dummy string. Not used!
		conn = getConnectorInfo()->create(getComponent(), dummyStr, dummyStr, QPoint());
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
		KSIMDEBUG(QString::fromLatin1("ConnectorPack::setConnectorCount greater than max (%1>%2)")
		          .arg(connCount).arg(getConnectorMaximum()));
		connCount = getConnectorMaximum();
	}
	if (connCount < getConnectorMinimum())
	{
		KSIMDEBUG(QString::fromLatin1("ConnectorPack::setConnectorCount lesser than min (%1<%2)")
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
	getComponent()->setModified();
	internalAddConnector();
	
	getComponent()->refresh();
}

void ConnectorPack::slotDeleteConnector()
{
	getComponent()->undoChangeProperty(i18n("Delete connector"));
	getComponent()->setModified();
	internalDeleteConnector();

	getComponent()->refresh();

}

void ConnectorPack::slotDeleteConnector(ConnectorBase * conn)
{
	if ((getConnList()->findRef(conn) != 0) && (conn->getWire() == 0))
	{
		getComponent()->undoChangeProperty(i18n("Delete connector"));
		getComponent()->setModified();
		internalDeleteConnector();
		getComponent()->refresh();
	}
}

void ConnectorPack::createNewName(ConnectorBase * conn)
{
	unsigned int i = 0;
	bool found;
	QString wireName;
	QString i18nName;
	
	const QString wireNameTemplate(getName() + " %1");
	
	do
	{
		found = false;
		i++;
		
		if (isLetter() && (i <= (2*26)))
		{
			QChar c( (i <= 26)
			         ? (char)i + 'A' - 1
			         : (char)i + 'a' - 1 - 26 );
			 
			wireName = wireNameTemplate.arg(c);
			i18nName = getConnectorName().arg(c);
		}
		else
		{
			wireName = wireNameTemplate.arg(i);
			i18nName = getConnectorName().arg(i);
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
	QPtrListIterator<ConnectorBase> it(*getConnList());
	
	ConnectorBase * conn = it.toLast();
	
	// No connectors
	if (!conn) return (ConnectorBase *)0;
	
	if (isDeleteLastOnly())
	{
		if (conn->getWire() == 0)
		{
			return conn;
		}
		else
		{
			return (ConnectorBase *)0;
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
	
	QPtrListIterator<ConnectorBase> it(*getConnList());
	
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
		
//	i = popup->insertItem(i18n("Add %1").arg(i18n(getName().latin1())), this, SLOT(slotAddConnector()));
	i = popup->insertItem(i18n("Add %1").arg(getConnectorName().arg(QString::fromLatin1("")).stripWhiteSpace()), this, SLOT(slotAddConnector()));
	if(getConnectorCount() >= getConnectorMaximum())
		popup->setItemEnabled(i, false);
		
//	i = popup->insertItem(i18n("Delete %1").arg(i18n(getName().latin1())), this, SLOT(slotDeleteConnector()));
	i = popup->insertItem(i18n("Delete %1").arg(getConnectorName().arg(QString::fromLatin1("")).stripWhiteSpace()), this, SLOT(slotDeleteConnector()));
	if((getConnectorCount() <= getConnectorMinimum()) || (getDeletableConnector() == 0))
		popup->setItemEnabled(i, false);

	return true;
}

void ConnectorPack::save(KSimData & file) const
{
	const QString oldGroup(file.group());
	const QString group((oldGroup + QString::fromLatin1(sGroup)).arg(getStoreName()));
	file.setGroup(group);
	
	file.writeEntry(sConnectorCount,getConnectorCount());
	
	unsigned int c = 0;
	FOR_EACH_CONNECTOR(it, *getConnList())
	{
		QString connGroup(QString::fromLatin1(sConn).arg(c));
		
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
	const QString oldGroup(file.group());
	const QString group((oldGroup + QString::fromLatin1(sGroup)).arg(getStoreName()));	
	file.setGroup(group);
	
	c = file.readUnsignedNumEntry(sConnectorCount);
	setConnectorCount(c);
		
	c = 0;
	FOR_EACH_CONNECTOR(it, *getConnList())
	{
		QString connGroup(QString::fromLatin1(sConn).arg(c));
		
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
	m_i18nConnName = connName;

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
	return m_i18nConnName;
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

void ConnectorPack::setStoreName(const QString & name)
{
	m_storeName = name;
}

#include "connectorpack.moc"
