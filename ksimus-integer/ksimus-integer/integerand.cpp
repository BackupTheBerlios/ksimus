/***************************************************************************
                          integerand.cpp  -  description
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
#include <limits.h>

// QT-Includes
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"
#include "integerand.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerBitwiseAnd::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerBitwiseAnd(container, ci);
}

const ComponentInfo * IntegerBitwiseAnd::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Bitwise AND"),
	                                QString::fromLatin1("Integer/Logic/Bitwise AND"),
	                                i18n("Component", "Integer/Logic/Bitwise AND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerBitwiseAnd::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "bit\nAnd");
}

//###############################################################

IntegerBitwiseAnd::IntegerBitwiseAnd(CompContainer * container, const ComponentInfo * ci)
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
void IntegerBitwiseAnd::calculate()
{
//	IntegerXIn1Out::calculate();

	unsigned int result = UINT_MAX; // all ones

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result &= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)result);
}


//###############################################################
//###############################################################

Component * IntegerBitwiseNand::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerBitwiseNand(container, ci);
}

const ComponentInfo * IntegerBitwiseNand::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Bitwise NAND"),
	                                QString::fromLatin1("Integer/Logic/Bitwise NAND"),
	                                i18n("Component", "Integer/Logic/Bitwise NAND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerBitwiseNand::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "bit\nNand");
}

//###############################################################

IntegerBitwiseNand::IntegerBitwiseNand(CompContainer * container, const ComponentInfo * ci)
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
void IntegerBitwiseNand::calculate()
{
//	IntegerXIn1Out::calculate();

	unsigned int result = UINT_MAX;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result &= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)~result);
}

//###############################################################
//###############################################################

Component * IntegerLogicalAnd::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLogicalAnd(container, ci);
}

const ComponentInfo * IntegerLogicalAnd::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Logical AND"),
	                                QString::fromLatin1("Integer/Logic/Logical AND"),
	                                i18n("Component", "Integer/Logic/Logical AND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerLogicalAnd::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "log\nAnd");
}

//###############################################################

IntegerLogicalAnd::IntegerLogicalAnd(CompContainer * container, const ComponentInfo * ci)
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
void IntegerLogicalAnd::calculate()
{
//	IntegerXIn1Out::calculate();

	int result = 1;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (((ConnectorIntegerIn*)it.current())->getInput() == 0)
		{
			result = 0;
			break;
		}
	}

	setValue(result);
}

//###############################################################
//###############################################################

Component * IntegerLogicalNand::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLogicalNand(container, ci);
}

const ComponentInfo * IntegerLogicalNand::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Logical NAND"),
	                                QString::fromLatin1("Integer/Logic/Logical NAND"),
	                                i18n("Component", "Integer/Logic/Logical NAND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerLogicalNand::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "log\nNand");
}

//###############################################################

IntegerLogicalNand::IntegerLogicalNand(CompContainer * container, const ComponentInfo * ci)
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
void IntegerLogicalNand::calculate()
{
//	IntegerXIn1Out::calculate();

	int result = 1;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (((ConnectorIntegerIn*)it.current())->getInput() == 0)
		{
			result = 0;
			break;
		}
	}

	setValue(!result);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

