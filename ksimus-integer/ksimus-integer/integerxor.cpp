/***************************************************************************
                          integerxor.cpp  -  description
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
#include "integerxor.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerBitwiseXor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerBitwiseXor(container, ci);
}

const ComponentInfo * IntegerBitwiseXor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Bitwise XOR"),
	                                QString::fromLatin1("Integer/Logic/Bitwise XOR"),
	                                i18n("Component", "Integer/Logic/Bitwise XOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerBitwiseXor::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "bit\nXor");
}

//###############################################################

IntegerBitwiseXor::IntegerBitwiseXor(CompContainer * container, const ComponentInfo * ci)
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
void IntegerBitwiseXor::calculate()
{
//	IntegerXIn1Out::calculate();

	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result ^= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)result);
}


//###############################################################
//###############################################################

Component * IntegerBitwiseXnor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerBitwiseXnor(container, ci);
}

const ComponentInfo * IntegerBitwiseXnor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Bitwise XNOR"),
	                                QString::fromLatin1("Integer/Logic/Bitwise XNOR"),
	                                i18n("Component", "Integer/Logic/Bitwise XNOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerBitwiseXnor::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "bit\nXnor");
}

//###############################################################

IntegerBitwiseXnor::IntegerBitwiseXnor(CompContainer * container, const ComponentInfo * ci)
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
void IntegerBitwiseXnor::calculate()
{
//	IntegerXIn1Out::calculate();

	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result ^= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)~result);
}

//###############################################################
//###############################################################

Component * IntegerLogicalXor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLogicalXor(container, ci);
}

const ComponentInfo * IntegerLogicalXor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Logical XOR"),
	                                QString::fromLatin1("Integer/Logic/Logical XOR"),
	                                i18n("Component", "Integer/Logic/Logical XOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerLogicalXor::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "log\nXor");
}

//###############################################################

IntegerLogicalXor::IntegerLogicalXor(CompContainer * container, const ComponentInfo * ci)
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
void IntegerLogicalXor::calculate()
{
//	IntegerXIn1Out::calculate();

	int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (((ConnectorIntegerIn*)it.current())->getInput() != 0)
		{
			result ^= 1;
		}
	}

	setValue(result);
}

//###############################################################
//###############################################################

Component * IntegerLogicalXnor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLogicalXnor(container, ci);
}

const ComponentInfo * IntegerLogicalXnor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Logical XNOR"),
	                                QString::fromLatin1("Integer/Logic/Logical XNOR"),
	                                i18n("Component", "Integer/Logic/Logical XNOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

void IntegerLogicalXnor::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "log\nXnor");
}

//###############################################################

IntegerLogicalXnor::IntegerLogicalXnor(CompContainer * container, const ComponentInfo * ci)
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
void IntegerLogicalXnor::calculate()
{
//	IntegerXIn1Out::calculate();

	int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (((ConnectorIntegerIn*)it.current())->getInput() != 0)
		{
			result ^= 1;
		}
	}

	setValue(!result);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

