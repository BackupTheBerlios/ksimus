/***************************************************************************
                          watchitembooltristate.cpp  -  description
                             -------------------
    begin                : Thu Aug 15 2002
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
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qgrid.h>
#include <qhbox.h>
#include <qtextview.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "watchitembooltristate.h"
#include "wirepropertybooltristate.h"
#include "connectorbooltristate.h"
#include "connectorbase.h"
#include "propertywidget.h"
#include "wire.h"
#include "ksimdebug.h"

// Forward declaration


//############################################################################################
//############################################################################################

class WatchItemBoolTriStateBase::StatePropertyWidget : public QComboBox
{
public:

	StatePropertyWidget(QWidget * parent, const char * name);
	StatePropertyWidget(eStateProperty value, QWidget * parent, const char * name);

	void setValue(eStateProperty value);
	eStateProperty value() const;

private:
	void init();
};

WatchItemBoolTriStateBase::StatePropertyWidget::StatePropertyWidget(QWidget * parent, const char * name)
	:	QComboBox(parent,name)
{
	init();
}

WatchItemBoolTriStateBase::StatePropertyWidget::StatePropertyWidget(eStateProperty value, QWidget * parent, const char * name)
	:	QComboBox(parent,name)
{
	init();
	setValue(value);
}

void WatchItemBoolTriStateBase::StatePropertyWidget::init()
{
	QString s;

	s = i18n("Boolean TriState Watch", "Inactive");
	insertItem(s, (int)eInactive);

	s = i18n("Boolean TriState Watch", "False");
	insertItem(s, (int)eFalse);

	s = i18n("Boolean TriState Watch", "True");
	insertItem(s, (int)eTrue);

	s = i18n("Boolean TriState Watch", "Inactive or False");
	insertItem(s, (int)eInactiveOrFalse);

	s = i18n("Boolean TriState Watch", "Inactive or True");
	insertItem(s, (int)eInactiveOrTrue);

	s = i18n("Boolean TriState Watch", "False or True");
	insertItem(s, (int)eFalseOrTrue);

	s = i18n("Boolean TriState Watch", "Don't care");
	insertItem(s, (int)eDontCare);
}

void WatchItemBoolTriStateBase::StatePropertyWidget::setValue(WatchItemBoolTriStateBase::eStateProperty value)
{
	setCurrentItem((int)value);
}

WatchItemBoolTriStateBase::eStateProperty WatchItemBoolTriStateBase::StatePropertyWidget::value() const
{
	return (eStateProperty) currentItem();
}
	
//############################################################################################
//############################################################################################

class WatchItemBoolTriStateBase::ActivePropertyWidget : public QHBox
{
public:

	ActivePropertyWidget(QWidget * parent, const char * name);
	ActivePropertyWidget(ActiveProperty value, unsigned int maxValue, QWidget * parent, const char * name);

	void setValue(ActiveProperty value);
	void setMaxValue(unsigned int maxValue);
	ActiveProperty value() const;

private:
	void init();
	QComboBox * m_box;
	QSpinBox * m_spin;
};

WatchItemBoolTriStateBase::ActivePropertyWidget::ActivePropertyWidget(QWidget * parent, const char * name)
	:	QHBox(parent, name)
{
	init();
}

WatchItemBoolTriStateBase::ActivePropertyWidget::ActivePropertyWidget(ActiveProperty value, unsigned int maxValue, QWidget * parent, const char * name)
	:	QHBox(parent, name)
{
	init();
	setValue(value);
	setMaxValue(maxValue);
}

void WatchItemBoolTriStateBase::ActivePropertyWidget::init()
{
	QString s;

	m_box = new QComboBox(this, name());
	CHECK_PTR(m_box);

	m_box->insertItem(QString::fromLatin1("<"), (int)eLesser);

	m_box->insertItem(QString::fromLatin1("="), (int)eEqual);

	m_box->insertItem(QString::fromLatin1(">"), (int)eLarger);

	s = i18n("Boolean TriState Watch", "Don't care");
	m_box->insertItem(s, (int)eDontCareCount);

	m_spin = new QSpinBox(this, name());
	CHECK_PTR(m_spin);
	m_spin->setMinValue(0);
}

void WatchItemBoolTriStateBase::ActivePropertyWidget::setValue(WatchItemBoolTriStateBase::ActiveProperty value)
{
	m_box->setCurrentItem((int)value.getProperty());
	m_spin->setValue(value.getCount());
}

void WatchItemBoolTriStateBase::ActivePropertyWidget::setMaxValue(unsigned int maxValue)
{
	m_spin->setMaxValue(maxValue);
};

WatchItemBoolTriStateBase::ActiveProperty WatchItemBoolTriStateBase::ActivePropertyWidget::value() const
{
	return ActiveProperty((eActiveProperty) m_box->currentItem(), m_spin->value());
} 

//############################################################################################
//############################################################################################

#define DEFAULT_OUT_STATE_T1 (isConnectorWatch() ? eInactive : eDontCare)
#define DEFAULT_OUT_STATE_T0 (isConnectorWatch() ? eFalseOrTrue : eDontCare)

#define DEFAULT_IN_STATE_T1 (isConnectorWatch() ? eDontCare : eInactive)
#define DEFAULT_IN_STATE_T0 (isConnectorWatch() ? eDontCare : eFalseOrTrue)

#define DEFAULT_IN_ACTIVE_T1 ActiveProperty(eDontCareCount, 1)
#define DEFAULT_IN_ACTIVE_T0 ActiveProperty(eDontCareCount, 1)

#define HIDE_IF_WIRE(widget) { if (isWirePropertyWatch()) widget->hide(); }


WatchItemBoolTriStateBase::WatchItemBoolTriStateBase(ConnectorBase * connector, bool inOnly)
	:	WatchItemBase(connector)
{
	init(inOnly);
}

WatchItemBoolTriStateBase::WatchItemBoolTriStateBase(WireProperty * wireProperty, bool inOnly)
	:	WatchItemBase(wireProperty)
{
	init(inOnly);
}

WatchItemBoolTriStateBase::~WatchItemBoolTriStateBase()
{
}

void WatchItemBoolTriStateBase::init(bool inOnly)
{
	m_traceInBuffer.fill(WireStateBoolTriState(), getTraceSize());
	m_traceOutBuffer.fill(WireStateBoolTriState(), getTraceSize());

	m_triggerOutStateT1 = DEFAULT_OUT_STATE_T1;
	m_triggerOutStateT0 = DEFAULT_OUT_STATE_T0;
	m_triggerOutStateT1Widget = (StatePropertyWidget*) 0;
	m_triggerOutStateT0Widget = (StatePropertyWidget*) 0;

	m_triggerInStateT1 = DEFAULT_IN_STATE_T1;
	m_triggerInStateT0 = DEFAULT_IN_STATE_T0;
	m_triggerInStateT1Widget = (StatePropertyWidget*) 0;
	m_triggerInStateT0Widget = (StatePropertyWidget*) 0;
	m_triggerInActiveT1 = DEFAULT_IN_ACTIVE_T1;
	m_triggerInActiveT0 = DEFAULT_IN_ACTIVE_T0;
	m_triggerInActiveT1Widget = (ActivePropertyWidget*)0;
	m_triggerInActiveT0Widget = (ActivePropertyWidget*)0;

	m_flags.inOnly = inOnly;
	m_flags.detailed = 0;

}

unsigned int WatchItemBoolTriStateBase::getMaxConnectors() const
{
	Wire * wire = (Wire*) 0;
	if (isConnectorWatch())
	{
		wire = getConnector()->getWire();
	}
	else
	{
		wire = getWireProperty()->getWire();
	}
	if (wire == (Wire *) 0)
	{
		return 0;
	}
	return wire->getConnList()->count();
}

/** Resets the item. */
void WatchItemBoolTriStateBase::reset()
{
	WatchItemBase::reset();
	m_traceInBuffer.fill(WireStateBoolTriState());
	m_traceOutBuffer.fill(WireStateBoolTriState());
}


void WatchItemBoolTriStateBase::setTraceSize(unsigned int size)
{
	WatchItemBase::setTraceSize(size);

	m_traceInBuffer.resize(getTraceSize());
	m_traceInBuffer.fill(WireStateBoolTriState());

	m_traceOutBuffer.resize(getTraceSize());
	m_traceOutBuffer.fill(WireStateBoolTriState());
}

QString WatchItemBoolTriStateBase::getDataText(unsigned int index)
{
	static QString str18n(i18n("Boolean TriState Watch", "%1 (Output: %2)"));
	unsigned int sel = m_flags.inOnly + m_flags.detailed * 2;
	
	switch(sel)
	{
		// m_flags.inOnly = 0;  m_flags.detailed = 0;
		case 0:
			return str18n.arg(m_traceInBuffer[index].getText())
			             .arg(m_traceOutBuffer[index].getText());
	
		default:
			KSIMDEBUG_VAR("FIXME: Unknown m_flags combination", sel);
		// m_flags.inOnly = 1;  m_flags.detailed = 0;
		case 1:
			return m_traceInBuffer[index].getText();
	
		// m_flags.inOnly = 0;  m_flags.detailed = 1;
		case 2:
			return str18n.arg(m_traceInBuffer[index].getDetailedText())
			             .arg(m_traceOutBuffer[index].getText());

		// m_flags.inOnly = 1;  m_flags.detailed = 1;
		case 3:
			return m_traceInBuffer[index].getDetailedText();
	}
}

bool WatchItemBoolTriStateBase::testState(WatchItemBoolTriStateBase::eStateProperty property, const WireStateBoolTriState & state)
{
	bool res = true;
	
	switch(property)
	{
		case eInactive:
			res = state.isInactive();
			break;
			
		case eFalse:
			res = state.isFalse();
			break;

		case eTrue:
			res = state.isTrue();
			break;

		case eInactiveOrFalse:
			res = !state.isTrue();
			break;

		case eInactiveOrTrue:
			res = !state.isFalse();
			break;
			
		case eFalseOrTrue:
			res = state.isActive();
			break;

		case eDontCare:
			break;
	}

	return res;
}

bool WatchItemBoolTriStateBase::testActive(WatchItemBoolTriStateBase::ActiveProperty & property, const WireStateBoolTriState & state)
{
	bool res = true;

	switch(property.getProperty())
	{
		case eLesser:
			res = (state.getTrue() + state.getFalse()) < property.getCount();
			break;
			
		case eEqual:
			res = (state.getTrue() + state.getFalse()) == property.getCount();
			break;
			
		case eLarger:
			res = (state.getTrue() + state.getFalse()) > property.getCount();
			break;
			
		case eDontCareCount:
			break;
	}

	return res;
}


bool WatchItemBoolTriStateBase::testBreak(unsigned int index, unsigned int stepCount)
{

	bool trigger = true;
	unsigned int previous = (index != 0) ? index-1 : getTraceSize()-1;

	// ####################    Output State     ####################
	// ####################         t - 1       ####################
	if (stepCount != 0)
	{
		trigger &= testState(m_triggerOutStateT1, m_traceOutBuffer[previous]);
	}
	else
	{
		trigger = false;
	}
	
	// ####################    Output State     ####################
	// ####################           t         ####################
	trigger &= testState(m_triggerOutStateT0, m_traceOutBuffer[index]);
	
	// ####################    Input State     ####################
	// ####################        t - 1       ####################
	if (stepCount != 0)
	{
		trigger &= testState(m_triggerInStateT1, m_traceInBuffer[previous]);
		trigger &= testActive(m_triggerInActiveT1, m_traceInBuffer[previous]);
	}
	else
	{
		trigger = false;
	}
	
	// ####################     Input State     ####################
	// ####################           t         ####################
	trigger &= testState(m_triggerInStateT0, m_traceInBuffer[index]);
	trigger &= testActive(m_triggerInActiveT0, m_traceInBuffer[index]);

	return trigger;
}

PropertyWidget * WatchItemBoolTriStateBase::createPropertyWidget(QWidget * parent)
{
	PropertyWidget * wid = WatchItemBase::createPropertyWidget(parent);
	CHECK_PTR(wid);

	m_detailed = new QCheckBox(i18n("Boolean TriState Watch", "Show detailed information"),
	                           wid, "m_detailed");
	CHECK_PTR(m_detailed);
	
	PropertyWidget::addToolTip(i18n("Boolean TriState Watch", "Shows the number of connectors with state true and false"),
	                           m_detailed);
	PropertyWidget::addWhatsThis(i18n("Boolean TriState Watch", "Shows the number of connectors with state true and false"),
	                             m_detailed);

	m_detailed->setChecked(m_flags.detailed);

	return wid;
}

void WatchItemBoolTriStateBase::propertyOkPressed()
{
	WatchItemBase::propertyOkPressed();
	m_flags.detailed = m_detailed->isChecked();
}

void WatchItemBoolTriStateBase::propertyDefaultPressed()
{
	WatchItemBase::propertyDefaultPressed();
	m_detailed->setChecked(false);
}

void WatchItemBoolTriStateBase::createTriggerPropertyWidget(QWidget * parent)
{
/*	ASSERT(m_triggerOut == 0);*/

/*	m_triggerOut = new OutProperty(parent, *this);
	CHECK_PTR(m_triggerOut);*/

	#define FIRST_COL_WIDTH 20

	ASSERT(m_triggerOutStateT1Widget == (StatePropertyWidget*) 0);
	ASSERT(m_triggerOutStateT0Widget == (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT1Widget == (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT0Widget == (StatePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT1Widget == (ActivePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT0Widget == (ActivePropertyWidget*) 0);


	PropertyWidget * mainWid = new PropertyWidget(3, parent);
	CHECK_PTR(mainWid);
	mainWid->addColSpacing(0, FIRST_COL_WIDTH);

	QLabel * label;
	QString s;
	static QString sStateProperty(i18n("Boolean TriState Watch",
	                                   ""
	                            "<table><tr><td><b>Inactive</b></td><td> Trigger if state is neither true nor false.</td></tr>"
	                                   "<tr><td><b>False</b></td><td> Trigger if state is false.</td></tr>"
	                                   "<tr><td><b>True</b></td><td> Trigger if state is true.</td></tr>"
	                                   "<tr><td><b>Inactive or False</b></td><td> Trigger if state is inactive or false (in other words not true).</td></tr>"
	                                   "<tr><td><b>Inactive or True</b></td><td> Trigger if state is inactive or true (in other words not false).</td></tr>"
	                                   "<tr><td><b>False or True</b></td><td> Trigger if state is false or true (in other words active).</td></tr>"
	                                   "<tr><td><b>Don't care </b></td><td> Don't check the state.</td></tr></table>"));
	static QString sActiveProperty(i18n("Boolean TriState Watch",
	                                    ""
	                                    "<p>Trigger if the number of active connectors matches the given condition.</p>"
	                             "<table><tr><td><b> &lt </b></td><td> Trigger if the number of active connectors is lesser.</td></tr>"
	                                    "<tr><td><b> = </b></td><td> Trigger if the number of active connectors is equal.</td></tr>"
	                                    "<tr><td><b> &gt </b></td><td> Trigger if the number of active connectors is larger.</td></tr>"
	                                    "<tr><td><b> Don't care </b></td><td> The number of active connectors causes no trigger.</td></tr></table>"));

	static QString preP(QString::fromLatin1("<p>"));
	static QString postP(QString::fromLatin1("</p>"));
	
	// ####################    Input State     ####################
	// ####################          t         ####################

	label = new QLabel(mainWid->newRowHBox(AlignLeft), "InT0Label");
	CHECK_PTR(label);
	if (isConnectorWatch())
	{
		label->setText(i18n("Boolean TriState Watch", "Input(t):"));
		s = i18n("Boolean TriState Watch",
		         "Defines the trigger condition of the input signal at time t.");
	}
	else
	{
		label->setText(i18n("Boolean TriState Watch", "Wire(t):"));
		s = i18n("Boolean TriState Watch",
		         "Defines the trigger condition of the signal at time t.");
	}
	PropertyWidget::addToolTip(s, label);
	PropertyWidget::addWhatsThis(s, label);

	mainWid->addEmptyCell();
	label = new QLabel(i18n("Boolean TriState Watch", "State:"),
	                   mainWid, "InT0LabelState");
	CHECK_PTR(label);

	m_triggerInStateT0Widget = new StatePropertyWidget(m_triggerInStateT0, mainWid, "InT0Property");
	CHECK_PTR(m_triggerInStateT0Widget);
	s = i18n("Boolean TriState Watch",
	         "Defines the signal state at time t.");
	PropertyWidget::addToolTip(s, label, m_triggerInStateT0Widget);
	PropertyWidget::addWhatsThis(preP + s + postP + sStateProperty, label, m_triggerInStateT0Widget);


	mainWid->addEmptyCell();
	label = new QLabel(i18n("Boolean TriState Watch", "Active connectors:"),
	                   mainWid, "InT0LabelCount");
	CHECK_PTR(label);

	m_triggerInActiveT0Widget = new ActivePropertyWidget(m_triggerInActiveT0, getMaxConnectors(), mainWid, "InT0Active");
	CHECK_PTR(m_triggerInActiveT0Widget);
	s = i18n("Boolean TriState Watch",
	         "This trigger depends on the number of active connectors at time t.");
	PropertyWidget::addToolTip(s, label, m_triggerInActiveT0Widget);
	PropertyWidget::addWhatsThis(preP + s + postP + sActiveProperty, label, m_triggerInActiveT0Widget);

	// ####################    Output State     ####################
	// ####################           t         ####################

	label = new QLabel(i18n("Boolean TriState Watch", "Output(t):"),
	                   mainWid->newRowHBox(AlignLeft), "OutT0Label");
	CHECK_PTR(label);
	HIDE_IF_WIRE(label);
	s = i18n("Boolean TriState Watch",
	         "Defines the trigger condition of the output signal at time t.");
	PropertyWidget::addToolTip(s, label);
	PropertyWidget::addWhatsThis(s, label);

	mainWid->addEmptyCell();
	label = new QLabel(i18n("Boolean TriState Watch", "State:"),
	                   mainWid, "OutT0LabelState");
	CHECK_PTR(label);
	HIDE_IF_WIRE(label);

	m_triggerOutStateT0Widget = new StatePropertyWidget(m_triggerOutStateT0, mainWid, "OutT0Property");
	CHECK_PTR(m_triggerOutStateT0Widget);
	HIDE_IF_WIRE(m_triggerOutStateT0Widget);
	s = i18n("Boolean TriState Watch",
	         "Defines the output signal state at time t.");
	PropertyWidget::addToolTip(s, label, m_triggerOutStateT0Widget);
	PropertyWidget::addWhatsThis(preP + s + postP + sStateProperty, label, m_triggerOutStateT0Widget);

	// ####################    Input State     ####################
	// ####################        t - 1       ####################

	label = new QLabel(mainWid->newRowHBox(AlignLeft), "InT1Label");
	CHECK_PTR(label);
	if (isConnectorWatch())
	{
		label->setText(i18n("Boolean TriState Watch", "Input(t-1):"));
		s = i18n("Boolean TriState Watch",
		         "Defines the trigger condition of the input signal at time t-1.");
	}
	else
	{
		label->setText(i18n("Boolean TriState Watch", "Wire(t-1):"));
		s = i18n("Boolean TriState Watch",
		         "Defines the trigger condition of the signal at time t-1.");
	}
	PropertyWidget::addToolTip(s, label);
	PropertyWidget::addWhatsThis(s, label);

	mainWid->addEmptyCell();
	label = new QLabel(i18n("Boolean TriState Watch", "State:"),
	                   mainWid, "InT1LabelState");
	CHECK_PTR(label);

	m_triggerInStateT1Widget = new StatePropertyWidget(m_triggerInStateT1, mainWid, "InT1Property");
	CHECK_PTR(m_triggerInStateT1Widget);
	s = i18n("Boolean TriState Watch",
	         "Defines the signal state at time t-1.");
	PropertyWidget::addToolTip(s, label, m_triggerInStateT1Widget);
	PropertyWidget::addWhatsThis(preP + s + postP + sStateProperty, label, m_triggerInStateT1Widget);

	mainWid->addEmptyCell();
	label = new QLabel(i18n("Boolean TriState Watch", "Active connectors:"),
	                   mainWid, "InT1LabelCount");
	CHECK_PTR(label);

	m_triggerInActiveT1Widget = new ActivePropertyWidget(m_triggerInActiveT1, getMaxConnectors(), mainWid, "InT1Active");
	CHECK_PTR(m_triggerInActiveT1Widget);
	s = i18n("Boolean TriState Watch",
	         "This trigger depends on the number of active connectors at time t-1.");
	PropertyWidget::addToolTip(s, label, m_triggerInActiveT1Widget);
	PropertyWidget::addWhatsThis(preP + s + postP + sActiveProperty, label, m_triggerInActiveT1Widget);

	// ####################    Output State     ####################
	// ####################         t - 1       ####################

	label = new QLabel(i18n("Boolean TriState Watch", "Output(t-1):"),
	                   mainWid->newRowHBox(AlignLeft), "OutT1Label");
	CHECK_PTR(label);
	HIDE_IF_WIRE(label);
	s = i18n("Boolean TriState Watch",
	         "Defines the trigger condition of the output signal at time t-1.");
	PropertyWidget::addToolTip(s, label);
	PropertyWidget::addWhatsThis(s, label);

	mainWid->addEmptyCell();
	label = new QLabel(i18n("Boolean TriState Watch", "State:"),
	                   mainWid, "OutT1LabelState");
	CHECK_PTR(label);
	HIDE_IF_WIRE(label);

	m_triggerOutStateT1Widget = new StatePropertyWidget(m_triggerOutStateT1, mainWid, "OutT1Property");
	CHECK_PTR(m_triggerOutStateT1Widget);
	HIDE_IF_WIRE(m_triggerOutStateT1Widget);
	s = i18n("Boolean TriState Watch",
	         "Defines the output signal state at time t-1.");
	PropertyWidget::addToolTip(s, label, m_triggerOutStateT1Widget);
	PropertyWidget::addWhatsThis(preP + s + postP + sStateProperty, label, m_triggerOutStateT1Widget);

	//##### Some Hints for the users ######
	label = new QLabel(i18n("Boolean TriState Watch",
	                "<b>Hints:</b>"
	            "<ul><li>A break trigger is released if <b>all</b> conditions are fulfilled.</li>"
	                "<li>The time t is the current simulation time. E.g. the state(t) is the current signal state.</li>"
	                "<li>The time t-1 is the simulation time one simulation cycle before. E.g. the state(t-1) is the last signal state.</li></ul>"),
	           mainWid->newRowHBox(AlignLeft));
//	label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
//	label->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));  Schlecht
//	label->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
//	label->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
//	label->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
	label->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

	#undef FIRST_COL_WIDTH
}

void WatchItemBoolTriStateBase::triggerPropertyOkPressed()
{
	ASSERT(m_triggerOutStateT1Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerOutStateT0Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT1Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT0Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT1Widget != (ActivePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT0Widget != (ActivePropertyWidget*) 0);

	m_triggerOutStateT1 = m_triggerOutStateT1Widget->value();
//	delete m_triggerOutStateT1Widget;
	m_triggerOutStateT1Widget = (StatePropertyWidget*) 0;

	m_triggerOutStateT0 = m_triggerOutStateT0Widget->value();
//	delete m_triggerOutStateT0Widget;
	m_triggerOutStateT0Widget = (StatePropertyWidget*) 0;

	m_triggerInStateT1 = m_triggerInStateT1Widget->value();
//	delete m_triggerInStateT1Widget;
	m_triggerInStateT1Widget = (StatePropertyWidget*) 0;

	m_triggerInStateT0 = m_triggerInStateT0Widget->value();
//	delete m_triggerInStateT0Widget;
	m_triggerInStateT0Widget = (StatePropertyWidget*) 0;

	m_triggerInActiveT1 = m_triggerInActiveT1Widget->value();
//	delete m_triggerInActiveT1Widget;
	m_triggerInActiveT1Widget = (ActivePropertyWidget*) 0;

	m_triggerInActiveT0 = m_triggerInActiveT0Widget->value();
//	delete m_triggerInActiveT0Widget;
	m_triggerInActiveT0Widget = (ActivePropertyWidget*) 0;
	
}

void WatchItemBoolTriStateBase::triggerPropertyDefaultPressed()
{
	ASSERT(m_triggerOutStateT1Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerOutStateT0Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT1Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT0Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT1Widget != (ActivePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT0Widget != (ActivePropertyWidget*) 0);

	m_triggerOutStateT1Widget->setValue(DEFAULT_OUT_STATE_T1);
	m_triggerOutStateT0Widget->setValue(DEFAULT_OUT_STATE_T0);
	m_triggerInStateT1Widget->setValue(DEFAULT_IN_STATE_T1);
	m_triggerInStateT0Widget->setValue(DEFAULT_IN_STATE_T0);
	m_triggerInActiveT1Widget->setValue(DEFAULT_IN_ACTIVE_T1);
	m_triggerInActiveT0Widget->setValue(DEFAULT_IN_ACTIVE_T0);
}

void WatchItemBoolTriStateBase::triggerPropertyCancelPressed()
{
	ASSERT(m_triggerOutStateT1Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerOutStateT0Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT1Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInStateT0Widget != (StatePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT1Widget != (ActivePropertyWidget*) 0);
	ASSERT(m_triggerInActiveT0Widget != (ActivePropertyWidget*) 0);

//	delete m_triggerOutStateT1Widget;
	m_triggerOutStateT1Widget = (StatePropertyWidget*) 0;
//	delete m_triggerOutStateT0Widget;
	m_triggerOutStateT0Widget = (StatePropertyWidget*) 0;

//	delete m_triggerInStateT1Widget;
	m_triggerInStateT1Widget = (StatePropertyWidget*) 0;
//	delete m_triggerInStateT0Widget;
	m_triggerInStateT0Widget = (StatePropertyWidget*) 0;

//	delete m_triggerInActiveT1Widget;
	m_triggerInActiveT1Widget = (ActivePropertyWidget*) 0;
//	delete m_triggerInActiveT0Widget;
	m_triggerInActiveT0Widget = (ActivePropertyWidget*) 0;
}

//############################################################################################
//############################################################################################

WatchItemBoolTriStateConnector::WatchItemBoolTriStateConnector(ConnectorBase * connector)
	:	WatchItemBoolTriStateBase(connector, false)
{
}

WatchItemBoolTriStateConnector::~WatchItemBoolTriStateConnector()
{
}

void WatchItemBoolTriStateConnector::readData(unsigned int index)
{
	ConnectorBoolTriState * conn = (ConnectorBoolTriState *) getConnector();
	const WireStateBoolTriState * pOutData = (const WireStateBoolTriState *)conn->readoutData();
	const WireStateBoolTriState * pInData = (const WireStateBoolTriState *)conn->readoutInData();
	if (pInData && pOutData)
	{
		m_traceOutBuffer[index] = *pOutData;
		m_traceInBuffer[index] = *pInData;
		setIndexUsed(index);
	}
}

//############################################################################################
//############################################################################################

WatchItemBoolTriStateWireProperty::WatchItemBoolTriStateWireProperty(WireProperty * wireProperty)
	:	WatchItemBoolTriStateBase(wireProperty, true)
{
}

WatchItemBoolTriStateWireProperty::~WatchItemBoolTriStateWireProperty()
{
}

void WatchItemBoolTriStateWireProperty::readData(unsigned int index)
{
	WirePropertyBoolTriState * wp = (WirePropertyBoolTriState *)getWireProperty();
	wp = (WirePropertyBoolTriState *)(wp->getMasterWireProperty());
	const WireStateBoolTriState * pData = (const WireStateBoolTriState *)wp->readoutData();
	if (pData)
	{
		m_traceInBuffer[index] = *pData;
		setIndexUsed(index);
	}
}

//############################################################################################
//############################################################################################

WatchItemBoolTriStateConnectorSpecial::WatchItemBoolTriStateConnectorSpecial(ConnectorBoolTriStateSpecial * connector)
	:	WatchItemBoolTriStateBase(connector, true)
{
}

WatchItemBoolTriStateConnectorSpecial::~WatchItemBoolTriStateConnectorSpecial()
{
}

void WatchItemBoolTriStateConnectorSpecial::readData(unsigned int index)
{
	const WirePropertyBoolTriState * wpb = (const WirePropertyBoolTriState *)getConnector()->getWireProperty();
	const void * pData = wpb->getMasterWireProperty()->readoutData();
	
	if (pData)
	{
		m_traceInBuffer[index] = *(const WireStateBoolTriState *)pData;
		setIndexUsed(index);
	}
}

