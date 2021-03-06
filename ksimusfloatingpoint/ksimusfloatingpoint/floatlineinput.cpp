/***************************************************************************
                          floatlineinput.cpp  -  description
                             -------------------
    begin                : Sat Feb 2 2002
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
#include <float.h>

// QT-Includes
#include <qlabel.h>
#include <qgrid.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qspinbox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdoubleedit.h"
#include "floatlineinput.h"

// Forward declaration


namespace KSimLibFloatingPoint
{

#define DEFAULT_MIN_VALUE    -1000.0
#define DEFAULT_MAX_VALUE    1000.0

#define DEFAULT_DECIMALS			10
#define DEFAULT_TRACKING			true
#define DEFAULT_TYPE					'g'

#define FLAGS_TYPE_MASK				0x00FF
#define FLAGS_TRACKING				0x0100

#define DEFAULT_FLAGS					((DEFAULT_TRACKING ? FLAGS_TRACKING : 0) | DEFAULT_TYPE)


//#######################################################################
//#######################################################################

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatLineInput(container, ci);
}

const ComponentInfo FloatLineInputInfo(I18N_NOOP("Floating Point Line Edit Input"),
                                       I18N_NOOP("Floating Point/Input/Line Edit"),
                                       QString::null,
                                       VA_SHEET_AND_USER,
                                       create,
                                       QString::null,
                                       "component-float-input-line-edit");


//############################################################################
//############################################################################


FloatLineInput::FloatLineInput(CompContainer * container, const ComponentInfo * ci)
	: FloatStyleRange1Out(container, ci),
		m_decimals(DEFAULT_DECIMALS),
		m_flags(DEFAULT_FLAGS)
{
	
//	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);
	
	setMinValue(DEFAULT_MIN_VALUE);
	setMaxValue(DEFAULT_MAX_VALUE);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatLineInputView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new FloatLineInputView(this, USER_VIEW);
	}
//	getAction().disable(KSimAction::UPDATEOUTPUT);
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
}


/** save component properties */
void FloatLineInput::save(KSimData & file) const
{
	FloatStyleRange1Out::save(file);
	
	if (getDecimals() != DEFAULT_DECIMALS)
	{
		file.writeEntry("Decimals", getDecimals());
	}
	if (isTrackingEnabled() != DEFAULT_TRACKING)
	{
		file.writeEntry("Value Tracking", isTrackingEnabled());
	}
	if (getConversionType() != DEFAULT_TYPE)
	{
		file.writeEntry("Conversion Type", (int)getConversionType());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool FloatLineInput::load(KSimData & file, bool copyLoad)
{
	setDecimals( file.readNumEntry("Decimals", DEFAULT_DECIMALS) );
	setTrackingEnabled( file.readBoolEntry("Value Tracking", DEFAULT_TRACKING) );
	setConversionType( file.readNumEntry("Conversion Type", DEFAULT_TYPE) );
	
	return FloatStyleRange1Out::load(file, copyLoad);
}

void FloatLineInput::reset()
{
	FloatStyleRange1Out::reset();
	
	emit valueChanged(getValue());
	updateOutput();
}

void FloatLineInput::newValueTracking(double i)
{
	if (i != getValue())
	{
		setValue(i);
	}
}

void FloatLineInput::newValue(double i)
{
	setValue(i);
	emit valueChanged(i);
}

void FloatLineInput::setTrackingEnabled(bool tracking)
{
	if (tracking)
	{
		m_flags |= FLAGS_TRACKING;
	}
	else
	{
		m_flags &= ~FLAGS_TRACKING;
	}
	emit trackingChanged(tracking);
}

bool FloatLineInput::isTrackingEnabled() const
{
	return m_flags & FLAGS_TRACKING;
}
	
int FloatLineInput::getDecimals() const
{
	return m_decimals;
}
	
void FloatLineInput::setDecimals(int decimals)
{
	m_decimals = decimals;
	emit decimalsChanged(decimals);
}


void FloatLineInput::setConversionType(char type)
{
	switch(type)
	{
		case 'e':
		case 'f':
		case 'g':
		case 'E':
		case 'F':
		case 'G':
			m_flags = (m_flags & ~FLAGS_TYPE_MASK) | type;
			break;
			
		default:
			m_flags = (m_flags & ~FLAGS_TYPE_MASK) | 'g';
			KSIMDEBUG_VAR("Unknown ",type);
			break;
	}
	emit conversionTypeChanged(getConversionType());
}

char FloatLineInput::getConversionType() const
{
	return m_flags & FLAGS_TYPE_MASK;
}

ComponentPropertyBaseWidget * FloatLineInput::createGeneralProperty(Component * comp, QWidget *parent)
{
	FloatLineInputPropertyGeneralWidget * wid;
	wid = new FloatLineInputPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//############################################################################
//############################################################################

FloatLineInputView::FloatLineInputView(FloatLineInput * comp, eViewType viewType)
	: FloatStyleRange1OutView(comp,viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->setFixedSize(true);
		setPlace(QRect(0, 0, 7*gridX, 3*gridY));
		setMinSize(5*gridX, 3*gridY);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
		enableConnectorSpacingLeft(false);
//		enableConnectorSpacingRight(false);
	}
	else
	{
		setPlace(QRect(0, 0, 6*gridX, 3*gridY));
		setMinSize(4*gridX, 3*gridY);
	}	
}


QWidget * FloatLineInputView::createCompViewWidget(QWidget * parent)
{
	FloatLineInputWidgetView * wid = new FloatLineInputWidgetView(this, parent,"Line Edit");
	
	/* Specific signals */
	// Number changed (Component->Edit)
	connect(getComponent(), SIGNAL(valueChanged(double)), wid->m_edit, SLOT(setValue(double)));
	// Number changed (final) (Edit->Component)
	connect(wid->m_edit, SIGNAL(valueChanged(double)), getComponent(), SLOT(newValue(double)));
	// Number changed (tracked) (Edit->Component)
	connect(wid->m_edit, SIGNAL(valueChangedTracking(double)), getComponent(), SLOT(newValueTracking(double)));
	// min value changed (Component->Widget)
	connect(getComponent(), SIGNAL(minChanged(double)), wid, SLOT(setMinValue(double)));
	// max value changed (Component->Widget)
	connect(getComponent(), SIGNAL(maxChanged(double)), wid, SLOT(setMaxValue(double)));
	// decimals changed (Component->Widget)
	connect(getComponent(), SIGNAL(decimalsChanged(int)), wid, SLOT(setDecimals(int)));
	// tracking changed (Component->Widget)
	connect(getComponent(), SIGNAL(trackingChanged(bool)), wid, SLOT(setTrackingEnabled(bool)));
	// tracking changed (Component->Widget)
	connect(getComponent(), SIGNAL(conversionTypeChanged(char)), wid, SLOT(setConversionType(char)));
	
	return wid;
}

/*FloatInputSliderView::~FloatInputSliderView()
{
} */

//##########################################################################################
//##########################################################################################

FloatLineInputWidgetView::FloatLineInputWidgetView(FloatLineInputView * cv, QWidget *parent, const char *name)
	:	CompViewHBox(cv,parent,name)
{
	m_edit = new KSimDoubleEdit(this);
	CHECK_PTR(m_edit);
	m_edit->setBottom(getComponent()->getMinValue());
	m_edit->setTop(getComponent()->getMaxValue());
	m_edit->setTrackingEnabled(getComponent()->isTrackingEnabled());
	m_edit->setDecimals(getComponent()->getDecimals());
	m_edit->setConversionType(getComponent()->getConversionType());
	
	m_edit->setValue(getComponent()->getValue());
}

/*FloatLineInputWidgetView::~FloatLineInputWidgetView()
{
} */

void FloatLineInputWidgetView::setMinValue(double d)
{
	m_edit->setBottom(d);
}

void FloatLineInputWidgetView::setMaxValue(double d)
{
	m_edit->setTop(d);
}

void FloatLineInputWidgetView::setTrackingEnabled(bool tracking)
{
	m_edit->setTrackingEnabled(tracking);
}

void FloatLineInputWidgetView::setDecimals(int decimals)
{
	m_edit->setDecimals(decimals);
}

void FloatLineInputWidgetView::setConversionType(char type)
{
	m_edit->setConversionType(type);
}

//###############################################################
//###############################################################

static int convertType2Idx(char type)
{
	if (type == 'f') return 0;
	if (type == 'e') return 1;
	return 2;
}

static char idx2ConvertType(int idx)
{
	if (idx == 0) return 'f';
	if (idx == 1) return 'e';
	return 'g';
}

FloatLineInputPropertyGeneralWidget::FloatLineInputPropertyGeneralWidget(FloatLineInput * comp, QWidget *parent, const char *name)
	:	FloatStyleRange1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip, precTip;
	
	m_convertTypeLabel = new QLabel(i18n("Notation: "), getGrid(), "m_convertTypeLabel");
	CHECK_PTR(m_convertTypeLabel);
	
	m_convertType = new QComboBox(getGrid(), "m_convertType");
	CHECK_PTR(m_convertType);
	m_convertType->insertItem(i18n("Fixed Point"), convertType2Idx('f'));
	m_convertType->insertItem(i18n("Exponential"), convertType2Idx('e'));
	m_convertType->insertItem(i18n("Automatic"), convertType2Idx('g'));
	tip = i18n("Sets the notation type of the input.");
	addToolTip(tip, m_convertType, m_convertTypeLabel);
	precTip = i18n("\nFixed Point:\n"
	               "The value is displayed in fixed point notation.\n"
	               "The 'Precision' gives the number of digits after the decimal point.\n"
	               "Exponential:\n"
	               "The value is displayed in exponential notation.\n"
	               "The 'Precision' gives the number of digits after the decimal point.\n"
	               "Automatic:\n"
	               "The notation depends on the value and precision.\n"
	               "The value is displayed in exponent notation if the exponent is less than -4 or\n"
	               "greater than or equal to the 'Precision'; otherwise the fixed-point notation is used."
	               );
	addWhatsThis(tip + precTip, m_convertType, m_convertTypeLabel);
	
	
	m_decimalsLabel = new QLabel(i18n("Precision: "), getGrid(), "DecimalLabel");
	CHECK_PTR(m_decimalsLabel);
	
	m_decimals = new QSpinBox(0, 100, 1, getGrid(), "Decimals");
	CHECK_PTR(m_decimals);
	tip = i18n("Sets the 'Precision' of the input.");
	addToolTip(tip, m_decimals, m_decimalsLabel);
	addWhatsThis(tip + precTip, m_decimals, m_decimalsLabel);
	
	
	m_tracking = new QCheckBox(i18n("Tracking"), getGrid(), "Tracking");
	CHECK_PTR(m_tracking);
	tip = i18n("Enables the value tracking.\n"
	           "If enabled the output will be updated after each inserted digit.\n"
	           "If disabled the output will be updated after the user press return or select another input field.");
	addToolTip(tip, m_tracking);
	addWhatsThis(tip, m_tracking);
	
	
	// Setup value
	m_decimals->setValue(comp->getDecimals());
	m_tracking->setChecked(comp->isTrackingEnabled());
	m_convertType->setCurrentItem(convertType2Idx(comp->getConversionType()));
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void FloatLineInputPropertyGeneralWidget::acceptPressed()
{
	FloatStyleRange1OutPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getDecimals() != m_decimals->value())
	{
		changeData();
		getComponent()->setDecimals(m_decimals->value());
	}

	if (getComponent()->isTrackingEnabled() != m_tracking->isChecked())
	{
		changeData();
		getComponent()->setTrackingEnabled(m_tracking->isChecked());
	}

	char type = idx2ConvertType(m_convertType->currentItem());
	if (getComponent()->getConversionType() != type)
	{
		changeData();
		getComponent()->setConversionType(type);
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void FloatLineInputPropertyGeneralWidget::defaultPressed()
{
	FloatStyleRange1OutPropertyGeneralWidget::defaultPressed();

	m_decimals->setValue(DEFAULT_DECIMALS);
	m_tracking->setChecked(DEFAULT_TRACKING);
	m_convertType->setCurrentItem(convertType2Idx(DEFAULT_TYPE));
}


//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint

