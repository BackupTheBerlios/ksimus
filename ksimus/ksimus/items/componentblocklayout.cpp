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
#include "ksimembfont.h"
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
		m_textAlign(AlignCenter)
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

		p->save();

		p->setPen(black);
		if (isDrawFrame())
		{
			CompView::drawFrame(p, rect);
		}

		if (!getText().isEmpty())
		{
			QRect r(rect.left() + 3, rect.top() + 3, rect.width() - 6, rect.height() - 6);
			g_embFont08->drawText(p, r, getTextAlign(), getText());
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

		p->setPen(black);
		if (isDrawFrame())
		{
			p->setBrush(black);
			const int x = rect.left();
			const int y = rect.top();
			const int w = rect.width();
			const int h = rect.height();
			p->drawRect(x,               y,                    w,           2          );
			p->drawRect(x,               y,                    2,           h - gridY/2);
			p->drawRect(x,               y + h - gridY/2 - 2,  gridY/2 + 2, 2          );
			p->drawRect(x + gridY/2,     y + h - gridY/2 - 2,  2,           gridY/2 + 2);
			p->drawRect(x + w-2,         y,                    2,           h - gridY/2);
			p->drawRect(x + w-2,         y + h - gridY/2 - 2,  -gridY/2,    2          );
			p->drawRect(x + w-2- gridY/2,y + h - gridY/2 - 2,  2,           gridY/2 + 2);
		}

		if (!getText().isEmpty())
		{
			QRect r(rect.left() + 3, rect.top() + 3, rect.width() - 6, rect.height() - 6);
			g_embFont08->drawText(p, r, getTextAlign(), getText());
		}
	}
}

