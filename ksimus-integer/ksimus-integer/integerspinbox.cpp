/***************************************************************************
                          integerspinbox.cpp  -  description
                             -------------------
    begin                : Wen Jul 30 2003
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
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qcheckbox.h>
#include <qvbox.h>
#include <qstringlist.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/componentlayout.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbaseintspinbox.h"
#include "integerspinbox.h"

// Forward declaration


namespace KSimLibInteger
{

Component * IntegerSpinbox::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerSpinbox(container, ci);
}

const ComponentInfo * IntegerSpinbox::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Spin Box"),
	                                QString::fromLatin1("Integer/Input/Spin Box"),
	                                i18n("Component", "Integer/Input/Spin Box"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-integer-input-line-edit")*/);
	return &Info;
}


//############################################################################
//############################################################################


IntegerSpinbox::IntegerSpinbox(CompContainer * container, const ComponentInfo * ci)
	: IntegerStyleRange1Out(container, ci)
{
	
//	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);

	setInitMaxValue(100);
	setInitMinValue(0);
	
	setTrackingEnabled(true, true);
	setWraparoundEnabled(false, true);
	setLineStep(KSimBaseInt(1, KSimBaseInt::Decimal), true);
	setSpecialValueText(QString::null);
	setPrefixText(QString::null);
	setSuffixText(QString::null);

	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerSpinboxView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new IntegerSpinboxView(this, USER_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);

}


void IntegerSpinbox::save(KSimData & file) const
{
	IntegerStyleRange1Out::save(file);
	
	if (isTrackingEnabled() != isTrackingInitEnabled())
	{
		file.writeEntry("Value Tracking", isTrackingEnabled());
	}
	if (isWraparoundEnabled() != isWraparoundInitEnabled())
	{
		file.writeEntry("Wrap Around", isWraparoundEnabled());
	}
	if (getLineStep() != getLineStepInit())
	{
		getLineStep().save(file, "Line Step");
	}
	if (getSpecialValueText() != QString::null)
	{
		file.writeEntry("Special Value Text", getSpecialValueText());
	}
	if (getPrefixText() != QString::null)
	{
		file.writeEntry("Prefix Text", getPrefixText());
	}
	if (getSuffixText() != QString::null)
	{
		file.writeEntry("Suffix Text", getSuffixText());
	}
	if (getConstantList() != QString::null)
	{
		file.writeEntry("Constant List", getConstantList());
	}
}


bool IntegerSpinbox::load(KSimData & file, bool copyLoad)
{
	setTrackingEnabled( file.readBoolEntry("Value Tracking", isTrackingInitEnabled()) );
	setWraparoundEnabled( file.readBoolEntry("Wrap Around", isWraparoundInitEnabled()) );
	
	KSimBaseInt loadLineStep;
	if (!loadLineStep.load(file, "Line Step"))
	{
		loadLineStep = getLineStepInit();
	}
	if (loadLineStep != getLineStep())
	{
		setLineStep(loadLineStep);
	}

	setSpecialValueText(file.readEntry("Special Value Text", QString::null));
	setPrefixText(file.readEntry("Prefix Text", QString::null));
	setSuffixText(file.readEntry("Suffix Text", QString::null));

	return IntegerStyleRange1Out::load(file, copyLoad);
}


void IntegerSpinbox::reset()
{
	IntegerStyleRange1Out::reset();

	m_currentValueTracking = m_currentValue = getResetValue();
	
	emit valueChanged(getResetValue());
}


void IntegerSpinbox::newValueTracking(const KSimBaseInt & i)
{
//	KSIMDEBUG(QString::fromLatin1("IntegerSpinbox::newValueTracking new=%1 old=%2").arg(i.text()).arg(m_currentValueTracking.text()));
	if (i != m_currentValueTracking)
	{
		m_currentValueTracking = i;
		setValue(m_currentValueTracking.value());
		executeNext();
	}
}


void IntegerSpinbox::newValue(const KSimBaseInt & i)
{
//	KSIMDEBUG(QString::fromLatin1("IntegerSpinbox::newValue new=%1 old=%2").arg(i.text()).arg(m_currentValue.text()));
	if (i != m_currentValue)
	{
		m_currentValue = i;
		setValue(m_currentValue.value());
		emit valueChanged(m_currentValue);
		executeNext();
	}
}


void IntegerSpinbox::setTrackingEnabled(bool tracking, bool init)
{
	if (init)
	{
		m_flags.trackingInit = tracking;
	}
	m_flags.tracking = tracking;
	emit trackingChanged(tracking);
}


bool IntegerSpinbox::isTrackingEnabled() const
{
	return m_flags.tracking;
}


bool IntegerSpinbox::isTrackingInitEnabled() const
{
	return m_flags.trackingInit;
}


void IntegerSpinbox::setWraparoundEnabled(bool warparound, bool init)
{
	if (init)
	{
		m_flags.warparoundInit = warparound;
	}
	m_flags.warparound = warparound;
	emit wraparoundChanged(warparound);
}


bool IntegerSpinbox::isWraparoundEnabled() const
{
	return m_flags.warparound;
}


bool IntegerSpinbox::isWraparoundInitEnabled() const
{
	return m_flags.warparoundInit;
}


void IntegerSpinbox::setLineStep(const KSimBaseInt & lines, bool init)
{
	if (init)
	{
		m_lineStepInit = lines;
	}
	m_lineStep = lines;
	emit lineStepChanged(lines.value());
}

void IntegerSpinbox::setSpecialValueText(const QString & text)
{
	m_specialValueText = text.isEmpty() ? QString::null : text;
	emit specialValueTextChanged(m_specialValueText);
}

void IntegerSpinbox::setPrefixText(const QString & text)
{
	m_prefixText = text.isEmpty() ? QString::null : text;
	emit prefixTextChanged(m_prefixText);
}

void IntegerSpinbox::setSuffixText(const QString & text)
{
	m_suffixText = text.isEmpty() ? QString::null : text;
	emit suffixTextChanged(m_suffixText);
}

void IntegerSpinbox::setConstantList(const QString & text)
{
	m_constantList = text.isEmpty() ? QString::null : text;
	emit constantListChanged(m_constantList);
}
 
ComponentPropertyBaseWidget * IntegerSpinbox::createGeneralProperty(QWidget *parent)
{
	IntegerSpinboxPropertyGeneralWidget * wid;
	wid = new IntegerSpinboxPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

void IntegerSpinbox::checkProperty(QStringList & errorMsg)
{
	IntegerStyleRange1Out::checkProperty(errorMsg);

	bool ok;
	QStringList list = QStringList::split("\n", getConstantList());
	QStringList::ConstIterator it = list.begin();

	for(; it != list.end(); ++it)
	{
		KSimBaseIntNamed i = KSimBaseIntNamed::convert(*it, &ok);
		if (!ok)
		{
			errorMsg.append(i18n("Format error in constant list (%1)!").arg(*it));
		}
	}
}


//############################################################################
//############################################################################

IntegerSpinboxView::IntegerSpinboxView(IntegerSpinbox * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	enableRotation(false);
	
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 9*gridX, 3*gridY));
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
		setPlace(QRect(0, 0, 8*gridX, 3*gridY));
		setMinSize(4*gridX, 3*gridY);
	}
}


QWidget * IntegerSpinboxView::createCompViewWidget(QWidget * parent)
{
	IntegerSpinboxWidgetView * wid = new IntegerSpinboxWidgetView(this, parent,"IntegerSpinboxWidgetView");
	
	/* Specific signals */
	// Number changed (Component->Edit)
	connect(getIntegerSpinbox(), SIGNAL(valueChanged(const KSimBaseInt &)), wid->m_spinbox, SLOT(setValue(const KSimBaseInt &)));
	// Number changed (final) (Edit->Component)
	connect(wid->m_spinbox, SIGNAL(valueChanged(const KSimBaseInt &)), getComponent(), SLOT(newValue(const KSimBaseInt &)));
	// Number changed (tracked) (Edit->Component)
	connect(wid->m_spinbox, SIGNAL(valueChangedTracking(const KSimBaseInt &)), getComponent(), SLOT(newValueTracking(const KSimBaseInt &)));
	// min value changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(minValueChanged(int)), wid, SLOT(setMinValue(int)));
	// max value changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(maxValueChanged(int)), wid, SLOT(setMaxValue(int)));
	// tracking changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(trackingChanged(bool)), wid, SLOT(setTrackingEnabled(bool)));
	// wraparound changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(wraparoundChanged(bool)), wid, SLOT(setWraparoundEnabled(bool)));
	// line step changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(lineStepChanged(int)), wid, SLOT(setLineStep(int)));
	// special Value Text changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(specialValueTextChanged(const QString &)), wid, SLOT(setSpecialValueText(const QString &)));
	// prefix Text changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(prefixTextChanged(const QString &)), wid, SLOT(setPrefixText(const QString &)));
	// suffix Text changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(suffixTextChanged(const QString &)), wid, SLOT(setSuffixText(const QString &)));
	// constant list changed (Component->Widget)
	connect(getIntegerSpinbox(), SIGNAL(constantListChanged(const QString &)), wid, SLOT(setConstantList(const QString &)));
	
	return wid;
}

/*IntegerSpinboxView::~IntegerSpinboxView()
{
} */

//##########################################################################################
//##########################################################################################

IntegerSpinboxWidgetView::IntegerSpinboxWidgetView(IntegerSpinboxView * cv, QWidget *parent, const char *name)
	:	CompViewHBox(cv,parent,name)
{
	m_spinbox = new KSimBaseIntSpinBox(this, "IntegerSpinboxWidgetView m_edit");
	CHECK_PTR(m_spinbox);
	m_spinbox->setMinValue(getIntegerSpinbox()->getMinValue().value());
	m_spinbox->setMaxValue(getIntegerSpinbox()->getMaxValue().value());
	m_spinbox->setTrackingEnabled(getIntegerSpinbox()->isTrackingEnabled());
	m_spinbox->setWrapping(getIntegerSpinbox()->isWraparoundEnabled());
	m_spinbox->setLineStep(getIntegerSpinbox()->getLineStep().value());
	m_spinbox->setSpecialValueText(getIntegerSpinbox()->getSpecialValueText());
	m_spinbox->setPrefix(getIntegerSpinbox()->getPrefixText());
	m_spinbox->setSuffix(getIntegerSpinbox()->getSuffixText());

	setConstantList(getIntegerSpinbox()->getConstantList());
	
	m_spinbox->setValue(getIntegerSpinbox()->getValue());
}

/*IntegerSpinboxWidgetView::~IntegerSpinboxWidgetView()
{
} */

void IntegerSpinboxWidgetView::setMinValue(int i)
{
	m_spinbox->setMinValue(i);
}

void IntegerSpinboxWidgetView::setMaxValue(int i)
{
	m_spinbox->setMaxValue(i);
}

void IntegerSpinboxWidgetView::setTrackingEnabled(bool tracking)
{
	m_spinbox->setTrackingEnabled(tracking);
}

void IntegerSpinboxWidgetView::setWraparoundEnabled(bool wraparound)
{
	m_spinbox->setWrapping(wraparound);
}

void IntegerSpinboxWidgetView::setLineStep(int lines)
{
	m_spinbox->setLineStep(lines);
}

void IntegerSpinboxWidgetView::setSpecialValueText(const QString & text)
{
	m_spinbox->setSpecialValueText(text);
}

void IntegerSpinboxWidgetView::setPrefixText(const QString & text)
{
	m_spinbox->setPrefix(text);
}

void IntegerSpinboxWidgetView::setSuffixText(const QString & text)
{
	m_spinbox->setSuffix(text);
}

void IntegerSpinboxWidgetView::setConstantList(const QString & text)
{
	m_spinbox->clearConstantList();

	bool ok;
	QStringList list = QStringList::split("\n", text);
	QStringList::ConstIterator it = list.begin();

	for(; it != list.end(); ++it)
	{
		KSimBaseIntNamed i = KSimBaseIntNamed::convert(*it, &ok);
		if (ok)
		{
			m_spinbox->appendConstant((KSimBaseInt)i, i.name());
		}
	}
}


//###############################################################
//###############################################################

IntegerSpinboxPropertyGeneralWidget::IntegerSpinboxPropertyGeneralWidget(IntegerSpinbox * comp, QWidget *parent, const char *name)
	:	IntegerStyleRange1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip, precTip;
	QLabel * lab;

	lab = new QLabel(i18n("integer", "Line step:"), this, "m_lineStep label");
	CHECK_PTR(lab);
	m_lineStep = new KSimBaseIntSpinBox(comp->getLineStep(), 1, 100000, 1, this, "m_lineStep");
	CHECK_PTR(m_lineStep);
	lab->setBuddy(m_lineStep);
	tip = i18n("The value of the spinbox will be incremented/decremented by the number of line steps if the user press the arrow buttons.");
	addToolTip(tip, m_lineStep, lab);
	addWhatsThis(tip, m_lineStep, lab);
	m_lineStep->appendConstant(KSimBaseInt(m_lineStep->minValue()), i18n("integer", "Minimum"));
	m_lineStep->appendConstant(KSimBaseInt(m_lineStep->maxValue()), i18n("integer", "Maximum"));
	
	QVBox * lineBox = newRowVBox();
	
	m_tracking = new QCheckBox(i18n("Integer", "Value tracking"), lineBox, "m_tracking");
	CHECK_PTR(m_tracking);
	tip = i18n("Enables the value tracking.\n"
	           "If enabled the output will be updated after each inserted digit.\n"
	           "If disabled the output will be updated after the user press return or select another input field.");
	addToolTip(tip, m_tracking);
	addWhatsThis(tip, m_tracking);
	
	m_wraparound = new QCheckBox(i18n("Integer", "Wrap around"), lineBox, "m_wraparound");
	CHECK_PTR(m_wraparound);
	tip = i18n("Enables the wrap around function.\n"
	           "If enabled it is possible to step from 'Minimum value' to 'Maximum value' and vice versa.");
	addToolTip(tip, m_wraparound);
	addWhatsThis(tip, m_wraparound);

	lab = new QLabel(i18n("integer", "Special value text:"), this, "m_specialValueText label");
	CHECK_PTR(lab);
	m_specialValueText = new QLineEdit(comp->getSpecialValueText(), this, "m_specialValueText");
	CHECK_PTR(m_specialValueText);
	lab->setBuddy(m_specialValueText);
	tip = i18n("The special value text is shown if the spinbox value is equal to 'Minimum value'.\n"
	           "An empty text disables the function.");
	addToolTip(tip, m_specialValueText, lab);
	addWhatsThis(tip, m_specialValueText, lab);

	lab = new QLabel(i18n("integer", "Prefix text:"), this, "m_prefixText label");
	CHECK_PTR(lab);
	m_prefixText = new QLineEdit(comp->getPrefixText(), this, "m_prefixText");
	CHECK_PTR(m_prefixText);
	lab->setBuddy(m_prefixText);
	tip = i18n("The prefix text is displayed in front of the spinbox value.\n"
	           "The text does not influence the value.");
	addToolTip(tip, m_prefixText, lab);
	addWhatsThis(tip, m_prefixText, lab);

	lab = new QLabel(i18n("integer", "Suffix text:"), this, "m_suffixText label");
	CHECK_PTR(lab);
	m_suffixText = new QLineEdit(comp->getSuffixText(), this, "m_suffixText");
	CHECK_PTR(m_suffixText);
	lab->setBuddy(m_suffixText);
	tip = i18n("The suffix text is displayed behind of the spinbox value.\n"
	           "The text does not influence the value.");
	addToolTip(tip, m_suffixText, lab);
	addWhatsThis(tip, m_suffixText, lab);

	lab = new QLabel(i18n("integer", "Constants:"), this, "m_constantList label");
	CHECK_PTR(lab);
	m_constantList = new QMultiLineEdit(this, "m_constantList");
	CHECK_PTR(m_constantList);
	lab->setBuddy(m_constantList);
	tip = i18n("Optional constants available by the popup menu during simulation.\n"
	           "One constant each line. Format: <value> <optional name>.");
	addToolTip(tip, m_constantList, lab);
	addWhatsThis(tip, m_constantList, lab);

	
	// Setup value
	m_tracking->setChecked(comp->isTrackingEnabled());
	m_wraparound->setChecked(comp->isWraparoundEnabled());
	m_constantList->setText(comp->getConstantList());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void IntegerSpinboxPropertyGeneralWidget::acceptPressed()
{
	IntegerStyleRange1OutPropertyGeneralWidget::acceptPressed();
	
	if (getIntegerSpinbox()->isTrackingEnabled() != m_tracking->isChecked())
	{
		changeData();
		getIntegerSpinbox()->setTrackingEnabled(m_tracking->isChecked());
	}
	if (getIntegerSpinbox()->isWraparoundEnabled() != m_wraparound->isChecked())
	{
		changeData();
		getIntegerSpinbox()->setWraparoundEnabled(m_wraparound->isChecked());
	}
	if (getIntegerSpinbox()->getLineStep() != m_lineStep->value())
	{
		changeData();
		getIntegerSpinbox()->setLineStep(m_lineStep->value());
	}
	if (getIntegerSpinbox()->getSpecialValueText() != m_specialValueText->text())
	{
		changeData();
		getIntegerSpinbox()->setSpecialValueText(m_specialValueText->text());
	}
	if (getIntegerSpinbox()->getPrefixText() != m_prefixText->text())
	{
		changeData();
		getIntegerSpinbox()->setPrefixText(m_prefixText->text());
	}
	if (getIntegerSpinbox()->getSuffixText() != m_suffixText->text())
	{
		changeData();
		getIntegerSpinbox()->setSuffixText(m_suffixText->text());
	}
	if (getIntegerSpinbox()->getConstantList() != m_constantList->text())
	{
		changeData();
		getIntegerSpinbox()->setConstantList(m_constantList->text());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void IntegerSpinboxPropertyGeneralWidget::defaultPressed()
{
	IntegerStyleRange1OutPropertyGeneralWidget::defaultPressed();

	m_tracking->setChecked(getIntegerSpinbox()->isTrackingInitEnabled());
	m_wraparound->setChecked(getIntegerSpinbox()->isWraparoundInitEnabled());
	m_lineStep->setValue(getIntegerSpinbox()->getLineStepInit());
	m_specialValueText->setText(QString::null);
	m_prefixText->setText(QString::null);
	m_suffixText->setText(QString::null);
	m_constantList->setText(QString::null);
}

//###############################################################
//###############################################################


};  //namespace KSimLibInteger

#include "integerspinbox.moc"
