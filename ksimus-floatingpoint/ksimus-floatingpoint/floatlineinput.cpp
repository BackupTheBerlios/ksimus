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
#include <qcheckbox.h>
#include <qcombobox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/connectorfloatout.h"
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

#define DEFAULT_DECIMALS     10
#define DEFAULT_TRACKING     true
#define DEFAULT_TYPE         'g'

#define FLAGS_TYPE_MASK      0x00FF
#define FLAGS_TRACKING       0x0100

#define DEFAULT_FLAGS        ((DEFAULT_TRACKING ? FLAGS_TRACKING : 0) | DEFAULT_TYPE)


//#######################################################################
//#######################################################################

Component * FloatLineInput::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatLineInput(container, ci);
}

const ComponentInfo * FloatLineInput::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Line Edit Input"),
	                                QString::fromLatin1("Floating Point/Input/Line Edit"),
	                                i18n("Component", "Floating Point/Input/Line Edit"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-input-line-edit"));
	return &Info;
}


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
}

void FloatLineInput::newValueTracking(double i)
{
	if (i != getValue())
	{
		setValue(i);
		executeNext();
	}
}

void FloatLineInput::newValue(double i)
{
	setValue(i);
	emit valueChanged(i);
	executeNext();
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

ComponentPropertyBaseWidget * FloatLineInput::createGeneralProperty(QWidget *parent)
{
	FloatLineInputPropertyGeneralWidget * wid;
	wid = new FloatLineInputPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//############################################################################
//############################################################################

FloatLineInputView::FloatLineInputView(FloatLineInput * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	enableRotation(false);
	
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 7*gridX, 3*gridY));
		setMinSize(5*gridX, 3*gridY);

		ComponentLayoutFixed * layout = new ComponentLayoutFixed(this, false);
		CHECK_PTR(layout);
		
		ComponentLayoutBlock * block = new ComponentLayoutBlock(layout);
		CHECK_PTR(layout);
	
		block->getRight()->addStretch(2);
		block->getRight()->addConnector(comp->getOutputConnector(),0);
		block->getRight()->addStretch(2);
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
	connect(getLineInput(), SIGNAL(valueChanged(double)), wid->m_edit, SLOT(setValue(double)));
	// Number changed (final) (Edit->Component)
	connect(wid->m_edit, SIGNAL(valueChanged(double)), getComponent(), SLOT(newValue(double)));
	// Number changed (tracked) (Edit->Component)
	connect(wid->m_edit, SIGNAL(valueChangedTracking(double)), getComponent(), SLOT(newValueTracking(double)));
	// min value changed (Component->Widget)
	connect(getLineInput(), SIGNAL(minChanged(double)), wid, SLOT(setMinValue(double)));
	// max value changed (Component->Widget)
	connect(getLineInput(), SIGNAL(maxChanged(double)), wid, SLOT(setMaxValue(double)));
	// decimals changed (Component->Widget)
	connect(getLineInput(), SIGNAL(decimalsChanged(int)), wid, SLOT(setDecimals(int)));
	// tracking changed (Component->Widget)
	connect(getLineInput(), SIGNAL(trackingChanged(bool)), wid, SLOT(setTrackingEnabled(bool)));
	// tracking changed (Component->Widget)
	connect(getLineInput(), SIGNAL(conversionTypeChanged(char)), wid, SLOT(setConversionType(char)));
	
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

int FloatLineInputPropertyGeneralWidget::convertType2Idx(char type)
{
	switch(type)
	{
		case 'f': return 0; break;
		case 'e': return 1; break;
		default:  return 2; break;
	}
}

char FloatLineInputPropertyGeneralWidget::idx2ConvertType(int idx)
{
	switch(idx)
	{
		case 0:  return 'f'; break;
		case 1:  return 'e'; break;
		default: return 'g'; break;
	}
}

FloatLineInputPropertyGeneralWidget::FloatLineInputPropertyGeneralWidget(FloatLineInput * comp, QWidget *parent, const char *name)
	:	FloatStyleRange1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip, precTip;
	
	m_convertTypeLabel = new QLabel(i18n("FloatingPoint", "Notation: "), this, "m_convertTypeLabel");
	CHECK_PTR(m_convertTypeLabel);
	
	m_convertType = new QComboBox(this, "m_convertType");
	CHECK_PTR(m_convertType);
	m_convertType->insertItem(i18n("FloatingPoint", "Fixed Point"), convertType2Idx('f'));
	m_convertType->insertItem(i18n("FloatingPoint", "Exponential"), convertType2Idx('e'));
	m_convertType->insertItem(i18n("FloatingPoint", "Automatic"), convertType2Idx('g'));
	tip = i18n("FloatingPoint", "Sets the notation type of the input.");
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
	
	
	m_decimalsLabel = new QLabel(i18n("FloatingPoint", "Precision: "), this, "DecimalLabel");
	CHECK_PTR(m_decimalsLabel);
	
	m_decimals = new KSimSpinBox(0, 100, 1, this, "Decimals");
	CHECK_PTR(m_decimals);
	tip = i18n("FloatingPoint", "Sets the 'Precision' of the input.");
	addToolTip(tip, m_decimals, m_decimalsLabel);
	addWhatsThis(tip + precTip, m_decimals, m_decimalsLabel);
	
	
	m_tracking = new QCheckBox(i18n("FloatingPoint", "Tracking"), this, "Tracking");
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

#undef DEFAULT_MIN_VALUE
#undef DEFAULT_MAX_VALUE
#undef DEFAULT_DECIMALS
#undef DEFAULT_TRACKING
#undef DEFAULT_TYPE
#undef FLAGS_TYPE_MASK
#undef FLAGS_TRACKING
#undef DEFAULT_FLAGS

//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint

#include "floatlineinput.moc"
