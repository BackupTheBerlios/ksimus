/***************************************************************************
                          integerstyle1out.cpp  -  description
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

// QT-Includes
#include <qlabel.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "integerstyle1out.h"

// Forward declaration


namespace KSimLibInteger
{

//###############################################################
//###############################################################

const KSimBaseInt IntegerStyle1Out::DefaultResetValue(0, KSimBaseInt::Decimal);

IntegerStyle1Out::IntegerStyle1Out(CompContainer * container, const ComponentInfo * ci)
	: ComponentStyle(container, ci),
		m_resetValue(DefaultResetValue),
		m_initResetValue(DefaultResetValue)
{
	
	m_out = new ConnectorIntegerOut(this,
	                                QString::fromLatin1("Output"),
	                                i18n("Integer-Connector", "Output"));
	CHECK_PTR(m_out);
	
}

/*IntegerStyle1Out::~IntegerStyle1Out()
{
} */

void IntegerStyle1Out::setValue(int newValue)
{
	getOutputConnector()->setOutput(newValue);
};

int IntegerStyle1Out::getValue() const
{
	return getOutputConnector()->getOutput();
};

void IntegerStyle1Out::setResetValue(const KSimBaseInt & resetValue)
{
	m_resetValue = resetValue;
	setValue(resetValue.value()); // Set output
	emit resetValueChanged(m_resetValue);
}

void IntegerStyle1Out::setInitResetValue(const KSimBaseInt & resetValue)
{
	m_initResetValue = resetValue;
	setResetValue(resetValue);
}

void IntegerStyle1Out::reset()
{
	ComponentStyle::reset();
	
	setValue(getResetValue().value());
}

/** save component properties */
void IntegerStyle1Out::save(KSimData & file) const
{
	ComponentStyle::save(file);
	
	if (m_resetValue != getInitResetValue())
	{
		m_resetValue.save(file, "Reset Value");
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool IntegerStyle1Out::load(KSimData & file, bool copyLoad)
{
	KSimBaseInt loadResetValue;
	if (!loadResetValue.load(file, "Reset Value"))
	{
		loadResetValue = getInitResetValue();
	}
	if (loadResetValue != getResetValue())
	{
		setResetValue(loadResetValue);
	}
	
	return ComponentStyle::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * IntegerStyle1Out::createGeneralProperty(QWidget *parent)
{
	IntegerStyle1OutPropertyGeneralWidget * wid;
	wid = new IntegerStyle1OutPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


IntegerStyle1OutView::IntegerStyle1OutView(IntegerStyle1Out * comp, eViewType viewType)
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
/*IntegerStyle1OutView::~IntegerStyle1OutView()
{
}*/


//###############################################################
//###############################################################

IntegerStyle1OutPropertyGeneralWidget::IntegerStyle1OutPropertyGeneralWidget(IntegerStyle1Out * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	m_resetValueLabel = new QLabel(i18n("Integer", "Reset value: "), this, "ResetValueLabel");
	CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimBaseIntEdit(this, "ResetValue");
	CHECK_PTR(m_resetValue);
	QString tip(i18n("Integer", "Change the reset value of the component here"));
	addToolTip(tip, m_resetValue, m_resetValueLabel);
	addWhatsThis(tip, m_resetValue, m_resetValueLabel);
	m_resetValueLabel->setBuddy(m_resetValue);
	
	
	// Setup value
	m_resetValue->setValue(comp->getResetValue());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void IntegerStyle1OutPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	KSimBaseInt reset(m_resetValue->value());
	if (getIntegerStyle1Out()->getResetValue() != reset)
	{
		changeData();
		getIntegerStyle1Out()->setResetValue(reset);
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void IntegerStyle1OutPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(getIntegerStyle1Out()->getInitResetValue());
}


//###############################################################
//###############################################################


};  //namespace KSimLibInteger


