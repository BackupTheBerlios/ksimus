/***************************************************************************
                          componentpropertygeneralwidget.cpp  -  description
                             -------------------
    begin                : Sat Feb 10 2001
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
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qpopupmenu.h>
#include <qhbox.h>
#include <qcursor.h>
#include <qintdict.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <qpointarray.h>
#include <qcolor.h>

// KDE-Includes
#include <klocale.h>
#include <kconfig.h>
#include <kapp.h>

// Project-Includes
#include "componentpropertygeneralwidget.h"
#include "component.h"
#include "componentinfo.h"
#include "connectorbase.h"
#include "wire.h"
#include "ksimdebug.h"
#include "ksimicon.h"

// Forward declaration

class ComponentPropertyGeneralWidget::Private
{
public:
	Private()
		: nameEdit((QLineEdit *)0),
			suggestionList(),
			suggestRecentList(),
			suggestionDict(),
			suggestionButton((QPushButton *)0),
			suggestionMenu((QPopupMenu *)0)
	{};
	~Private()
	{};

	QLineEdit * nameEdit;
	QPushButton * clearNameButton;
	QStringList suggestionList;
	QStringList suggestRecentList;
	QIntDict<const QString> suggestionDict;
	QPushButton * suggestionButton;
	QPopupMenu * suggestionMenu;
};


//##################################################################################
//##################################################################################

ComponentPropertyGeneralWidget::ComponentPropertyGeneralWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, 2, parent, name)
{
//	setColStretch(0,0);
	setColStretch(1,1);
	
	QString str;
	QLabel * label;

	m_p = new Private();
	CHECK_PTR(m_p);
	
	// Name edit	
	str = i18n("Change the name of the component here");
	label = new QLabel(i18n("Name:"), this,"m_p->nameEdit Label");
	CHECK_PTR(label);
	addToolTip(str, label);
	addWhatsThis(str, label);

	QHBox * nameBox = new QHBox(this, "nameBox");
	CHECK_PTR(nameBox);
	nameBox->setSpacing(0);
	nameBox->setMargin(0);
	
	m_p->nameEdit = new QLineEdit(nameBox,"m_p->nameEdit");
	CHECK_PTR(m_p->nameEdit);
	m_p->nameEdit->setText(getComponent()->getName());
	addToolTip(str, m_p->nameEdit);
	addWhatsThis(str, m_p->nameEdit);

	str = i18n("Deletes the name input line.");
	m_p->clearNameButton = new QPushButton(nameBox, "m_p->clearNameButton");
	CHECK_PTR(m_p->clearNameButton);
	QPixmap clearNamePix(KSimIcon::load("clear_left", KIcon::Small));
	m_p->clearNameButton->setPixmap(clearNamePix);
	connect(m_p->clearNameButton, SIGNAL(clicked()), SLOT(slotClearName()));
	addToolTip(str, m_p->clearNameButton);
	addWhatsThis(str, m_p->clearNameButton);

	str = i18n("Gives a name suggestion.");

	m_p->suggestionButton = new QPushButton(nameBox, "m_p->suggestionButton");
	CHECK_PTR(m_p->suggestionButton);
	QPixmap suggestionPix(KSimIcon::load("1rightarrow", KIcon::Small));
	m_p->suggestionButton->setPixmap(suggestionPix);
	addToolTip(str, m_p->suggestionButton);
	addWhatsThis(str, m_p->suggestionButton);
	connect(m_p->suggestionButton, SIGNAL(pressed()), SLOT(slotOpenSuggestionName()));

	// Component type
	str = i18n("Shows the component type.");
	
	label = new QLabel(i18n("Type:"), this);
	CHECK_PTR(label);
	addToolTip(str, label);
	addWhatsThis(str, label);
	
	label = new QLabel(getComponent()->getInfo()->getName(), this);
	CHECK_PTR(label);
	addToolTip(str, label);
	addWhatsThis(str, label);
}

ComponentPropertyGeneralWidget::~ComponentPropertyGeneralWidget()
{
	delete m_p;
}

void ComponentPropertyGeneralWidget::addNameSuggestion(const QString & suggName)
{
	if (!m_p->suggestionList.contains(suggName))
	{
		m_p->suggestionList.append(suggName);
	}
}
void ComponentPropertyGeneralWidget::addNameSuggestion(const QStringList & suggNameList)
{
	QStringList::ConstIterator it;
	for(it = suggNameList.begin(); it != suggNameList.end(); ++it)
	{
		addNameSuggestion(*it);
	}
}


void ComponentPropertyGeneralWidget::slotOpenSuggestionName()
{
//	if (!m_p->suggestionMenu)
	delete m_p->suggestionMenu;
	{
		m_p->suggestionMenu = new QPopupMenu(this);
		CHECK_PTR(m_p->suggestionMenu);
		connect(m_p->suggestionMenu, SIGNAL(activated(int)), SLOT(slotSuggestionName(int)));

		// Add recently used names
		{
			KConfig * config=kapp->config();
			QString oldGroup(config->group());
			config->setGroup("Component Name");
			m_p->suggestRecentList = config->readListEntry("Recently Used");
			config->setGroup(oldGroup);
			if (m_p->suggestRecentList.count() != 0)
			{
				QPopupMenu * recentMenu = new QPopupMenu(m_p->suggestionMenu);
				CHECK_PTR(recentMenu);
				m_p->suggestionMenu->insertItem(i18n("Recently used"), recentMenu);
				connect(recentMenu, SIGNAL(activated(int)), SLOT(slotSuggestionName(int)));
				
				QStringList::ConstIterator it;
				for(it = m_p->suggestRecentList.begin(); it != m_p->suggestRecentList.end(); ++it)
				{
					int id = recentMenu->insertItem(*it);
					m_p->suggestionDict.insert(id, &(*it));
				}
			}
		}
		

		// Add connector and wire names
		QStringList connMenuList;
		QStringList wireMenuList;
		FOR_EACH_CONNECTOR(it, *getComponent()->getConnList())
		{
			ConnectorBase * conn (it.current());
			if (!connMenuList.contains(conn->getName()))
			{
				connMenuList.append(conn->getName());
			}

			Component * comp = conn->getWire();
			if (comp && (comp != getComponent())
			         && (comp->getName() != comp->getDefaultName())
			         && !wireMenuList.contains(comp->getName()))
			{
				wireMenuList.append(comp->getName());
			}
		}

		if (connMenuList.count() != 0)
		{
			QPopupMenu * connMenu = new QPopupMenu(m_p->suggestionMenu);
			CHECK_PTR(connMenu);
			m_p->suggestionMenu->insertItem(i18n("Connectors"), connMenu);
			connect(connMenu, SIGNAL(activated(int)), SLOT(slotSuggestionName(int)));
			QStringList::ConstIterator it;
			for(it = connMenuList.begin(); it != connMenuList.end(); ++it)
			{
				int id = connMenu->insertItem(*it);
				m_p->suggestionDict.insert(id, &(*it));
			}
		}
			
		if (wireMenuList.count() != 0)
		{
			QPopupMenu * wireMenu = new QPopupMenu(m_p->suggestionMenu);
			CHECK_PTR(wireMenu);
			m_p->suggestionMenu->insertItem(i18n("Wires"), wireMenu);
			connect(wireMenu, SIGNAL(activated(int)), SLOT(slotSuggestionName(int)));
			QStringList::ConstIterator it;
			for(it = wireMenuList.begin(); it != wireMenuList.end(); ++it)
			{
				int id = wireMenu->insertItem(*it);
				m_p->suggestionDict.insert(id, &(*it));
			}
		}

		{
			QStringList::ConstIterator it;
			for(it = m_p->suggestionList.begin(); it != m_p->suggestionList.end(); ++it)
			{
				int id = m_p->suggestionMenu->insertItem(*it);
				m_p->suggestionDict.insert(id, &(*it));
			}
		}
	}
	m_p->suggestionMenu->exec(QCursor::pos());
	m_p->suggestionButton->setDown(false);
}

void ComponentPropertyGeneralWidget::slotSuggestionName(int id)
{
	const QString * pStr = m_p->suggestionDict.find(id);
	if (pStr)
	{
		m_p->nameEdit->setText(*pStr);
	}
	else
	{
		KSIMDEBUG_VAR("FIXME: String not found", id);
	}
}

void ComponentPropertyGeneralWidget::slotClearName()
{
	m_p->nameEdit->setText(QString::null);
}


void ComponentPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();

	QString newName(m_p->nameEdit->text());
	if (getComponent()->getName() != newName)
	{
		changeData();
		getComponent()->setName(newName);
		if ((getComponent()->getDefaultName() != newName) && (newName != QString::null))
		{
			KConfig * config=kapp->config();
			QString oldGroup(config->group());
			config->setGroup("Component Name");
			unsigned int maxCount = config->readUnsignedNumEntry("Recently Used Count", 10);
			m_p->suggestRecentList.remove(newName);  // remove if exist
			m_p->suggestRecentList.prepend(newName);
			while (m_p->suggestRecentList.count() > maxCount)
			{
				m_p->suggestRecentList.remove(m_p->suggestRecentList.end());
			}
			config->writeEntry("Recently Used", m_p->suggestRecentList);
			config->setGroup(oldGroup);
		}
	}

}

void ComponentPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	m_p->nameEdit->setText(getComponent()->getDefaultName());
	
}

#include "componentpropertygeneralwidget.moc"
