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
#include <qlabel.h>
#include <qgroupbox.h>
#include <qvalidator.h>
#include <qhbox.h>
#include <qvbox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "mapsizeswidget.h"
#include "resource.h"
#include "ksimusdoc.h"
#include "compcontainer.h"
#include "compview.h"
#include "ksimundo.h"
#include "ksimdebug.h"
#include "ksimspinbox.h"

// Forward declaration

#include <kdialog.h>


MapSizesWidget::MapSizesWidget(KSimusDoc * doc, QWidget *parent, const char *name)
	:	PropertyWidget(1, parent, name),
		m_doc(doc)
{
/*	QGroupBox * schematicBox;
	QGroupBox * userBox;*/
	QLabel * label;
	QIntValidator * vali;
	QString str;
	
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
		
	QGroupBox * schematicBox1 = new QGroupBox(1, Qt::Horizontal, i18n("Schematic size:"), this, "Schematic size");
	CHECK_PTR(schematicBox1);
	RowLayoutWidget * schematicBox = new RowLayoutWidget(2,schematicBox1);
	
	schematicBox->setSpacing(KDialog::spacingHint());
	
	// Schematic Width
	label = new QLabel(i18n("Width:"), schematicBox);
	CHECK_PTR(label);
	
	m_schematicWidth = new KSimSpinBox(minWidth, 10000, gridX, schematicBox, "m_schematicWidth");
	CHECK_PTR(m_schematicWidth);
	vali = new QIntValidator(minWidth, 10000, m_schematicWidth, "m_schematicWidthValidator");
	CHECK_PTR(vali);
	m_schematicWidth->setValidator(vali);
	label->setBuddy(m_schematicWidth);
	str = i18n("Adjusts the width of the Schematic.");
	addToolTip(str, label, m_schematicWidth);
	addWhatsThis(str, label, m_schematicWidth);
	
	
	// Schematic Height
	label = new QLabel(i18n("Height:"), schematicBox);
	CHECK_PTR(label);
	
	m_schematicHeight = new KSimSpinBox(minHeight, 10000, gridY, schematicBox, "m_schematicHeight");
	CHECK_PTR(m_schematicHeight);
	vali = new QIntValidator(minHeight, 10000, m_schematicHeight, "m_schematicHeightValidator");
	CHECK_PTR(vali);
	m_schematicHeight->setValidator(vali);
	label->setBuddy(m_schematicHeight);
	str = i18n("Adjusts the height of the Schematic.");
	addToolTip(str, label, m_schematicHeight);
	addWhatsThis(str, label, m_schematicHeight);
	
//	schematicBox->setColStretch(0,0);
	schematicBox->setColStretch(1,1);
	
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
		
	QGroupBox * userBox1 = new QGroupBox(2, Qt::Horizontal, i18n("User interface size:"), this, "User size");
	CHECK_PTR(userBox1);
	
	// User Interface Width
	RowLayoutWidget * userBox = new RowLayoutWidget(2,userBox1);
	CHECK_PTR(userBox);
	userBox->setSpacing(KDialog::spacingHint());
	
	label = new QLabel(i18n("Width:"), userBox);
	CHECK_PTR(label);
	
	m_userWidth = new KSimSpinBox(minWidth, 10000, gridX, userBox, "m_userWidth");
	CHECK_PTR(m_userWidth);
	vali = new QIntValidator(minWidth, 10000, m_userWidth, "m_userWidthValidator");
	CHECK_PTR(vali);
	m_userWidth->setValidator(vali);
	label->setBuddy(m_userWidth);
	str = i18n("Adjusts the width of the User Interface.");
	addToolTip(str, label, m_userWidth);
	addWhatsThis(str, label, m_userWidth);
	
	// User Interface Height
//	userBox = new QHBox(this);
	label = new QLabel(i18n("Height:"), userBox);
	CHECK_PTR(label);
	
	m_userHeight = new KSimSpinBox(minHeight, 10000, gridY, userBox, "m_userHeight");
	CHECK_PTR(m_userHeight);
	vali = new QIntValidator(minHeight, 10000, m_userHeight, "m_userHeightValidator");
	CHECK_PTR(vali);
	m_userHeight->setValidator(vali);
	label->setBuddy(m_userHeight);
	str = i18n("Adjusts the height of the User Interface.");
	addToolTip(str, label, m_userHeight);
	addWhatsThis(str, label, m_userHeight);
	
//	userBox->setColStretch(0,0);
	userBox->setColStretch(1,1);
	
	// Fixes a problem with the truncated layout on the right side! Why? TODO
	setRightColSpacing(6 * margin());
	
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

#include "mapsizeswidget.moc"
