/***************************************************************************
                          flipflopbase.cpp  -  description
                             -------------------
    begin                : Fri Dec 7 2001
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
#include <qgrid.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbooleanbox.h"
#include "flipflopbase.h"


// Forward declaration
class ConnectorBoolOut;
class ConnectorBoolIn;



namespace KSimLibBoolean
{


//###############################################################
//###############################################################

FlipFlopBase::FlipFlopBase(CompContainer * container, const ComponentInfo * ci)
	:	Boolean1Out(container, ci),
		m_setDominant(false)
{
	m_notOut = new ConnectorBoolOut(this,
	                             QString::fromLatin1("/Output"),
	                             i18n("Boolean-Connector", "/Output"));
	CHECK_PTR(m_notOut);
	m_notOut->setNegate(true, true);
	
	m_inSet = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Set"),
	                             i18n("Boolean-Connector", "Set"));
	CHECK_PTR(m_inSet);
	
	m_inReset = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Reset"),
	                             i18n("Boolean-Connector", "Reset"));
	CHECK_PTR(m_inReset);


}

/*FlipFlopBase::~FlipFlopBase()
{
}*/


void FlipFlopBase::updateOutput()
{
	Boolean1Out::updateOutput();
	getNotOutputConnector()->setOutput(getState());
}

void FlipFlopBase::reset()
{
	Boolean1Out::reset();
	
	setState( getResetState() );
	getOutputConnector()->setOutput(getState());
	getNotOutputConnector()->setOutput(getState());
}

/** save component properties */
void FlipFlopBase::save(KSimData & file) const
{
	Boolean1Out::save(file);
	
	if (getDominant())   // Save true state only (default is false!)
	{
		file.writeEntry("Set Dominant", true);
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool FlipFlopBase::load(KSimData & file, bool copyLoad)
{
	setDominant( file.readBoolEntry("Set Dominant", false) );
	
	return Boolean1Out::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * FlipFlopBase::createGeneralProperty(QWidget *parent)
{
	FlipFlopBasePropertyGeneralWidget * wid;
	wid = new FlipFlopBasePropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//##########################################################################################
//##########################################################################################


FlipFlopBasePropertyGeneralWidget::FlipFlopBasePropertyGeneralWidget(FlipFlopBase * comp, QWidget *parent, const char *name)
	:	Boolean1OutPropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;	

	lab = new QLabel(i18n("Boolean", "Dominant Input:"), getGrid());
	CHECK_PTR(lab);
	
	m_dominant = new KSimBooleanBox(comp->getDominant(), getGrid());
	CHECK_PTR(m_dominant);
	
	m_dominant->setTrueText(i18n("Boolean", "Set Input"));
	m_dominant->setFalseText(i18n("Boolean", "Reset Input"));
	
	str = i18n("Boolean", "Set here dominant input.");
	QToolTip::add(m_dominant, str);
	QToolTip::add(lab, str);
	QWhatsThis::add(m_dominant, str);
	QWhatsThis::add(lab, str);
	
}

/*FlipFlopBasePropertyGeneralWidget::~FlipFlopBasePropertyGeneralWidget()
{
} */

void FlipFlopBasePropertyGeneralWidget::acceptPressed()
{
	Boolean1OutPropertyGeneralWidget::acceptPressed();

	if (((FlipFlopBase*)getComponent())->getDominant() != m_dominant->getValue())
	{
		changeData();
		((FlipFlopBase*)getComponent())->setDominant(m_dominant->getValue());
	}
}

void FlipFlopBasePropertyGeneralWidget::defaultPressed()
{
	Boolean1OutPropertyGeneralWidget::defaultPressed();

	m_dominant->setValue(false);
}


//###############################################################



}; //namespace KSimLibBoolean
