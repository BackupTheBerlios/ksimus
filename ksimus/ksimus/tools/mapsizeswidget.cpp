/***************************************************************************
                          mapsizeswidget.cpp  -  description
                             -------------------
    begin                : Sun Jan 6 2002
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
#include <qlayout.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qvalidator.h>


// KDE-Includes
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "mapsizeswidget.h"
#include "resource.h"
#include "ksimusdoc.h"
#include "compcontainer.h"
#include "compview.h"
#include "ksimundo.h"

// Forward declaration



MapSizesWidget::MapSizesWidget(KSimusDoc * doc, QWidget *parent, const char *name)
	:	PropertyWidget(parent, name),
		m_doc(doc)
{
	QGroupBox * schematicBox;
	QGroupBox * userBox;
	QLabel * label;
	QIntValidator * vali;
	QGridLayout * layout;
	
	QRect allViews;
	int minWidth, minHeight;
	
	// ##### Schematic #####
	
	allViews = m_doc->getContainer()->getSheetViewList()->getRect();
	if (allViews.isNull())
	{
		minWidth = gridX;
		minHeight = gridY;
	}
	else
	{
		minWidth = allViews.right();
		minHeight = allViews.bottom();
	}
		
	schematicBox = new QGroupBox(2, Horizontal, i18n("Schematic size:"), this, "Schematic size");
	CHECK_PTR(schematicBox);
	
	// Schematic Width
	label = new QLabel(i18n("Width:"), schematicBox);
	CHECK_PTR(label);
	
	m_schematicWidth = new QSpinBox(minWidth, 10000, gridX, schematicBox, "m_schematicWidth");
	CHECK_PTR(m_schematicWidth);
	vali = new QIntValidator(minWidth, 10000, m_schematicWidth, "m_schematicWidthValidator");
	CHECK_PTR(vali);
	m_schematicWidth->setValidator(vali);
	
	
	// Schematic Height
	label = new QLabel(i18n("Height:"), schematicBox);
	CHECK_PTR(label);
	
	m_schematicHeight = new QSpinBox(minHeight, 10000, gridY, schematicBox, "m_schematicHeight");
	CHECK_PTR(m_schematicHeight);
	vali = new QIntValidator(minHeight, 10000, m_schematicHeight, "m_schematicHeightValidator");
	CHECK_PTR(vali);
	m_schematicHeight->setValidator(vali);
	
	// ##### User Interface #####
	
	allViews = m_doc->getContainer()->getUserViewList()->getRect();
	if (allViews.isNull())
	{
		minWidth = gridX;
		minHeight = gridY;
	}
	else
	{
		minWidth = allViews.right();
		minHeight = allViews.bottom();
	}
		
	userBox = new QGroupBox(2, Horizontal, i18n("User interface size:"), this, "User size");
	CHECK_PTR(userBox);
	
	// User Interface Width
	label = new QLabel(i18n("Width:"), userBox);
	CHECK_PTR(label);
	
	m_userWidth = new QSpinBox(minWidth, 10000, gridX, userBox, "m_userWidth");
	CHECK_PTR(m_userWidth);
	vali = new QIntValidator(minWidth, 10000, m_userWidth, "m_userWidthValidator");
	CHECK_PTR(vali);
	m_userWidth->setValidator(vali);
	
	// User Interface Height
	label = new QLabel(i18n("Height:"), userBox);
	CHECK_PTR(label);
	
	m_userHeight = new QSpinBox(minHeight, 10000, gridY, userBox, "m_userHeight");
	CHECK_PTR(m_userHeight);
	vali = new QIntValidator(minHeight, 10000, m_userHeight, "m_userHeightValidator");
	CHECK_PTR(vali);
	m_userHeight->setValidator(vali);
	
	
	layout = new QGridLayout(this,3,2);	
	CHECK_PTR(vali);
	layout->setMargin(KDialog::marginHint());
	layout->setSpacing(KDialog::spacingHint());
	layout->colStretch(1);

	layout->addWidget(schematicBox,0,0);
	layout->addWidget(userBox,1,0);

/*	layout->addWidget(tickLabel,2,0);
	layout->addWidget(m_tickSynchronized,2,1);
	layout->addWidget(m_tickTime,3,1);*/

	layout->colStretch(2);
	
	// Setup values
	defaultPressed();
	
	connect(this,SIGNAL(signalChangeData()), this, SLOT(slotUndo()));
}

/*MapSizesWidget::~MapSizesWidget()
{
} */



void MapSizesWidget::acceptPressed()
{
	QSize currentSize;
	QSize newSize;
	bool changed = false;
	
	// ##### Schematic #####
	currentSize = m_doc->getSheetSize();
	newSize = QSize(m_schematicWidth->value(), m_schematicHeight->value());
	
	if (currentSize != newSize)
	{
		changeData();
		m_doc->setSheetSize(newSize);
		changed = true;
	}
	
	// ##### User Interface #####
	currentSize = m_doc->getUserSize();
	newSize = QSize(m_userWidth->value(), m_userHeight->value());
	
	if (currentSize != newSize)
	{
		if (!changed)
			changeData();
		m_doc->setUserSize(newSize);
	}
}

void MapSizesWidget::defaultPressed()
{
	QSize currentSize;
	
	currentSize = m_doc->getSheetSize();
	m_schematicWidth->setValue(currentSize.width());
	m_schematicHeight->setValue(currentSize.height());

	currentSize = m_doc->getUserSize();
	m_userWidth->setValue(currentSize.width());
	m_userHeight->setValue(currentSize.height());
}


/** Stores data for undo mechanism */
void MapSizesWidget::slotUndo()
{
	KSimUndo * undo = m_doc->getUndo();
	if(undo)
	{
		undo->changeContainerProperty(i18n("Change Map Size"));
		m_doc->setModified();
	}
}

