/***************************************************************************
                          floatxin1out.cpp  -  description
                             -------------------
    begin                : Wen Jan 3 2002
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

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "floatxin1out.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"



// Forward declaration


namespace KSimLibFloatingPoint
{

//###############################################################
//###############################################################


FloatXIn1Out::FloatXIn1Out(CompContainer * container, const ComponentInfo * ci)
	:	Float1Out(container, ci)
{
	m_inPack = new ConnectorPack(this, I18N_NOOP("Input"), &ConnectorFloatInInfo, 2, 10);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorName(I18N_NOOP("Input %1"));
	m_inPack->setConnectorCount(2);
}

/*FloatXIn1Out::~FloatXIn1Out()
{
} */


//###############################################################
//###############################################################



FloatXIn1OutView::FloatXIn1OutView(FloatXIn1Out * comp, eViewType viewType)
	: Float1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnectorPack(comp->getInputConnectorPack());
	
		getComponentLayout()->updateLayout();
	}
}

/*FloatXIn1OutView::~FloatXIn1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

