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
#include <qpushbutton.h>

// KDE-Includes
#include <kdialog.h>
#include <klocale.h>
#include <kcolorbtn.h>
#include <kfontdialog.h>
#include <kapp.h>

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
	m_defaultColors = new QCheckBox(i18n("Use system colors"), getColorBox(), "Default Colors");
	CHECK_PTR(m_defaultColors);
	str = i18n("Check the box if the component have to use the color scheme of your system.");
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
	
	
	// Font Group	
	m_fontGroup = new QVGroupBox(i18n("Font:"), getVBox(), "Style Font Box");
	CHECK_PTR(m_fontGroup);
	
	// Font Group - Default Font
	m_defaultFont = new QCheckBox(i18n("Use default font"), getFontBox(), "Font default");
	CHECK_PTR(m_defaultFont);
	str = i18n("Check the box if component have to use the default (the application) font.");
	addToolTip(str, m_defaultFont);
	addWhatsThis(str, m_defaultFont);
	
	// Font Group - Font Dialog Button
  m_fontButton = new QPushButton(i18n("Select font"), getFontBox(), "Font Button");
	CHECK_PTR(m_fontButton);
	
	m_exampleFont = new QLabel(getFontBox(), "Font Example");
	CHECK_PTR(m_fontButton);
	


  //######## Setup values
  //*** color ***
  // Ena color box
	if (!getCompStyle()->isColorAdjustmentEnabled())
		getColorBox()->hide();
	// def color
	bool useDefaultColor = !(getCompStyle()->getForegroundColor().isValid()
	                        && getCompStyle()->getBackgroundColor().isValid());
	
	m_defaultColors->setChecked(useDefaultColor);
	// Fore color
	if (getCompStyle()->getForegroundColor().isValid())
	{
		m_foreGround->setColor(getCompStyle()->getForegroundColor());
	}
	else
	{
		m_foreGround->setColor(palette().color(QPalette::Active, QColorGroup::Foreground));
	}
	m_foreGround->setDisabled(useDefaultColor);
	connect(m_defaultColors, SIGNAL(toggled(bool)), m_foreGround, SLOT(setDisabled(bool)));
	// Back color
	if (getCompStyle()->getBackgroundColor().isValid())
	{
		m_backGround->setColor(getCompStyle()->getBackgroundColor());
	}
	else
	{
		m_backGround->setColor(palette().color(QPalette::Active, QColorGroup::Background));
	}
	m_backGround->setDisabled(useDefaultColor);
	connect(m_defaultColors, SIGNAL(toggled(bool)), m_backGround, SLOT(setDisabled(bool)));
	
	
	
  //*** frame ***
  // Ena frame box
	if (!getCompStyle()->isFrameAdjustmentEnabled())
		getFrameBox()->hide();
	// frame ena
	m_enaFrame->setChecked(getCompStyle()->isFrameEnabled());
		

  //*** font ***
  // Ena color box
	if (!getCompStyle()->isFontAdjustmentEnabled())
		getFontBox()->hide();
	m_defaultFont->setChecked(getCompStyle()->isDefaultFontEnabled());
	m_fontButton->setDisabled(getCompStyle()->isDefaultFontEnabled());
	connect(m_defaultFont, SIGNAL(toggled(bool)), m_fontButton, SLOT(setDisabled(bool)));
	connect(m_fontButton, SIGNAL(clicked()), this, SLOT(slotFontDialog()));
	m_font = getCompStyle()->getFont();
	m_exampleFont->setFont(m_font);
	m_exampleFont->setText(m_font.family());
	connect(m_defaultFont, SIGNAL(toggled(bool)), m_exampleFont, SLOT(setDisabled(bool)));
		
	
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

	QColor foreColor = m_foreGround->color();
	QColor backColor = m_backGround->color();

	if (m_defaultColors->isChecked())
	{
		foreColor = QColor();
		backColor = QColor();
	}
	
	if (getCompStyle()->getForegroundColor() != foreColor)
	{
		changeData();
		getCompStyle()->setForegroundColor(foreColor);
	}

	if (getCompStyle()->getBackgroundColor() != backColor)
	{
		changeData();
		getCompStyle()->setBackgroundColor(backColor);
	}
	
	if (getCompStyle()->isFrameEnabled() != m_enaFrame->isChecked())
	{
		changeData();
		getCompStyle()->setFrameEnabled(m_enaFrame->isChecked());
	}

	
	if (getCompStyle()->getFont() != m_font)
	{
		changeData();
		getCompStyle()->setFont(m_font);
	}
	
	// First set font and then set
	if (getCompStyle()->isDefaultFontEnabled() != m_defaultFont->isChecked())
	{
		changeData();
		getCompStyle()->setDefaultFontEnabled(m_defaultFont->isChecked());
	}
}

void ComponentPropertyStyleWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	bool useDefaultColor = !(getCompStyle()->getDefaultForegroundColor().isValid()
	                        && getCompStyle()->getDefaultBackgroundColor().isValid());
	
	m_defaultColors->setChecked(useDefaultColor);
	// Fore color
	if (getCompStyle()->getDefaultForegroundColor().isValid())
	{
		m_foreGround->setColor(getCompStyle()->getDefaultForegroundColor());
	}
	else
	{
		m_foreGround->setColor(palette().color(QPalette::Active, QColorGroup::Foreground));
	}
	m_foreGround->setDisabled(useDefaultColor);
	// Back color
	if (getCompStyle()->getDefaultBackgroundColor().isValid())
	{
		m_backGround->setColor(getCompStyle()->getDefaultBackgroundColor());
	}
	else
	{
		m_backGround->setColor(palette().color(QPalette::Active, QColorGroup::Background));
	}
	m_backGround->setDisabled(useDefaultColor);
	
	
	
	m_enaFrame->setChecked(getCompStyle()->isFrameAdjustmentEnabled());

	m_defaultFont->setChecked(getCompStyle()->isFontAdjustmentEnabled());
	m_font = KApplication::font();
	m_exampleFont->setFont(m_font);
	m_exampleFont->setText(m_font.family());
	
}

void ComponentPropertyStyleWidget::slotFontDialog()
{
	QFont newFont(m_font);
	
	int result = KFontDialog::getFont(newFont);
	if(result == KFontDialog::Accepted)
	{
		m_font = newFont;
		m_exampleFont->setFont(m_font);
		m_exampleFont->setText(m_font.family());
	}
}
