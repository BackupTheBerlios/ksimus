/***************************************************************************
                          floatstyle1out.cpp  -  description
                             -------------------
    begin                : Fri Feb 1 2002
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


// KDE-Includes
#include <klocale.h>
//#include <kinstance.h>

// Project-Includes
#include "ksimus/ksimdoubleedit.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorfloatout.h"
#include "floatstyle1out.h"

// Forward declaration


namespace KSimLibFloatingPoint
{

#define DEFAULT_RESET_VALUE  0.0


//###############################################################
//###############################################################


FloatStyle1Out::FloatStyle1Out(CompContainer * container, const ComponentInfo * ci)
	: ComponentStyle(container, ci),
		m_resetValue(DEFAULT_RESET_VALUE)
{
	
	m_out = new ConnectorFloatOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("FloatingPoint-Connector", "Output"));
	CHECK_PTR(m_out);
	
}

/*FloatStyle1Out::~FloatStyle1Out()
{
} */

void FloatStyle1Out::setValue(double newValue)
{
	getOutputConnector()->setOutput(newValue);
};

double FloatStyle1Out::getValue() const
{
	return getOutputConnector()->getOutput();
};

void FloatStyle1Out::setResetValue(double resetValue)
{
	m_resetValue = resetValue;
}
	
double FloatStyle1Out::getResetValue() const
{
	return m_resetValue;
};
	

void FloatStyle1Out::reset()
{
	ComponentStyle::reset();
	
	setValue(getResetValue());
}

/** save component properties */
void FloatStyle1Out::save(KSimData & file) const
{
	ComponentStyle::save(file);
	
	if (getResetValue() != DEFAULT_RESET_VALUE)
	{
		file.writeEntry("Reset Value", getResetValue());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool FloatStyle1Out::load(KSimData & file, bool copyLoad)
{
	setResetValue( file.readDoubleNumEntry("Reset Value", DEFAULT_RESET_VALUE) );
	
	return ComponentStyle::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * FloatStyle1Out::createGeneralProperty(QWidget *parent)
{
	FloatStyle1OutPropertyGeneralWidget * wid;
	wid = new FloatStyle1OutPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


FloatStyle1OutView::FloatStyle1OutView(FloatStyle1Out * comp, eViewType viewType)
	: CompViewSize(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
//	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);
	
		m_layout->getRight()->addStretch(2);
		m_layout->getRight()->addConnector(comp->getOutputConnector(),0);
		m_layout->getRight()->addStretch(2);
	
		m_layout->updateLayout();
	}
	else
	{
		m_layout = 0;
	}
}
/*FloatStyle1OutView::~FloatStyle1OutView()
{
}*/


//###############################################################
//###############################################################

FloatStyle1OutPropertyGeneralWidget::FloatStyle1OutPropertyGeneralWidget(FloatStyle1Out * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	m_resetValueLabel = new QLabel(i18n("FloatingPoint", "Reset value: "), this, "ResetValueLabel");
	CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimDoubleEdit(this, "ResetValue");
	CHECK_PTR(m_resetValue);
	QString tip(i18n("FloatingPoint", "Change the reset value of the component here"));
	addToolTip(tip, m_resetValue, m_resetValueLabel);
	addWhatsThis(tip, m_resetValue, m_resetValueLabel);
	
	
	// Setup value
	m_resetValue->setValue(comp->getResetValue());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void FloatStyle1OutPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	double reset = m_resetValue->value();
	if (getFloatStyle1Out()->getResetValue() != reset)
	{
		changeData();
		getFloatStyle1Out()->setResetValue(reset);
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void FloatStyle1OutPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(DEFAULT_RESET_VALUE);
}


//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint


