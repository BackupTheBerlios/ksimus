/***************************************************************************
                          boolean2in1out.cpp  -  description
                             -------------------
    begin                : Mon May 14 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
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
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "boolean2in1out.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/componentlayout.h"

// Forward declaration

// Project-Includes


//###############################################################
//###############################################################
namespace KSimLibBoolean
{


Boolean2In1OutView::Boolean2In1OutView(Boolean2In1Out * comp, eViewType viewType, const QString & text, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		ComponentLayoutSimple * layout = new ComponentLayoutSimple(this);
		CHECK_PTR(layout);
	
		layout->setMinSize(3, 5);

		layout->getLeft()->addSpace(1);
		layout->getLeft()->addConnector(comp->getInputConnectorA());
		layout->getLeft()->addConnector(comp->getInputConnectorB());
		
		layout->getRight()->addStretch(2);
		layout->getRight()->addConnector(comp->getOutputConnector());
		layout->getRight()->addStretch(2);
		
		if (!text.isEmpty())
		{
			new ComponentLayoutBlockContentText(layout->getBlock(), text);
		}
	}
}
/*Boolean2In1OutView::~Boolean2In1OutView()
{
}*/

//###############################################################

Boolean2In1Out::Boolean2In1Out(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	m_out = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Boolean-Connector", "Output"),
	                             QPoint(4,2));
	CHECK_PTR(m_out);
	m_inA = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Input A"),
	                             i18n("Boolean-Connector", "Input A"),
	                             QPoint(0,1));
	CHECK_PTR(m_inA);
	m_inB = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Input B"),
	                             i18n("Boolean-Connector", "Input B"),
	                             QPoint(0,3));
	CHECK_PTR(m_inB);
}

/*Boolean2In1Out::~Boolean2In1Out()
{
} */

void Boolean2In1Out::setState(bool newState)
{
	getOutputConnector()->setOutput(newState);
}
	
bool Boolean2In1Out::getState() const
{
	return getOutputConnector()->getOutput();
}

void Boolean2In1Out::reset()
{
	Component::reset();
	setState(false);
}

}; //namespace KSimLibBoolean

#include "boolean2in1out.moc"
