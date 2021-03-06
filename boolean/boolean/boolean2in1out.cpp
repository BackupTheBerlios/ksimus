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
#include "ksimus/componentinfo.h"

// Forward declaration

// Project-Includes


//###############################################################
//###############################################################
namespace KSimLibBoolean
{


Boolean2In1OutView::Boolean2In1OutView(Component * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
}
/*Boolean2In1OutView::~Boolean2In1OutView()
{
}*/

void Boolean2In1OutView::draw(QPainter * p)
{
	drawFrame(p);
	
	CompView::draw(p);
}


//###############################################################

Boolean2In1Out::Boolean2In1Out(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	m_out = new ConnectorBoolOut (this, I18N_NOOP("Output"), QPoint(4,2));
	CHECK_PTR(m_out);
	m_inA = new ConnectorBoolIn (this, I18N_NOOP("Input A"), QPoint(0,1));
	CHECK_PTR(m_inA);
	m_inB = new ConnectorBoolIn (this, I18N_NOOP("Input B"), QPoint(0,3));
	CHECK_PTR(m_inB);
}

/*Boolean2In1Out::~Boolean2In1Out()
{
} */

void Boolean2In1Out::updateOutput()
{
	Component::updateOutput();
	
	m_out->setOutput(m_result);
}

void Boolean2In1Out::reset()
{
	Component::reset();
	
	m_result = false;
}

}; //namespace KSimLibBoolean

