/***************************************************************************
                          booleanconstant.cpp  -  description
                             -------------------
    begin                : Mon Dec 17 2001
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
#include <qpainter.h>
#include <qbitmap.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/compview.h"
#include "booleanconstant.h"

// Forward declaration

namespace KSimLibBoolean
{

Component * BooleanConstantFalse::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanConstantFalse(container, ci);
}

const ComponentInfo * BooleanConstantFalse::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Const False"),
	                                QString::fromLatin1("Boolean/Input/Const False"),
	                                i18n("Component", "Boolean/Input/Const False"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-const-false"));
	return &Info;
}

//###############################################################
//###############################################################

/** Comp view for boolan constants.
  * @author Rasmus Diekenbrock
  */


class BooleanConstantFalse::View : public CompView
{
public:
	View(BooleanConstantFalse * comp, eViewType viewType);
	~View();
	
	virtual void draw(QPainter * p);
	ConnectorBoolOut * getConnector() const { return ((BooleanConstantFalse *)getComponent())->getOutputConnector(); };
	
private:	
	static QBitmap * m_bitmap;
	static unsigned int m_instanceCnt;
};

unsigned int BooleanConstantFalse::View::m_instanceCnt = 0;
QBitmap * BooleanConstantFalse::View::m_bitmap = 0;

BooleanConstantFalse::View::View(BooleanConstantFalse * comp, eViewType viewType)
	:	CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 3*gridX, 3*gridY));
	enableRotation(true);

	getConnector()->setGridPos(1,0);
	getConnector()->setOrientation(CO_TOP);
	
	m_instanceCnt++;
	if (m_bitmap == 0)
	{
		m_bitmap = new QBitmap(3*gridX, 3*gridY, true);
		Q_CHECK_PTR(m_bitmap);
		QPainter p(m_bitmap);
		p.setPen(QPen(color1,2));
		p.setBrush(NoBrush);
		const int x = 3*gridX/2;	// to middle
		const int y = 3*gridY/2;
		p.drawLine(x,   y-4, x,   y+4);
		p.drawLine(x-6, y+4, x+6, y+4);
		m_bitmap->setMask(*m_bitmap); // selfMask 
	}
}


BooleanConstantFalse::View::~View()
{
	if (--m_instanceCnt == 0)
	{
		delete m_bitmap;
		m_bitmap = 0;
	}
}

void BooleanConstantFalse::View::draw(QPainter * p)
{
	CompView::draw(p);
	p->drawPixmap(QPoint(0,0), *m_bitmap);
}

//############################################################################
//############################################################################


BooleanConstantFalse::BooleanConstantFalse(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci)
{
	m_out = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Boolean-Connector", "Output"));
	CHECK_PTR(m_out);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
}

/*BooleanConstantFalse::~BooleanConstantFalse()
{}*/

void BooleanConstantFalse::reset()
{
  	getOutputConnector()->setOutput(false);
}

//###############################################################
//###############################################################


Component * BooleanConstantTrue::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanConstantTrue(container, ci);
}

const ComponentInfo * BooleanConstantTrue::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Const True"),
	                                QString::fromLatin1("Boolean/Input/Const True"),
	                                i18n("Component", "Boolean/Input/Const True"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-const-true"));
	return &Info;
}

//###############################################################
//###############################################################

/** Comp view for boolan constants.
  * @author Rasmus Diekenbrock
  */


class BooleanConstantTrue::View : public CompView
{
public:
	View(BooleanConstantTrue * comp, eViewType viewType);
	~View();
	
	virtual void draw(QPainter * p);
	ConnectorBoolOut * getConnector() const { return ((BooleanConstantTrue *)getComponent())->getOutputConnector(); };
	
private:
	static QBitmap * m_bitmap;
	static unsigned int m_instanceCnt;
};

unsigned int BooleanConstantTrue::View::m_instanceCnt = 0;
QBitmap * BooleanConstantTrue::View::m_bitmap = 0;

BooleanConstantTrue::View::View(BooleanConstantTrue * comp, eViewType viewType)
	:	CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 3*gridX, 3*gridY));
	enableRotation(true);

	getConnector()->setGridPos(1,2);
	getConnector()->setOrientation(CO_BOTTOM);
	
	m_instanceCnt++;
	if (m_bitmap == 0)
	{
		m_bitmap = new QBitmap(3*gridX, 3*gridY, true);
		Q_CHECK_PTR(m_bitmap);
		QPainter p(m_bitmap);
		p.setPen(QPen(color1, 2));
		p.setBrush(NoBrush);
		const int x = 3*gridX/2;	// to middle
		const int y = 3*gridY/2 - 2;
		const int dia = 12;
		p.drawEllipse(x - dia/2, y - dia/2, dia, dia);
		p.drawLine(x, y - dia/4, x, y + dia/4);
		p.drawLine(x - dia/4, y, x + dia/4, y);
		m_bitmap->setMask(*m_bitmap); // selfMask 
	}
}


BooleanConstantTrue::View::~View()
{
	if (--m_instanceCnt == 0)
	{
		delete m_bitmap;
		m_bitmap = 0;
	}
}

void BooleanConstantTrue::View::draw(QPainter * p)
{
	CompView::draw(p);
	p->drawPixmap(QPoint(0,0), *m_bitmap);
}

//############################################################################
//############################################################################


BooleanConstantTrue::BooleanConstantTrue(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci)
{
	m_out = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Boolean-Connector", "Output"));
	CHECK_PTR(m_out);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
}

/*BooleanConstant::~BooleanConstant()
{}*/

void BooleanConstantTrue::reset()
{
	getOutputConnector()->setOutput(true);
}

//###############################################################
//###############################################################


}; //namespace KSimLibBoolean

