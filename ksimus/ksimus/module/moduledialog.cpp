/***************************************************************************
                          moduledialog.cpp  -  description
                             -------------------
    begin                : Wed Jul 19 2000
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

// C-Includes

// QT-Includes
#include <qradiobutton.h>
#include <qvbuttongroup.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qgroupbox.h>
#include <qscrollview.h>
#include <qlayout.h>
#include <qfontmetrics.h>
#include <qvbox.h>
#include <qsplitter.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

// KDE-Includes
#include <kapp.h>
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "moduledialog.h"
#include "moduledata.h"
#include "module.h"
#include "moduleeditor.h"
#include "typeselection.h"
#include "modulepropertywidget.h"
#include "ksimdebug.h"
#include "ksimundo.h"
#include "connectorbase.h"
#include "connectorinfo.h"
#include "component.h"
#include "compcontainer.h"
#include "externalconnector.h"


// Forward declaration


//####################################################################################
//####################################################################################

ModuleDialog::ModuleDialog(CompContainer * container, QWidget *parent, const char *name )
	:	QVBox(parent,name),
		m_container(container),
		mdata(container->getModuleData()),
		connViewItems(0)
{
	// Create Widget
	createView();

	setup();
}

ModuleDialog::~ModuleDialog()
{
	delete connViewItems;
}

/** Setup module data and refreshs view */
void ModuleDialog::setup()
{
	// Possible that pointer mdata is not valid
	mdata = m_container->getModuleData();
	// Setup data
	mdata->searchExternals(m_container->getComponentList());
	mdata->setupGenericData();
	mdata->setupPixmapData();
	mdata->setupUserViewData();
	
	// Init widget data
	initView();
	
	bool ena = true;
	if (mdata->getModuleView() == MV_NONE)
		ena = false;
	if ((mdata->getModuleView() == MV_PIXMAP) && !mdata->isPixmapFileValid())
		ena = false;
		
	connView->setEnabled(ena);
	m_scrView->setEnabled(ena);

	emit signalSetup();
}


void ModuleDialog::resizeEvent(QResizeEvent * ev)
{
	modEdit->updateDrawMapNow();
	QWidget::resizeEvent(ev);
}

CompContainer * ModuleDialog::getContainer() const
{
	return m_container;
}

/** Creates the widget view */
void ModuleDialog::createView()
{
	QVBox * leftBox;
	QSplitter * vertSplitter;
	QSplitter * horiSplitter;
	
	horiSplitter = new QSplitter(Horizontal, this);
	CHECK_PTR(horiSplitter);
	
	leftBox = new QVBox(horiSplitter);
	CHECK_PTR(leftBox);
	
	vertSplitter = new QSplitter(Vertical, horiSplitter);
	CHECK_PTR(vertSplitter);
	// Insert type selection widget
	createTypeSelectionWidget(leftBox);
	
	createEditorWidget(vertSplitter);
	createConnSelectionWidget(vertSplitter);


	// Widget sizes
	QValueList<int> sizeList;

	sizeList.append(50);
	sizeList.append(50);
	horiSplitter->setSizes(sizeList);

	sizeList.clear();
	sizeList.append(70);
	sizeList.append(30);
	vertSplitter->setSizes(sizeList);

}

/** Init the data of the view */
void ModuleDialog::initView()
{
	if (connViewItems)
	{
		connViewItems->clear();
	}
	else
	{
		connViewItems = new QList<QListViewItem>;
		CHECK_PTR(connViewItems);
	}
	connView->clear();
	
	selectedConn = 0;

	FOR_EACH_COMP(it, *mdata->getExternalList())
	{
		const ExternalConnector * extConn = (ExternalConnector*)it.current();
		QListViewItem * item = new QListViewItem(connView);
		CHECK_PTR(item);
		
		item->setText(1, extConn->getName());
		item->setText(2, extConn->getExternalConn()->getConnInfo()->getName());
		connViewItems->append(item);
	}
	modEdit->updateDrawMapNow();
	setupUsedData();
}
	

/** Creates the type selection widget
	Returns a pointer to type selection widget */
QWidget * ModuleDialog::createTypeSelectionWidget(QWidget * parent)
{
	QVBox * box = new QVBox(parent);

	box->setMargin(KDialog::marginHint());
	box->setSpacing(KDialog::spacingHint());

	QWidget * widget1 = new TypeSelection(this, box);
	connect(widget1, SIGNAL(clicked(ModuleViewType)), this, SLOT(setModuleView(ModuleViewType)));
	connect(this, SIGNAL(signalSetup()), widget1, SLOT(setup()));

  QWidget * widget2 =  new ModulePropertyWidget(this, box);
	connect(this, SIGNAL(signalSetup()), widget2, SLOT(setup()));
	connect(widget2, SIGNAL(changed()), SLOT(slotPropertyChanged()));

	QWidget * spacing =  new QWidget(box);
	box->setStretchFactor(spacing, 10);
	
	return box;

}

/** Creates the connector selection widget
	Returns a pointer to connector selection widget */
QWidget * ModuleDialog::createConnSelectionWidget(QWidget * parent)
{
	QString text;
	QVBox * box = new QVBox(parent);

	QString tooltip = i18n("List of the avilable external connectors.\nSelect the connectors here and place it  at the \"Module Preview\" .");
  QToolTip::add(box,tooltip);
  QWhatsThis::add(box,tooltip);

	box->setMargin(KDialog::marginHint());
	box->setSpacing(KDialog::spacingHint());
	
	QGroupBox * widget = new QGroupBox(1,Horizontal,i18n("Connector Selection"),box);
	CHECK_PTR(widget);
	
	QFontMetrics metric(widget->font());
		
	text = i18n("used");
	connView = new QListView(widget);
	CHECK_PTR(connView);
//	connView->setMinimumSize( 100, 50);
	connView->addColumn(text,metric.width(text)+metric.width(QString::fromLatin1("  ")));
	connView->setColumnAlignment(0, AlignHCenter);
	connView->addColumn(i18n("Connector Name"));
	connView->addColumn(i18n("Connector Type"));
	connView->setAllColumnsShowFocus(true);
	connect(connView, SIGNAL(selectionChanged(QListViewItem *)), SLOT(slotSelectConnBox(QListViewItem *)));
    		
	return widget;	
}

/** Creates the module editor widget
	Returns a pointer to connector selection widget */
QWidget * ModuleDialog::createEditorWidget(QWidget * parent)
{
	QVBox * box = new QVBox(parent);
	QString tooltip = i18n("Preview of the module.\nSelect the connectors at the \"Connector selection\" and place it here.");
  QToolTip::add(box,tooltip);
  QWhatsThis::add(box,tooltip);

	box->setMargin(KDialog::marginHint());
	box->setSpacing(KDialog::spacingHint());
	
	QGroupBox * widget = new QGroupBox(1,Horizontal,i18n("Module Preview"), box);
	CHECK_PTR(widget);

	m_scrView = new QScrollView(widget);
	CHECK_PTR(m_scrView);
	modEdit = new ModuleEditor(this, m_scrView->viewport());
	CHECK_PTR(modEdit);
	m_scrView->addChild(modEdit);
	m_scrView->viewport()->setBackgroundColor(white);
	
	return widget;
}

void ModuleDialog::modifyExternals(const QString & description)
{
	KSimUndo * undo = getContainer()->getUndo();
	
	if (undo)
	{
		undo->begin(description);
		undo->reload(mdata->getExternalList());
		undo->end();
	}
	
	getContainer()->setModified();
}

void ModuleDialog::modifyProperties(const QString & description)
{
	KSimUndo * undo = getContainer()->getUndo();
	
	if (undo)
	{
			undo->changeContainerProperty(description);
	}
	
	getContainer()->setModified();
}

/** the slot is called if connector is selected in connBox */
void ModuleDialog::slotSelectConnBox(QListViewItem * selItem)
{
	int idx = connViewItems->findRef(selItem);
	selectedConn = (ExternalConnector *)	mdata->getExternalList()->at(idx);
	modEdit->updateDrawMapNow();
}

/** set the type of module view
	Types are: MV_GENERIC, MV_USERVIEW, MV_PIXMAP */
void ModuleDialog::setModuleView(ModuleViewType viewType)
{
	if (viewType != mdata->getModuleView())
	{
		modifyProperties(i18n("Change Module View"));
		mdata->setModuleView(viewType);
	}
/*	modEdit->updateDrawMapNow();
	setupUsedData();*/
	setup();
}

void ModuleDialog::slotPropertyChanged()
{
	setup();
}

	
void ModuleDialog::posSelected(QPoint & pos, bool insert, ConnOrientationType orientation)
{
	ExternalConnector * extConn = 0;
	
	switch(mdata->getModuleView())
	{
		case MV_NONE:
		case MV_GENERIC:
			break;
		
		case MV_PIXMAP:
			// Search connector on place "pos"
			FOR_EACH_COMP(it,*mdata->getExternalList())
			{
				ExternalConnector * current = (ExternalConnector *)it.current();
				if(current->getPixmapPos() == pos)
				{
					extConn = current;
					break;
				}
			}
			
			if (insert && selectedConn && (selectedConn->getPixmapPos() != pos))
			{
				modifyExternals(i18n("Change Connector Position"));
				if (extConn)
				{
					// A connector on this place. Remove it.
					extConn->setPixmapPos(QPoint(-1,0));
				}
				// place selected connector
				selectedConn->setPixmapPos(pos);
				selectedConn->setPixmapOrientation(orientation);
			}
			else if (!insert && extConn)
			{
				// Remove connector
				modifyExternals(i18n("Remove Connector"));
				extConn->setPixmapPos(QPoint(-1,0));
				selectedConn = extConn;
			}
			setupUsedData();
			break;

		
		case MV_USERVIEW:
			// Search connector on place "pos"
			FOR_EACH_COMP(it,*mdata->getExternalList())
			{
				ExternalConnector * current = (ExternalConnector *)it.current();
				if(current->getUserViewPos() == pos)
				{
					extConn = current;
					break;
				}
			}
			
			if (insert && selectedConn && (selectedConn->getUserViewPos() != pos))
			{
				modifyExternals(i18n("Change Connector Position"));
				if (extConn)
				{
					// A connector on this place. Remove it.
					extConn->setUserViewPos(QPoint(-1,0));
				}
				// place selected connector
				selectedConn->setUserViewPos(pos);
				selectedConn->setUserViewOrientation(orientation);
			}
			else if (!insert && extConn)
			{
				// Remove connector
				modifyExternals(i18n("Remove Connector"));
				extConn->setUserViewPos(QPoint(-1,0));
				selectedConn = extConn;
			}
			setupUsedData();
			break;
			
		default:
			KSIMDEBUG_VAR("unknown viewType", (int)mdata->getModuleView());
			break;
	}
	modEdit->updateDrawMapNow();
}
				
/** update used information in connView */
void ModuleDialog::setupUsedData()
{
	unsigned int i;
	int x;
	ExternalConnector * extConn;
//	PointList * posList = 0;
	
	for (i=0; i < connViewItems->count(); i++)
	{
		extConn = (ExternalConnector *)	mdata->getExternalList()->at(i);
		switch(mdata->getModuleView())
		{
			case MV_NONE:
				x = -1;
				break;
		
			case MV_GENERIC:
				x = 0;	// Always used !!!
				break;
		
			case MV_PIXMAP:
				x = extConn->getPixmapPos().x();
				break;
			
			case MV_USERVIEW:
				x = extConn->getUserViewPos().x();
				break;
			
			default:
				KSIMDEBUG_VAR("unknown viewType", (int)mdata->getModuleView());
				x = -1;
				break;
		}
		
		if (x == -1)
		{
			connViewItems->at(i)->setText(0,QString::fromLatin1(""));
		}
		else
		{
			connViewItems->at(i)->setText(0,QString::fromLatin1("X"));
		}
	}
}

#include "moduledialog.moc"
