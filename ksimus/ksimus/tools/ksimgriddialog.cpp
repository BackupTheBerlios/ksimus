/***************************************************************************
                          ksimgriddialog.cpp  -  description
                             -------------------
    begin                : Mon Oct 9 2000
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

#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qlayout.h>

#include <klocale.h>
#include <kcolorbtn.h>
#include <knuminput.h>

#include "ksimgriddialog.h"
#include "ksimgrid.h"

KSimGridDialogWidget::KSimGridDialogWidget(KSimGrid * grid, QWidget *parent, const char * name)
	:	PropertyWidget(3, parent, name),
		m_gridStart(grid)
{
	// Copy values
	m_gridWork = new KSimGrid(*m_gridStart);
	CHECK_PTR(m_gridWork);
	
	initView();
	initData();
}

KSimGridDialogWidget::~KSimGridDialogWidget()
{
	delete m_gridWork;
}


void KSimGridDialogWidget::initView()
{
	enaGrid = new QCheckBox(i18n("&Enable Grid"), newRowVBox());
	CHECK_PTR(enaGrid);
	connect(enaGrid, SIGNAL(toggled(bool)), SLOT(slotEnabled(bool)));
	
	addEmptyCell();
	
	QLabel * styleLabel = new QLabel(i18n("&Style:") , this);
	CHECK_PTR(styleLabel);

	styleSel = new QComboBox (this);
	CHECK_PTR(styleSel);
	styleSel->insertItem(i18n("Dots"), (int) GridDots);
	styleSel->insertItem(i18n("Dot Line"), (int) GridDotLine);
	styleSel->insertItem(i18n("Dash Line"), (int) GridDashLine);
	styleSel->insertItem(i18n("Dash Dot Line"), (int) GridDashDotLine);
	styleSel->insertItem(i18n("Dash Dot Dot Line"), (int) GridDashDotDotLine);
	styleSel->insertItem(i18n("Solid Line"), (int) GridSolidLine);
	styleLabel->setBuddy(styleSel);
	connect(styleSel, SIGNAL(activated(int)), SLOT(slotStyleSel(int)));
	
	addEmptyCell();
	
	QLabel * colorLabel = new QLabel(i18n("&Color:") , this);
	CHECK_PTR(colorLabel);
	
	colorButton = new KColorButton(m_gridWork->getColor(),this);
	CHECK_PTR(colorButton);
//	colorButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	colorLabel->setBuddy(colorButton);
	connect(colorButton, SIGNAL(changed(const QColor &)), SLOT(slotColorChanged(const QColor &)));
	
	enaGlobalGrid = new QCheckBox(i18n("&User defined grid"), newRowVBox());
	CHECK_PTR(enaGlobalGrid);
	connect(enaGlobalGrid, SIGNAL(toggled(bool)), SLOT(slotGlobalEnabled(bool)));
	
	addEmptyCell();
	
	QLabel * gridXLabel = new QLabel(i18n("&X:") , this);
	CHECK_PTR(gridXLabel);

	gridXEdit = new KIntNumInput (this);
	CHECK_PTR(gridXEdit);
	gridXEdit->setRange(5,100,1,false);
	gridXLabel->setBuddy(gridXEdit);
	connect(gridXEdit, SIGNAL(valueChanged(int)), SLOT(slotGridXChanged(int)));
	
	addEmptyCell();
	
	QLabel * gridYLabel = new QLabel(i18n("&Y:") , this);
	CHECK_PTR(gridYLabel);
	
	gridYEdit = new KIntNumInput (this);
	CHECK_PTR(gridYEdit);
//	gridYEdit->setFixedWidth(metric.width("00000"));
	gridYEdit->setRange(5,100,1,false);
	gridYLabel->setBuddy(gridYEdit);
	connect(gridYEdit, SIGNAL(valueChanged(int)), SLOT(slotGridYChanged(int)));
	
//	setColStretch(0,0);
	setColStretch(2,1);
	addColSpacing(0,16);
}

void KSimGridDialogWidget::initData()
{
	enaGrid->setChecked(m_gridWork->isEnabled());
	
	styleSel->setCurrentItem((int)m_gridWork->getStyle());
	
	colorButton->setColor(m_gridWork->getColor());

	enaGlobalGrid->setChecked(!m_gridWork->isGlobalGridEnabled());

	gridXEdit->setValue(m_gridWork->getGridX());
	gridYEdit->setValue(m_gridWork->getGridY());
	
	slotEnabled(m_gridWork->isEnabled());
}




/** Called if accept button is pressed */
void KSimGridDialogWidget::acceptPressed()
{
	PropertyWidget::acceptPressed();
	*m_gridStart = *m_gridWork;
}

/** Called if cancel button is pressed */
void KSimGridDialogWidget::cancelPressed()
{
	PropertyWidget::cancelPressed();
}

/** Called if apply button is pressed */
void KSimGridDialogWidget::defaultPressed()
{
	PropertyWidget::defaultPressed();
	
	m_gridWork->setEnable(true);
	m_gridWork->setColor(gray);
	m_gridWork->setStyle(GridDots);
	m_gridWork->enableGlobalGrid(true);
	initData();
	
}


void KSimGridDialogWidget::slotEnabled(bool on)
{
	m_gridWork->setEnable(on);
	
	styleSel->setEnabled(on);
	colorButton->setEnabled(on);
	enaGlobalGrid->setEnabled(on);
	gridXEdit->setEnabled(on && !m_gridWork->isGlobalGridEnabled());
	gridYEdit->setEnabled(on && !m_gridWork->isGlobalGridEnabled());
}

void KSimGridDialogWidget::slotStyleSel(int index)
{
    m_gridWork->setStyle((GRIDSTYLE)index);
}

void KSimGridDialogWidget::slotColorChanged(const QColor &newColor)
{
	m_gridWork->setColor(newColor);
}

void KSimGridDialogWidget::slotGlobalEnabled(bool on)
{
	m_gridWork->enableGlobalGrid(!on);
	
	gridXEdit->setEnabled(on);
	gridYEdit->setEnabled(on);
}

void KSimGridDialogWidget::slotGridXChanged(int value)
{
	m_gridWork->setGridX(value);
	m_gridWork->setGridY(value);
	gridYEdit->setValue(value);
}
void KSimGridDialogWidget::slotGridYChanged(int value)
{
	m_gridWork->setGridY(value);
}
	
//###################################################################################

KSimGridDialog::KSimGridDialog(KSimGrid * grid, QWidget *parent, const char * name)
	:	KDialogBase(KDialogBase::Plain,
								i18n("Grid Properties"),
								KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
 								KDialogBase::Ok,
 								parent, name)
{
	QWidget * wid = plainPage();
			
	KSimGridDialogWidget * child = new KSimGridDialogWidget(grid, wid, i18n("Grid Properties"));
			
	QBoxLayout * horLayout = new QHBoxLayout(wid);
	horLayout->setMargin(KDialog::marginHint());
	horLayout->setSpacing(KDialog::spacingHint());
	horLayout->addWidget(child);
			
	connect(this, SIGNAL(okClicked()), child, SLOT(slotAccept()));
	connect(this, SIGNAL(defaultClicked()), child, SLOT(slotDefault()));
	connect(this, SIGNAL(cancelClicked()), child, SLOT(slotCancel()));
	
}
