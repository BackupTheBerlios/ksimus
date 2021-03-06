/***************************************************************************
                          connectorpropertywidget.cpp  -  description
                             -------------------
    begin                : Thu Jan 11 2001
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
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qgrid.h>

#include <qptrlist.h>
#include <qobjectlist.h>
#include <qbuttongroup.h>
#include <qtooltip.h>


// KDE-Includes
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "connectorpropertywidget.h"
#include "connectorbase.h"
#include "connectorinfo.h"
#include "ksimdebug.h"


// Forward declaration

static void setGlobalTabOrder(QWidget * main, QWidget ** last);





class ConnectorPropertyWidget::Private
{
public:	
	Private() :
		rowWidget(0),
		boxWidget(0),
		settingWidget(0)
	 {};
	~Private() {};
	
	QLineEdit * nameEdit;
	QCheckBox * hide;
	QCheckBox * negate;

	QGrid * rowWidget;
	QGrid * boxWidget;
	QGroupBox * settingWidget;
		
};

ConnectorPropertyWidget::ConnectorPropertyWidget(ConnectorBase * connector,QWidget *parent, const char *name )
	:	ComponentPropertyBaseWidget(connector->getComponent(),1, parent,name),
		m_conn(connector)
{
	QLabel * label;
	QGrid * grid;
	QString str;
	
	m_p = new Private();
	Q_CHECK_PTR(m_p);
	
	m_p->rowWidget = new QGrid(1, QGrid::Horizontal, this, "rowWidget");
	Q_CHECK_PTR(m_p->rowWidget);
	m_p->rowWidget->setMargin(KDialog::marginHint());
	m_p->rowWidget->setSpacing(KDialog::spacingHint());
	m_p->boxWidget = new QGrid(2, QGrid::Horizontal, this, "boxWidget");
	Q_CHECK_PTR(m_p->boxWidget);
	m_p->boxWidget->setSpacing(KDialog::spacingHint());
	
	m_p->settingWidget = new QGroupBox(1, Qt::Horizontal, i18n("Settings"), boxWidget(), "boxWidget");
	Q_CHECK_PTR(m_p->settingWidget);
	
	grid = new QGrid(2, /*QGrid::Horizontal,*/ rowWidget());
	Q_CHECK_PTR(grid);
	grid->setSpacing(KDialog::spacingHint());
	
	
	// Name edit
	str = i18n("Change the name of the connector here");
	label = new QLabel(i18n("Name:"), grid,"LineEditLabel");
	Q_CHECK_PTR(label);
	
	m_p->nameEdit = new QLineEdit(grid,"LineEdit");
	Q_CHECK_PTR(m_p->nameEdit);
	m_p->nameEdit->setText(m_conn->getName());
	label->setBuddy(m_p->nameEdit);
	PropertyWidget::addToolTip(str, label, m_p->nameEdit);
	PropertyWidget::addWhatsThis(str, label, m_p->nameEdit);
	

	// Shows connector type
	str = i18n("The shows the connector type.");
	label = new QLabel(i18n("Type:"), grid,"TypeLabel");
	Q_CHECK_PTR(label);
	
	QLabel * label1 = new QLabel(m_conn->getConnInfo()->getName(), grid,"Type");
	Q_CHECK_PTR(label1);
	label->setBuddy(label1);
	PropertyWidget::addToolTip(str, label, label1);
	PropertyWidget::addWhatsThis(str, label, label1);
	
	
	if (m_conn->isHideEnabled())
	{
		m_p->hide = new QCheckBox(i18n("Hidden"),settingWidget(),"HiddenCheck");
		Q_CHECK_PTR(m_p->hide);
		str = i18n("Hide the connector");
		PropertyWidget::addToolTip(str, m_p->hide);
		PropertyWidget::addWhatsThis(str, m_p->hide);
	
		if (m_conn->getWire() != 0)
		{
			m_p->hide->setEnabled(false);
		}
		m_p->hide->setChecked(m_conn->isHidden());
	}
	else
	{
		m_p->hide = 0;
	}

	if (m_conn->isNegateEnabled())
	{
		m_p->negate = new QCheckBox(i18n("Negated"),settingWidget(),"NegateCheck");
		m_p->negate->setChecked(m_conn->isNegated());
		str = i18n("Negate the connector");
		PropertyWidget::addToolTip(str, m_p->negate);
		PropertyWidget::addWhatsThis(str, m_p->negate);
	}
	else
	{
		m_p->negate = 0;
	}

	connect(m_conn, SIGNAL(destroyed()), this, SLOT(slotDelete()));
}



ConnectorPropertyWidget::~ConnectorPropertyWidget()
{
	delete m_p;
}

void ConnectorPropertyWidget::slotDelete()
{
	delete this;
}

void ConnectorPropertyWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();
	
	if (m_conn->getName() != m_p->nameEdit->text())
	{
		changeData();
		m_conn->setName(m_p->nameEdit->text());
	}
	if ((m_p->hide != 0) && (m_conn->isHidden() != m_p->hide->isChecked()))
	{
		changeData();
		m_conn->setHide(m_p->hide->isChecked());
	}
	if (m_p->negate && (m_conn->isNegated() != m_p->negate->isChecked()))
	{
		changeData();
		m_conn->setNegate(m_p->negate->isChecked());
	}
}

void ConnectorPropertyWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();
	
	m_p->nameEdit->setText(m_conn->getInitName());
	if ((m_p->hide != 0) && (m_conn->getWire() == 0))
	{
		m_p->hide->setChecked(m_conn->isInitHidden());
	}
	if (m_p->negate)
	{
		m_p->negate->setChecked(m_conn->isInitNegate());
	}
}


void ConnectorPropertyWidget::polish()
{
	QWidget::polish();
	
	// Hide setting widget if it does not contains properties
	bool hideSetting = true;
	const QObjectList * list = m_p->settingWidget->children();
	if (list)
	{
		QPtrListIterator<QObject> it(*list);
		for (; it.current(); ++it)
		{
			if (it.current()->isWidgetType())
			{
				if (!((QWidget*)it.current())->isHidden())
				{
					hideSetting = false;
					break;
				}
			}
		}
	}
	if (hideSetting)
	{
		m_p->settingWidget->hide();
	}
		
	// Set tab order
	QWidget * wid = 0;
	setGlobalTabOrder(this,&wid);
}

static void setGlobalTabOrder(QWidget * main, QWidget ** last)
{
	const QObjectList * list = main->children();
	
	if (list)
	{
		QPtrListIterator<QObject> it(*list);
		for (; it.current(); ++it)
		{
			if (it.current()->isWidgetType())
			{
				if (*last)
				{
					QWidget::setTabOrder(*last,(QWidget*)it.current());
				}
				*last = (QWidget*)it.current();
				setGlobalTabOrder((QWidget*)it.current(), last);
			}
		}
	}
}	

QWidget * ConnectorPropertyWidget::rowWidget()
{
	return m_p->rowWidget;
}
QWidget * ConnectorPropertyWidget::boxWidget()
{
	return m_p->boxWidget;
}
QWidget * ConnectorPropertyWidget::settingWidget()
{
	return m_p->settingWidget;
}

#include "connectorpropertywidget.moc"
