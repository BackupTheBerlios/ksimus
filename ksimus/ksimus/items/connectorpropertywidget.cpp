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
	
	m_p = new Private();
	
	m_p->rowWidget = new QGrid(1, QGrid::Horizontal, this, "rowWidget");
	m_p->rowWidget->setMargin(KDialog::marginHint());
	m_p->rowWidget->setSpacing(KDialog::spacingHint());
	m_p->boxWidget = new QGrid(2, QGrid::Horizontal, this, "boxWidget");
//	m_p->boxWidget->setMargin(KDialog::marginHint());
	m_p->boxWidget->setSpacing(KDialog::spacingHint());
	
	mainLayout = new QGridLayout(this,2,1);
	m_layout = new QGridLayout(mainLayout,1,2);
	m_layout->addWidget(rowWidget(),0,0);
	m_layout->addWidget(boxWidget(),1,0);
//	mainLayout->addLayout(m_layout,0,0);
	mainLayout->setRowStretch(1,1);
	
	m_p->settingWidget = new QGroupBox(1, Qt::Horizontal, i18n("Settings"), boxWidget(), "boxWidget");
	
	wid = new QWidget(rowWidget());
	lay1 = new QGridLayout(wid,2,2);
	lay1->setMargin(KDialog::marginHint());
	lay1->setSpacing(KDialog::spacingHint());
	lay1->setColStretch(1,1);
	
	label = new QLabel(i18n("Name : "), wid,"LineEditLabel");
	lay1->addWidget(label,0,0);
	m_p->nameEdit = new QLineEdit(wid,"LineEdit");
	m_p->nameEdit->setText(m_conn->getName());
	lay1->addWidget(m_p->nameEdit,0,1);
	QToolTip::add(m_p->nameEdit,i18n("Change the name of the connector here"));

	label = new QLabel(i18n("Type : "), wid,"TypeLabel");
	lay1->addWidget(label,1,0);
	label = new QLabel(m_conn->getConnInfo()->getName(), wid,"Type");
	lay1->addWidget(label,1,1,AlignLeft);
	QToolTip::add(label,i18n("The type of the connector"));
	
	
	if (m_conn->isHideEnabled())
	{
		m_p->hide = new QCheckBox(i18n("Hidden"),settingWidget(),"HiddenCheck");
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

void ConnectorPropertyWidget::slotAccept()
{
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

void ConnectorPropertyWidget::slotDefault()
{
	m_p->nameEdit->setText(m_conn->getInitName());
	if ((m_p->hide != 0) && (m_conn->getWire() == 0))
	{
		m_p->hide->setChecked(m_conn->isInitHidden());
	}
}
	


void ConnectorPropertyWidget::polish()
{
	QWidget::polish();
	
	QWidget * wid = 0;
	setGlobalTabOrder(this,&wid);
}

static void setGlobalTabOrder(QWidget * main, QWidget ** last)
{
	const QObjectList * list = main->children();
	
    if (list)
    {
		QObject * obj;
	    for ( obj = ((QObjectList*)list)->first(); obj; obj = ((QObjectList*)list)->next() )
    	{
    		if (obj->isWidgetType())
	    	{
    			if (*last)
    			{
	    			QWidget::setTabOrder(*last,(QWidget*)obj);
		    	}
		    	*last = (QWidget*)obj;
	    		setGlobalTabOrder((QWidget*)obj, last);
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
