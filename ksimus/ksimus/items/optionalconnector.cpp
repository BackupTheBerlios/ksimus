/***************************************************************************
                          optionalconnector.cpp  -  description
                             -------------------
    begin                : Sun Jun 2 2002
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

// C-Includes

// QT-Includes
#include <qlabel.h>
#include <qcombobox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "optionalconnector.h"
#include "connectorbase.h"
#include "ksimdebug.h"
#include "propertywidget.h"

// Forward declaration

class OptionalConnector::Private
{
public:
	Private()
	{};
	~Private()
	{};

	ConnectorList connList;
};

//####################################################################################
//####################################################################################


OptionalConnector::OptionalConnector(Component * component, const QString & addOnName)
	: OptionalFunction(component, addOnName, true)
{
	init();
}

OptionalConnector::OptionalConnector(ConnectorBase * conn, const QString & addOnName)
	: OptionalFunction(conn->getComponent(), addOnName, true)
{
	init();
	addConnector(conn);
}

OptionalConnector::OptionalConnector(ConnectorBase * conn, const QString & addOnName, const QString & i18nLabelText)
	: OptionalFunction(conn->getComponent(), addOnName, true)
{
	init();
	setLabelText(i18nLabelText);
	addConnector(conn);
}

OptionalConnector::OptionalConnector(bool enabled, ConnectorBase * conn, const QString & addOnName, const QString & i18nLabelText)
	: OptionalFunction(conn->getComponent(), addOnName, true)
{
	init();
	setLabelText(i18nLabelText);
	setDefaultEnabled(enabled);
	setEnabled(enabled);
	addConnector(conn);
}

OptionalConnector::~OptionalConnector()
{
	delete m_p;
}

void OptionalConnector::init()
{
	static const QString pageName(i18n("Component property dialog", "Optional connectors"));
	
	m_p = new Private;
	Q_CHECK_PTR(m_p);
	setDialogPageName(pageName);
	getAction().setAction(KSimAction::INITPROPERTYDIALOG);
}

void OptionalConnector::addConnector(ConnectorBase * conn)
{
	if (-1 == m_p->connList.findRef(conn))
	{
		m_p->connList.append(conn);
		conn->setHide(!isEnabled(), true);
		connect(conn, SIGNAL(signalProperty()), SLOT(slotConnProperty()));
		connect(conn, SIGNAL(destroyed()), SLOT(slotConnDestroyed()));
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("Connector %1 already in list %2")
		         .arg(conn->getName()).arg(getName()));
	}
}

void OptionalConnector::setEnabled(bool ena)
{
	if (ena || !isConnected())
	{
		OptionalFunction::setEnabled(ena);
		
		FOR_EACH_CONNECTOR(it, m_p->connList)
		{
			it.current()->setHide(!ena);
		}
	}
	else
	{
		KSIMDEBUG_VAR("One or more connectors are connected", getName());
	}

}
	
ConnectorList * OptionalConnector::getConnList()
{
	return &m_p->connList;
}
ConnectorList * OptionalConnector::getConnList() const
{
	return &m_p->connList;
}

unsigned int OptionalConnector::count() const
{
	return m_p->connList.count();
}


bool OptionalConnector::isConnected() const
{
	return (countConnected() != 0);
}

unsigned int OptionalConnector::countConnected() const
{
	unsigned int u = 0;
	
	FOR_EACH_CONNECTOR(it, m_p->connList)
	{
		if (it.current()->isConnected())
			u++;
	}
	return u;
}

void OptionalConnector::slotConnProperty()
{
	FOR_EACH_CONNECTOR(it, m_p->connList)
	{
		if (!it.current()->isHidden())
		{
			setEnabled(true);
			return;
		}
	}
	setEnabled(false);
}

void OptionalConnector::slotConnDestroyed()
{
	ConnectorBase * conn = (ConnectorBase *)sender();

	if (!m_p->connList.removeRef(conn))
	{
		KSIMDEBUG("Connector not found!!!");
	}
}


QString OptionalConnector::getLabelInternal()
{
	if (getLabelText().isEmpty())
	{
		if (count() == 1)
		{
			return i18n("Connector '%1':").arg(getConnList()->first()->getName());
		}
		else
		{
			KSIMDEBUG("empty i18nLabel is not allowed in comination with more than one connectors.")
			return i18n("Connectors:");
		}
	}
	return getLabelText();
}

QString OptionalConnector::makeToolTip() const
{
	if (count() == 1)
	{
		static const QString i18nText(i18n("Enables or disables the optional connector '%1'."));
		return i18nText.arg(getConnList()->first()->getName());
	}
	else
	{
		static const QString i18nText(i18n("Enables or disables the optional connectors."));
		return i18nText;
	}
}

QString OptionalConnector::makeHints() const
{
	if (isConnected())
	{
		if (count() == 1)
		{
			static const QString i18nText(i18n("\nYou can't disable the connector '%1' because it is in use."));
			return i18nText.arg(getConnList()->first()->getName());
		}
		else
		{
			unsigned int u = countConnected();
			
			if (u == 1)
			{
				static const QString i18nText(i18n("\nYou can't disable the connectors because the connector '%1' is in use."));
				QString connName;
				FOR_EACH_CONNECTOR(it, *getConnList())
				{
					if (it.current()->isConnected())
					{
						connName = it.current()->getName();
						break;
					}
				}
				return i18nText.arg(connName);
			}
			else
			{
				static const QString i18nText(i18n("\nYou can't disable the connectors because the following connectors are in use:"));
				QString msg(i18nText);
				FOR_EACH_CONNECTOR(it, *getConnList())
				{
					if (it.current()->isConnected())
					{
						msg += "\n'" + it.current()->getName() + "'";
					}
				}
				return msg;
			}
		}
	}
	return QString::null;
}


QString OptionalConnector::getToolTipInternal()
{
	QString toolTip(getToolTipText());
	if (toolTip.isEmpty())
	{
		toolTip = makeToolTip();
	}
	
	toolTip += makeHints();
	
	return toolTip;
}

QString OptionalConnector::getWhatsThisInternal()
{
	QString whatsThis(getWhatsThisText());
	if (whatsThis.isEmpty())
	{
		whatsThis = makeToolTip();
	}
	
	whatsThis += makeHints();
	
	return whatsThis;
}

void OptionalConnector::addToPropertyWidget(PropertyWidget * widget)
{
	OptionalFunction::addToPropertyWidget(widget);
	
	PropertyWidget::addToolTip(getToolTipInternal(), getLabel(), getComboBox());
	PropertyWidget::addWhatsThis(getWhatsThisInternal(), getLabel(), getComboBox());
	
	if (isConnected())
	{
		getComboBox()->setEnabled(false);
	}
}

#include "optionalconnector.moc"
