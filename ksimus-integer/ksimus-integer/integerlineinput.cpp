/***************************************************************************
                          integerlineinput.cpp  -  description
                             -------------------
    begin                : Mon Jul 14 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include <limits.h>

// QT-Includes
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/componentlayout.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbaseintedit.h"
#include "integerlineinput.h"

// Forward declaration


namespace KSimLibInteger
{

Component * IntegerLineInput::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLineInput(container, ci);
}

const ComponentInfo * IntegerLineInput::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Line Edit Input"),
	                                QString::fromLatin1("Integer/Input/Line Edit"),
	                                i18n("Component", "Integer/Input/Line Edit"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-integer-input-line-edit")*/);
	return &Info;
}


//############################################################################
//############################################################################


IntegerLineInput::IntegerLineInput(CompContainer * container, const ComponentInfo * ci)
	: IntegerStyleRange1Out(container, ci)
{
	
//	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerLineInputView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new IntegerLineInputView(this, USER_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);

	setTrackingEnabled(true, true);
}


/** save component properties */
void IntegerLineInput::save(KSimData & file) const
{
	IntegerStyleRange1Out::save(file);
	
	if (isTrackingEnabled() != isTrackingInitEnabled())
	{
		file.writeEntry("Value Tracking", isTrackingEnabled());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool IntegerLineInput::load(KSimData & file, bool copyLoad)
{
	setTrackingEnabled( file.readBoolEntry("Value Tracking", isTrackingInitEnabled()) );
	
	return IntegerStyleRange1Out::load(file, copyLoad);
}

void IntegerLineInput::reset()
{
	IntegerStyleRange1Out::reset();
	
	emit valueChanged(getResetValue());
}

void IntegerLineInput::newValueTracking(const KSimBaseInt & i)
{
	if (i != getValue())
	{
		setValue(i.value());
		executeNext();
	}
}

void IntegerLineInput::newValue(const KSimBaseInt & i)
{
	KSIMDEBUG_VAR("IntegerLineInput::newValue", i.text());
	setValue(i.value());
	emit valueChanged(i);
	executeNext();
}

void IntegerLineInput::setTrackingEnabled(bool tracking, bool init)
{
	if (init)
	{
		m_flags.trackingInit = tracking;
	}
	m_flags.tracking = tracking;
	emit trackingChanged(tracking);
}

bool IntegerLineInput::isTrackingEnabled() const
{
	return m_flags.tracking;
}

bool IntegerLineInput::isTrackingInitEnabled() const
{
	return m_flags.trackingInit;
}

ComponentPropertyBaseWidget * IntegerLineInput::createGeneralProperty(QWidget *parent)
{
	IntegerLineInputPropertyGeneralWidget * wid;
	wid = new IntegerLineInputPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//############################################################################
//############################################################################

IntegerLineInputView::IntegerLineInputView(IntegerLineInput * comp, eViewType viewType)
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


QWidget * IntegerLineInputView::createCompViewWidget(QWidget * parent)
{
	IntegerLineInputWidgetView * wid = new IntegerLineInputWidgetView(this, parent,"Line Edit");
	
	/* Specific signals */
	// Number changed (Component->Edit)
	connect(getIntegerLineInput(), SIGNAL(valueChanged(const KSimBaseInt &)), wid->m_edit, SLOT(setValue(const KSimBaseInt &)));
	// Reset Number changed (Component->Edit)
	connect(getIntegerLineInput(), SIGNAL(resetValueChanged(const KSimBaseInt &)), wid->m_edit, SLOT(setValue(const KSimBaseInt &)));
	// Number changed (final) (Edit->Component)
	connect(wid->m_edit, SIGNAL(valueChanged(const KSimBaseInt &)), getIntegerLineInput(), SLOT(newValue(const KSimBaseInt &)));
	// Number changed (tracked) (Edit->Component)
	connect(wid->m_edit, SIGNAL(valueChangedTracking(const KSimBaseInt &)), getIntegerLineInput(), SLOT(newValueTracking(const KSimBaseInt &)));
	// min value changed (Component->Widget)
	connect(getIntegerLineInput(), SIGNAL(minValueChanged(int)), wid, SLOT(setMinValue(int)));
	// max value changed (Component->Widget)
	connect(getIntegerLineInput(), SIGNAL(maxValueChanged(int)), wid, SLOT(setMaxValue(int)));
	// tracking changed (Component->Widget)
	connect(getIntegerLineInput(), SIGNAL(trackingChanged(bool)), wid, SLOT(setTrackingEnabled(bool)));
	
	return wid;
}

/*IntegerLineInputView::~IntegerLineInputView()
{
} */

//##########################################################################################
//##########################################################################################

IntegerLineInputWidgetView::IntegerLineInputWidgetView(IntegerLineInputView * cv, QWidget *parent, const char *name)
	:	CompViewHBox(cv,parent,name)
{
	m_edit = new KSimBaseIntEdit(this);
	CHECK_PTR(m_edit);
	m_edit->setBottom(getIntegerLineInput()->getMinValue().value());
	m_edit->setTop(getIntegerLineInput()->getMaxValue().value());
	m_edit->setTrackingEnabled(getIntegerLineInput()->isTrackingEnabled());
	
	m_edit->setValue(getIntegerLineInput()->getResetValue()); // Sets the correct base
	m_edit->setValue(getIntegerLineInput()->getValue());
}

/*IntegerLineInputWidgetView::~IntegerLineInputWidgetView()
{
} */

void IntegerLineInputWidgetView::setMinValue(int i)
{
	m_edit->setBottom(i);
}

void IntegerLineInputWidgetView::setMaxValue(int i)
{
	m_edit->setTop(i);
}

void IntegerLineInputWidgetView::setTrackingEnabled(bool tracking)
{
	m_edit->setTrackingEnabled(tracking);
}

//###############################################################
//###############################################################

IntegerLineInputPropertyGeneralWidget::IntegerLineInputPropertyGeneralWidget(IntegerLineInput * comp, QWidget *parent, const char *name)
	:	IntegerStyleRange1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	

	m_tracking = new QCheckBox(i18n("Integer", "Value tracking"), this, "m_tracking");
	CHECK_PTR(m_tracking);
	tip = i18n("Enables the value tracking.\n"
	           "If enabled the output will be updated after each inserted digit.\n"
	           "If disabled the output will be updated after the user press return or select another input field.");
	addToolTip(tip, m_tracking);
	addWhatsThis(tip, m_tracking);
	
	
	// Setup value
	m_tracking->setChecked(comp->isTrackingEnabled());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void IntegerLineInputPropertyGeneralWidget::acceptPressed()
{
	IntegerStyleRange1OutPropertyGeneralWidget::acceptPressed();
	
	if (getIntegerLineInput()->isTrackingEnabled() != m_tracking->isChecked())
	{
		changeData();
		getIntegerLineInput()->setTrackingEnabled(m_tracking->isChecked());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void IntegerLineInputPropertyGeneralWidget::defaultPressed()
{
	IntegerStyleRange1OutPropertyGeneralWidget::defaultPressed();

	m_tracking->setChecked(getIntegerLineInput()->isTrackingInitEnabled());
}

//###############################################################
//###############################################################


};  //namespace KSimLibInteger

#include "integerlineinput.moc"
