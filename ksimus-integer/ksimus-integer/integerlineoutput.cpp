/***************************************************************************
                          integerlineoutput.cpp  -  description
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
#include "ksimus/resource.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/ksimbaseintbaseselection.h"
#include "ksimus/ksimdata.h"

#include "integerlineoutput.h"


// Forward declaration

namespace KSimLibInteger
{

//#######################################################################
//#######################################################################

Component * IntegerLineOutput::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLineOutput(container, ci);
}

const ComponentInfo * IntegerLineOutput::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Label"),
	                                QString::fromLatin1("Integer/Output/Label"),
	                                i18n("Component", "Integer/Output/Label"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-integer-output-label")*/);
	return &Info;
}


const KSimBaseInt::eBaseType IntegerLineOutput::defaultBase = KSimBaseInt::Decimal;

//##########################################################################################
//##########################################################################################


IntegerLineOutput::IntegerLineOutput(CompContainer * container, const ComponentInfo * ci)
	: ComponentStyle(container, ci),
		m_number(0, defaultBase),
		m_base(defaultBase)
{
	m_input = new ConnectorIntegerIn(this,
	                                 QString::fromLatin1("Input"),
	                                 i18n("Integer-Connector", "Input"));
	CHECK_PTR(m_input);
	
	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerLineOutputView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new IntegerLineOutputView(this, USER_VIEW);
	}
}

/*IntegerLineOutput::~IntegerLineOutput()
{
} */

void IntegerLineOutput::calculate()
{
	ComponentStyle::calculate();

	int newValue = getInput()->getInput();
	
	if(m_number.value() != newValue)
	{
		m_number = newValue; // Base is unchanged !!
		emit signalSetText(m_number.text());
	}
}


void IntegerLineOutput::reset()
{
	ComponentStyle::reset();

	m_number = (int)0; // Base is unchanged !!
	
	emit signalSetText(m_number.text());
}


void IntegerLineOutput::save(KSimData & file) const
{
	ComponentStyle::save(file);

	if (getBase() != defaultBase)
	{
		const char * text = KSimBaseInt::convertBase(getBase());
		file.writeEntry("Label Base", text);
	}
}


bool IntegerLineOutput::load(KSimData & file, bool copyLoad)
{
	QString text = file.readEntry("Label Base");
	setBase(KSimBaseInt::convertBase(text.latin1(), defaultBase));

	return ComponentStyle::load(file, copyLoad);
}


ComponentPropertyBaseWidget * IntegerLineOutput::createGeneralProperty(QWidget *parent)
{
	IntegerLineOutputPropertyGeneralWidget * wid;
	wid = new IntegerLineOutputPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

void IntegerLineOutput::setBase(KSimBaseInt::eBaseType base)
{
	m_base = base;
	m_number.setBase(base);
	emit signalSetText(m_number.text());
}

//##########################################################################################
//##########################################################################################




IntegerLineOutputView::IntegerLineOutputView(IntegerLineOutput * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 7*gridX, 3*gridY));
		setMinSize(5*gridX, 3*gridY);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
		enableConnectorSpacingRight(false);
	}
	else
	{
		setPlace(QRect(0, 0, 6*gridX, 3*gridY));
		setMinSize(4*gridX, 3*gridY);
	}	
}


QWidget * IntegerLineOutputView::createCompViewWidget(QWidget * parent)
{
	IntegerLineOutputWidgetView * wid = new IntegerLineOutputWidgetView(this, parent,"Label");
	
	/* Specific signals */
	// Number changed (Component->LCD)
	connect(getLineOutput(), SIGNAL(signalSetText(const QString &)), wid->m_label, SLOT(setText(const QString &)));

	// Setup initial text
	wid->m_label->setText(getLineOutput()->getNumber().text());

	return wid;
}

void IntegerLineOutputView::resize()
{
	CompViewSize::resize();

	if (getViewType() == SHEET_VIEW)
	{
		QPoint connPos(0, getPlace().height()/gridY/2);
		getLineOutput()->getInput()->setGridPos(connPos);
	}
}


/*IntegerLineOutputView::~IntegerLineOutputView()
{
} */


//##########################################################################################
//##########################################################################################

IntegerLineOutputWidgetView::IntegerLineOutputWidgetView(IntegerLineOutputView * cv, QWidget *parent, const char *name)
	:	CompViewVBox(cv,parent,name)
{
	m_label = new QLabel(this);
	CHECK_PTR(m_label);
	m_label->setAlignment(AlignRight | AlignVCenter);
}

/*IntegerLineOutputWidgetView::~IntegerLineOutputWidgetView()
{
} */


//##########################################################################################
//##########################################################################################

IntegerLineOutputPropertyGeneralWidget::IntegerLineOutputPropertyGeneralWidget(IntegerLineOutput * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip, precTip;


	QLabel * lab = new QLabel(i18n("Integer", "Base: "), this, "BaseLabel");
	CHECK_PTR(lab);
	
	m_baseSel = new KSimBaseIntBaseSelection(comp->getBase(), this, "m_baseSel");
	CHECK_PTR(m_baseSel);

	tip = i18n("The base of the number.");
	addToolTip(tip, m_baseSel, lab);
	addWhatsThis(tip, m_baseSel, lab);
	lab->setBuddy(m_baseSel);
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void IntegerLineOutputPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();

	if (getIntegerLineOutput()->getBase() != m_baseSel->getBase())
	{
		changeData();
		getIntegerLineOutput()->setBase(m_baseSel->getBase());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void IntegerLineOutputPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_baseSel->setBase(IntegerLineOutput::defaultBase);
}

//##########################################################################################
//##########################################################################################


};  //namespace KSimLibInteger

#include "integerlineoutput.moc"
