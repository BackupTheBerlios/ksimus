/***************************************************************************
                          float1out.cpp  -  description
                             -------------------
    begin                : Tue Jan 2 2002
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


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdoubleedit.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorfloatout.h"

#include "float1out.h"

// Forward declaration


namespace KSimLibFloatingPoint
{

#define DEFAULT_RESET_VALUE  0.0


//###############################################################
//###############################################################


Float1Out::Float1Out(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_value(DEFAULT_RESET_VALUE),
		m_resetValue(DEFAULT_RESET_VALUE)
{
	
	m_out = new ConnectorFloatOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("FloatingPoint-Connector", "Output"));
	CHECK_PTR(m_out);
	
}

/*Float1Out::~Float1Out()
{
} */

void Float1Out::updateOutput()
{
	Component::updateOutput();
	getOutputConnector()->setOutput(getValue());
}

void Float1Out::setResetValue(double resetValue)
{
	m_resetValue = resetValue;
	reset(); // Update values
}
	
double Float1Out::getResetValue() const
{
	return m_resetValue;
};
	

void Float1Out::reset()
{
	Component::reset();
	
	setValue(getResetValue());
	getOutputConnector()->setOutput(getValue());
}

/** save component properties */
void Float1Out::save(KSimData & file) const
{
	Component::save(file);
	
	if (getResetValue() != DEFAULT_RESET_VALUE)
	{
		file.writeEntry("Reset Value", getResetValue());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Float1Out::load(KSimData & file, bool copyLoad)
{
	setResetValue( file.readDoubleNumEntry("Reset Value", DEFAULT_RESET_VALUE) );
	
	return Component::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * Float1Out::createGeneralProperty(Component * comp, QWidget *parent)
{
	Float1OutPropertyGeneralWidget * wid;
	wid = new Float1OutPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


Float1OutView::Float1OutView(Float1Out * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
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
/*Float1OutView::~Float1OutView()
{
}*/

void Float1OutView::draw(QPainter * p)
{
	drawFrame(p);	
	CompView::draw(p);
}


//###############################################################
//###############################################################

Float1OutPropertyGeneralWidget::Float1OutPropertyGeneralWidget(Float1Out * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	m_resetValueLabel = new QLabel(i18n("FloatingPoint", "Reset value: "), getGrid(), "ResetValueLabel");
	CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimDoubleEdit(getGrid(), "ResetValue");
	CHECK_PTR(m_resetValue);
	QString tip(i18n("FloatingPoint", "Change the reset value of the component here."));
	addToolTip(tip, m_resetValue, m_resetValueLabel);
	addWhatsThis(tip, m_resetValue, m_resetValueLabel);
	
	
	// Setup value
	m_resetValue->setValue(comp->getResetValue());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void Float1OutPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	double reset = m_resetValue->value();
	if (getFloat1Out()->getResetValue() != reset)
	{
		changeData();
		getFloat1Out()->setResetValue(reset);
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void Float1OutPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(DEFAULT_RESET_VALUE);
}


//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint


