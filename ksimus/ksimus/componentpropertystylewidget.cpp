/***************************************************************************
                          componentpropertystylewidget.cpp  -  description
                             -------------------
    begin                : Sun Dec 30 2001
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
#include <qtooltip.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qlayout.h>
#include <qgrid.h>
#include <qcheckbox.h>

// KDE-Includes
#include <kdialog.h>
#include <klocale.h>
#include <kcolorbtn.h>

// Project-Includes
#include "componentpropertystylewidget.h"
#include "componentstyle.h"
#include "componentinfo.h"
#include "ksimdebug.h"

// Forward declaration


ComponentPropertyStyleWidget::ComponentPropertyStyleWidget(ComponentStyle * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, parent, name)
{
	QGridLayout * layout;
	QString str;
	QLabel * label;
	
	m_vBox = new QVBox(this, "Style VBox");
	CHECK_PTR(m_vBox);
	
	m_vBox->setMargin(KDialog::marginHint());
	m_vBox->setSpacing(KDialog::spacingHint());
	
	
	// Color Group	
	m_colorGroup = new QVGroupBox(i18n("Color:"), getVBox(), "Style Color Box");
	CHECK_PTR(m_colorGroup);
	
	// Color Group - Default colors
	m_defaultColors = new QCheckBox(i18n("Use default colors"), getColorBox(), "Default Colors");
	CHECK_PTR(m_defaultColors);
	str = i18n("Check the box if the component have to use the default (common) color scheme.");
	addToolTip(str, m_defaultColors);
	addWhatsThis(str, m_defaultColors);
	
	// Color Group - Color button group
	QGrid * colButGrp = new QGrid(2, /*QGrid::Horizontal,*/ getColorBox());
	CHECK_PTR(colButGrp);
	
	// Color Group - Color button group - Foreground color
	label = new QLabel(i18n("Foreground Color:"), colButGrp);
	CHECK_PTR(label);
	m_foreGround = new KColorButton(colButGrp);
	CHECK_PTR(m_foreGround);
	str = i18n("Select the foreground color.");
	addToolTip(str, m_foreGround, label);
	addWhatsThis(str, m_foreGround, label);

	// Color Group - Color button group - Background color
	label = new QLabel(i18n("Background Color:"), colButGrp);
	CHECK_PTR(label);
	m_backGround = new KColorButton(colButGrp);
	CHECK_PTR(m_backGround);
	str = i18n("Select the background color.");
	addToolTip(str, m_backGround, label);
	addWhatsThis(str, m_backGround, label);

	
	// Frame Group	
	m_frameGroup = new QVGroupBox(i18n("Frame:"), getVBox(), "Style Frame Box");
	CHECK_PTR(m_frameGroup);
	
	// Frame Group - Frame ena
	m_enaFrame = new QCheckBox(i18n("Enable frame in user interface"), getFrameBox(), "Frame Ena");
	CHECK_PTR(m_enaFrame);
	str = i18n("Check the box if component have to display a frame.");
	addToolTip(str, m_enaFrame);
	addWhatsThis(str, m_enaFrame);
	
	
  //######## Setup values
  //*** color ***
  // Ena color box
	if (!getCompStyle()->isColorAdjustmentEnabled())
		getColorBox()->hide();
	// def color
	m_defaultColors->setChecked(getCompStyle()->isDefaultColorEnabled());
	// Fore color
	m_foreGround->setColor(getCompStyle()->getForegroundColor());
	m_foreGround->setDisabled(getCompStyle()->isDefaultColorEnabled());
	connect(m_defaultColors, SIGNAL(toggled(bool)), m_foreGround, SLOT(setDisabled(bool)));
	// Back color
	m_backGround->setColor(getCompStyle()->getBackgroundColor());
	m_backGround->setDisabled(getCompStyle()->isDefaultColorEnabled());
	connect(m_defaultColors, SIGNAL(toggled(bool)), m_backGround, SLOT(setDisabled(bool)));
	
  //*** frame ***
  // Ena frame box
	getFrameBox()->setEnabled(getCompStyle()->isFrameAdjustmentEnabled());
	// frame ena
	m_enaFrame->setChecked(getCompStyle()->isFrameEnabled());
		
	
	
	// Set main layout
	layout = new QGridLayout(this,2,1);
	layout->addWidget(getVBox(),0,0);
	layout->setRowStretch(1,1);
	

}

ComponentPropertyStyleWidget::~ComponentPropertyStyleWidget()
{
}

void ComponentPropertyStyleWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();

	if (getCompStyle()->isDefaultColorEnabled() != m_defaultColors->isChecked())
	{
		changeData();
		getCompStyle()->setDefaultColorEnabled(m_defaultColors->isChecked());
	}

	if (getCompStyle()->getForegroundColor() != m_foreGround->color())
	{
		changeData();
		getCompStyle()->setForegroundColor(m_foreGround->color());
	}

	if (getCompStyle()->getBackgroundColor() != m_backGround->color())
	{
		changeData();
		getCompStyle()->setBackgroundColor(m_backGround->color());
	}

	if (getCompStyle()->isFrameEnabled() != m_enaFrame->isChecked())
	{
		changeData();
		getCompStyle()->setFrameEnabled(m_enaFrame->isChecked());
	}

}

void ComponentPropertyStyleWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	if(getCompStyle()->getDefaultForegroundColor().isValid()
		|| getCompStyle()->getDefaultBackgroundColor().isValid())
	{
		m_defaultColors->setChecked(false);
	}
	else
	{
		m_defaultColors->setChecked(true);
	}
	
	m_foreGround->setColor(getCompStyle()->getDefaultForegroundColor());
	m_backGround->setColor(getCompStyle()->getDefaultBackgroundColor());
	
	m_enaFrame->setChecked(getCompStyle()->isFrameAdjustmentEnabled());
	
}
