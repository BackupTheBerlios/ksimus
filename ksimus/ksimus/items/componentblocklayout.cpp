/***************************************************************************
                          componentblocklayout.cpp  -  description
                             -------------------
    begin                : Sun Dec 16 2001
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

// KDE-Includes

// Project-Includes
#include "componentblocklayout.h"
#include "component.h"
#include "compview.h"
#include "ksimaction.h"
#include "resource.h"
#include "ksimdebug.h"

// Forward declaration


#define FLAG_DRAW_FRAME 0x0001


//#######################################################################
//#######################################################################

ComponentBlockLayout::ComponentBlockLayout(CompView * sheetView, ComponentLayout * compLayout, Orientation orient, QString blockName)
	:	ComponentAddOn(sheetView->getComponent(), blockName),
		DoubleLayout(compLayout, orient),
		m_flags(FLAG_DRAW_FRAME),	
		m_text(),
		m_alignFlags(AlignCenter)
{
	getAction().disable(KSimAction::ALL);
	getAction().enable(KSimAction::DRAWSHEETVIEW);
}

ComponentBlockLayout::~ComponentBlockLayout()
{
//	KSIMDEBUG("ComponentBlockLayout::~ComponentBlockLayout()");
}

void ComponentBlockLayout::setDrawFrame(bool enable)
{
	if(enable)
		m_flags |= FLAG_DRAW_FRAME;
	else
		m_flags &= ~FLAG_DRAW_FRAME;
}

bool ComponentBlockLayout::isDrawFrame() const
{
	return m_flags & FLAG_DRAW_FRAME;
}

void ComponentBlockLayout::drawSheetView (QPainter *p) const
{
	if (isDrawFrame() || !getText().isEmpty())
	{	
		QRect rect(DoubleLayout::getRect(false));
		QRect rectView(getComponent()->getSheetView()->getPlace());
	
		rect.moveBy(rectView.topLeft().x(),rectView.topLeft().y());
		rect.rLeft() ++;
		rect.rTop() ++;
		p->save();
		p->setPen(QPen(black, 2));
		p->setBrush(NoBrush);
	
		if (isDrawFrame())
		{
			p->drawRect(rect);
		}
		
		rect.rTop() --;
		if (!getText().isEmpty())
		{
			QFont newFont("helvetica",8);
			p->setFont(newFont);
			p->setPen(black);
			p->drawText(rect, getAlign(), getText());
		}
		p->restore();
	}
}

//#######################################################################
//#######################################################################

ComponentControlBlock::ComponentControlBlock(CompView * sheetView, ComponentLayout * compLayout, QString blockName)
	: ComponentBlockLayout(sheetView, compLayout, Qt::Horizontal, blockName)
{
}
	
ComponentControlBlock::~ComponentControlBlock()
{
}

void ComponentControlBlock::drawSheetView(QPainter *p) const
{
	if (isDrawFrame() || !getText().isEmpty())
	{	
		QRect rect(DoubleLayout::getRect(false));
		QRect rectView(getComponent()->getSheetView()->getPlace());
	
		rect.moveBy(rectView.topLeft().x(),rectView.topLeft().y());
		rect.rLeft() ++;
		rect.rTop() ++;
		p->save();
		p->setPen(QPen(black, 2));
//		p->setPen(black);
		p->setBrush(NoBrush);
	
	
		if (isDrawFrame())
		{
			p->moveTo(rect.bottomLeft() + QPoint(gridX/2,0));
			p->lineTo(rect.bottomLeft() + QPoint(gridX/2,-gridY/2));
			p->lineTo(rect.bottomLeft() + QPoint(0,-gridY/2));
			p->lineTo(rect.topLeft());
			p->lineTo(rect.topRight());
			p->lineTo(rect.bottomRight() + QPoint(0,-gridY/2));
			p->lineTo(rect.bottomRight() + QPoint(-gridX/2,-gridY/2));
			p->lineTo(rect.bottomRight() + QPoint(-gridX/2,0));
		}
		
		rect.rTop() --;
		if (!getText().isEmpty())
		{
			QFont newFont("helvetica",8);
			p->setFont(newFont);
			p->setPen(black);
			p->drawText(rect, getAlign(), getText());
		}
		p->restore();
	}
}

