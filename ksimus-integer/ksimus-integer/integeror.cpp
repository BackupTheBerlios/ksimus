/***************************************************************************
                          integeror.cpp  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"
#include "integeror.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerBitwiseOr::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerBitwiseOr(container, ci);
}

const ComponentInfo * IntegerBitwiseOr::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Bitwise OR"),
	                                QString::fromLatin1("Integer/Logic/Bitwise OR"),
	                                i18n("Component", "Integer/Logic/Bitwise OR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerBitwiseOr::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "bit\nOr");
}

//###############################################################

IntegerBitwiseOr::IntegerBitwiseOr(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerBitwiseOr::calculate()
{
//	IntegerXIn1Out::calculate();

	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result |= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)result);
}


//###############################################################
//###############################################################

Component * IntegerBitwiseNor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerBitwiseNor(container, ci);
}

const ComponentInfo * IntegerBitwiseNor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Bitwise NOR"),
	                                QString::fromLatin1("Integer/Logic/Bitwise NOR"),
	                                i18n("Component", "Integer/Logic/Bitwise NOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerBitwiseNor::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "bit\nNor");
}

//###############################################################

IntegerBitwiseNor::IntegerBitwiseNor(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerBitwiseNor::calculate()
{
//	IntegerXIn1Out::calculate();

	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result |= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)~result);
}

//###############################################################
//###############################################################

Component * IntegerLogicalOr::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLogicalOr(container, ci);
}

const ComponentInfo * IntegerLogicalOr::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Logical OR"),
	                                QString::fromLatin1("Integer/Logic/Logical OR"),
	                                i18n("Component", "Integer/Logic/Logical OR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerLogicalOr::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "log\nOr");
}

//###############################################################

IntegerLogicalOr::IntegerLogicalOr(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerLogicalOr::calculate()
{
//	IntegerXIn1Out::calculate();

	int result = 0; // all ones

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (((ConnectorIntegerIn*)it.current())->getInput() != 0)
		{
			result = 1;
			break;
		}
	}

	setValue(result);
}

//###############################################################
//###############################################################

Component * IntegerLogicalNor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLogicalNor(container, ci);
}

const ComponentInfo * IntegerLogicalNor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Logical NOR"),
	                                QString::fromLatin1("Integer/Logic/Logical NOR"),
	                                i18n("Component", "Integer/Logic/Logical NOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerLogicalNor::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "log\nNor");
}

//###############################################################

IntegerLogicalNor::IntegerLogicalNor(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerLogicalNor::calculate()
{
//	IntegerXIn1Out::calculate();

	int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (((ConnectorIntegerIn*)it.current())->getInput() != 0)
		{
			result = 1;
			break;
		}
	}

	setValue(!result);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

