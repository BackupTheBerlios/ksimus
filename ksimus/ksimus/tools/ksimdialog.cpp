/***************************************************************************
                          ksimdialog.cpp  -  description
                             -------------------
    begin                : Mon Jun 10 2002
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
#include <qlist.h>
#include <qvbox.h>
#include <qobjectlist.h>

// KDE-Includes

// Project-Includes
#include "ksimdialog.h"
#include "propertywidget.h"
#include "ksimdebug.h"

// Forward declaration

class KSimDialog::ListItem
{
public:
	ListItem(const QStringList & i18nName, QWidget * parentWidget, PropertyWidget * propertyWidget = 0)
		:	m_i18nName(i18nName),
			m_parentWidget(parentWidget),
			m_propertyWidget(propertyWidget),
			m_index()
	{};
	
	QStringList m_i18nName;
	QWidget * m_parentWidget;
	PropertyWidget * m_propertyWidget;
	int m_index;
};


//######################################################################################
//######################################################################################

#define FOR_EACH_LISTITEM(_it_,_list_)	\
		for(QListIterator<ListItem> _it_(_list_);_it_.current();++_it_)


class KSimDialog::List : public QList<KSimDialog::ListItem>
{

	public:
	
	ListItem * findI18nName(const QStringList & i18nName) const;
	ListItem * findParentWidget(const QWidget * widget) const;
	ListItem * findPropertyWidget(const QWidget * widget) const;
	ListItem * findIndex(int index) const;
};



KSimDialog::ListItem * KSimDialog::List::findI18nName(const QStringList & i18nName) const
{
	FOR_EACH_LISTITEM(it, *this)
	{
		if (i18nName == it.current()->m_i18nName)
		{
			return it.current();
		}
	}
	return (ListItem *)0;
}

KSimDialog::ListItem * KSimDialog::List::findParentWidget(const QWidget * widget) const
{
	FOR_EACH_LISTITEM(it, *this)
	{
		if (widget == it.current()->m_parentWidget)
		{
			return it.current();
		}
	}
	return (ListItem *)0;
}

KSimDialog::ListItem * KSimDialog::List::findPropertyWidget(const QWidget * widget) const
{
	FOR_EACH_LISTITEM(it, *this)
	{
		if (widget == it.current()->m_propertyWidget)
		{
			return it.current();
		}
	}
	return (ListItem *)0;
}

KSimDialog::ListItem * KSimDialog::List::findIndex(int index) const
{
	FOR_EACH_LISTITEM(it, *this)
	{
		if (index == it.current()->m_index)
		{
			return it.current();
		}
	}
	return (ListItem *)0;
}

//######################################################################################
//######################################################################################

class KSimDialog::Private
{

public:
	Private()
		:	m_changed(false)
	{
		m_pageList = new List;
		CHECK_PTR(m_pageList);
		m_pageList->setAutoDelete(true);
	}

	~Private()
	{
		delete m_pageList;
		m_pageList = 0;
	}

	List * m_pageList;
	bool m_changed;
};


//######################################################################################
//######################################################################################

KSimDialog::KSimDialog(const QString & caption, QWidget *parent, const char *name)
	:	KDialogBase(TreeList,
		            caption,
		            Default | Ok | Cancel,
		            Ok,
		            parent,
		            name)
{
	m_p = new Private();
	CHECK_PTR(m_p);
}

KSimDialog::~KSimDialog()
{
	delete m_p;
	m_p = 0;
}

QStringList KSimDialog::makeUniqueName(QStringList i18nName)
{
	if(m_p->m_pageList->findI18nName(i18nName) != 0)
	{
		KSIMDEBUG_VAR("FIXME: double item", i18nName.join(QString::fromLatin1("/")));
		do
		{
			i18nName.last() += QString::fromLatin1(".");
		} while (m_p->m_pageList->findI18nName(i18nName) != 0);
	};
	return i18nName;
}

QVBox * KSimDialog::addVBoxPage(const QString & item)
{
	return addVBoxPage(QStringList(item));
}

QVBox * KSimDialog::addVBoxPage(const QStringList & item)
{
	QStringList myItem(makeUniqueName(item));
	QVBox * box = KDialogBase::addVBoxPage(myItem);
	ListItem * listItem = new ListItem(myItem, box);
	CHECK_PTR(listItem);
	m_p->m_pageList->append(listItem);
	connect(box,SIGNAL(destroyed()),this,SLOT(pageDeleted()));
	return box;
}

PropertyWidget * KSimDialog::getPage(const QStringList & item)
{
	ListItem * listItem = m_p->m_pageList->findI18nName(item);
	
	if (listItem)
	{
		// Found item
		if (!listItem->m_propertyWidget)
		{
			// m_propertyWidget is *not* set --> search it
			const QObjectList * childList = listItem->m_parentWidget->children();
			if (childList)
			{
				QListIterator<QObject> it(*childList);
				for (; it.current(); ++it)
				{
					if (it.current()->isWidgetType())
					{
						if (((QWidget*)it.current())->inherits("PropertyWidget"))
						{
							listItem->m_propertyWidget = (PropertyWidget*)it.current();
							break;
						}
					}
				}
			}
		}
		return listItem->m_propertyWidget;
	}
	else
	{
		// Page does not exist --> create it
		QVBox * box = addVBoxPage(item);
		if (box)
		{
			PropertyWidget * widget;
			widget = new PropertyWidget(2, box);
			CHECK_PTR(widget);
//			widget->setColStretch(0,0);
			widget->setColStretch(1,1);
			widget->setCenterRowStretch(0);
			widget->setBottomRowStretch(1);
			m_p->m_pageList->last()->m_propertyWidget = widget;
			connectSlots(widget);
			
			return widget;
		}
		return (PropertyWidget*)0;
	}
}

void KSimDialog::pageDeleted()
{
	if (!m_p || !m_p->m_pageList) return;
	
	QWidget * delWidget = (QWidget *) sender();
	
	ListItem * listItem = m_p->m_pageList->findParentWidget(delWidget);
	
	if (listItem)
	{
		m_p->m_pageList->removeRef(listItem);
	}
	else
	{
		KSIMDEBUG("Item not found");
	}

}


void KSimDialog::connectSlots(PropertyWidget * wid)
{
	connect(this, SIGNAL(okClicked()), wid, SLOT(slotAccept()));
	connect(this, SIGNAL(cancelClicked()), wid, SLOT(slotCancel()));
	connect(wid, SIGNAL(signalChangeData()), this, SLOT(slotDataChanged()));
	
	QObject * daddy = wid->parent();
	if (daddy && daddy->isWidgetType())
	{
		int index = pageIndex((QWidget*)daddy);
		if (index != -1)
		{
			ListItem * item = m_p->m_pageList->findParentWidget((QWidget*)daddy);
			if (item)
			{
				if (item->m_propertyWidget == 0)
				{
					item->m_propertyWidget = wid;
				}
				item->m_index = index;
			}
			else
			{
				KSIMDEBUG("FIXME: Item not found");
			}
		}
		else
		{
			KSIMDEBUG(QString::fromLatin1("Invalid Widget %1 (parent is %2)")
			          .arg(QString::fromLatin1(wid->name()))
			          .arg(QString::fromLatin1(daddy->name())));
		}
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("Widget %1 has no (widget) parent")
		          .arg(QString::fromLatin1(wid->name())));
	}
}


void KSimDialog::slotOk()
{
	emit okClicked();
	QDialog::accept();
}

void KSimDialog::slotDefault()
{
	KDialogBase::slotDefault();
	
	ListItem * item = m_p->m_pageList->findIndex(activePageIndex());
	if(item)
	{
		QTimer::singleShot(0, item->m_propertyWidget, SLOT(slotDefault()));
	}
}


void KSimDialog::slotDataChanged()
{
	m_p->m_changed = true;
}

bool KSimDialog::isChanged() const
{
	return m_p->m_changed;
}

void KSimDialog::setChanged(bool changed)
{
	m_p->m_changed = changed;
}


