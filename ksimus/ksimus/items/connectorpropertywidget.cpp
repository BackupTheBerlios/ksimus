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

#include <qlist.h>
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

	QGrid * rowWidget;
	QGrid * boxWidget;
	QGroupBox * settingWidget;
		
};

ConnectorPropertyWidget::ConnectorPropertyWidget(ConnectorBase * connector,QWidget *parent, const char *name )
	:	ComponentPropertyBaseWidget(connector->getComponent(),parent,name),
		m_conn(connector)
{
	QLabel * label;
	QGridLayout * lay1;
	QWidget * wid;
	QGridLayout * m_layout;
	QGridLayout * mainLayout;
	QString str;
	
	m_p = new Private();
	CHECK_PTR(m_p);
	
	m_p->rowWidget = new QGrid(1, QGrid::Horizontal, this, "rowWidget");
	CHECK_PTR(m_p->rowWidget);
	m_p->rowWidget->setMargin(KDialog::marginHint());
	m_p->rowWidget->setSpacing(KDialog::spacingHint());
	m_p->boxWidget = new QGrid(2, QGrid::Horizontal, this, "boxWidget");
	CHECK_PTR(m_p->boxWidget);
	m_p->boxWidget->setSpacing(KDialog::spacingHint());
	
	mainLayout = new QGridLayout(this,2,1);
	CHECK_PTR(mainLayout);
	m_layout = new QGridLayout(mainLayout,1,2);
	CHECK_PTR(m_layout);
	m_layout->addWidget(rowWidget(),0,0);
	m_layout->addWidget(boxWidget(),1,0);
	mainLayout->setRowStretch(1,1);
	
	m_p->settingWidget = new QGroupBox(1, Qt::Horizontal, i18n("Settings"), boxWidget(), "boxWidget");
	CHECK_PTR(m_p->settingWidget);
	
	wid = new QWidget(rowWidget());
	CHECK_PTR(wid);
	lay1 = new QGridLayout(wid,2,2);
	CHECK_PTR(lay1);
	lay1->setMargin(KDialog::marginHint());
	lay1->setSpacing(KDialog::spacingHint());
	lay1->setColStretch(1,1);
	
	
	// Name edit
	str = i18n("Change the name of the connector here");
	label = new QLabel(i18n("Name:"), wid,"LineEditLabel");
	CHECK_PTR(label);
	QToolTip::add(label, str);
	lay1->addWidget(label,0,0);
	
	m_p->nameEdit = new QLineEdit(wid,"LineEdit");
	CHECK_PTR(m_p->nameEdit);
	m_p->nameEdit->setText(m_conn->getName());
	lay1->addWidget(m_p->nameEdit,0,1);
	QToolTip::add(m_p->nameEdit, str);

	// Shows connector type
	str = i18n("The shows the connector type.");
	label = new QLabel(i18n("Type:"), wid,"TypeLabel");
	CHECK_PTR(label);
	QToolTip::add(label, str);
	lay1->addWidget(label,1,0);
	
	label = new QLabel(m_conn->getConnInfo()->getName(), wid,"Type");
	CHECK_PTR(label);
	QToolTip::add(label, str);
	lay1->addWidget(label,1,1,AlignLeft);
	
	
	if (m_conn->isHideEnabled())
	{
		m_p->hide = new QCheckBox(i18n("Hidden"),settingWidget(),"HiddenCheck");
		CHECK_PTR(m_p->hide);
		QToolTip::add(m_p->hide,i18n("Hide the connector"));
	
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
}



ConnectorPropertyWidget::~ConnectorPropertyWidget()
{
	delete m_p;
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
}

void ConnectorPropertyWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();
	
	m_p->nameEdit->setText(m_conn->getInitName());
	if ((m_p->hide != 0) && (m_conn->getWire() == 0))
	{
		m_p->hide->setChecked(m_conn->isInitHidden());
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
		QListIterator<QObject> it(*list);
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
		QListIterator<QObject> it(*list);
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
