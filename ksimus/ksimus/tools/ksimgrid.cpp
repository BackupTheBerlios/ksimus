/***************************************************************************
                          ksimgrid.cpp  -  description
                             -------------------
    begin                : Sun Oct 8 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

// include QT files
#include <qapplication.h>
#include <qpainter.h>
#include <qpen.h>
#include <qsize.h>
#include <qrect.h>
#include <qstring.h>

// include KDE files

// include project files
#include "resource.h"
#include "ksimdebug.h"
#include "ksimgrid.h"
#include "ksimdata.h"

static const char * const sGridStyle  = "Style";
static const char * const sGridColor  = "Color";
static const char * const sGridX      = "SpaceX";
static const char * const sGridY      = "SpaceY";
static const char * const sEnable     = "Enable";
static const char * const sGlobaleEna = "GlobalGrid";

KSimGrid::KSimGrid(GRIDSTYLE style, const QColor & color)
	:	m_style((style == GridNone)?GridDots:style),
		m_gridX(gridX),
		m_gridY(gridY),
		m_enabled(style != GridNone),
		m_useGlobalGrid(true)
{
	m_color = new QColor(color);
	CHECK_PTR(m_color);
}


KSimGrid::KSimGrid(const KSimGrid & grid)
	:	m_style(grid.m_style),
		m_gridX(grid.m_gridX),
		m_gridY(grid.m_gridY),
		m_enabled(grid.m_enabled),
		m_useGlobalGrid(grid.m_useGlobalGrid)
{
	m_color = new QColor(*grid.m_color);
	CHECK_PTR(m_color);
}

const KSimGrid & KSimGrid::operator=(const KSimGrid & grid)
{
	if (this != &grid)
	{
		m_style 		= grid.m_style;
		*m_color 		= *grid.m_color;
		m_gridX 		= grid.m_gridX;
		m_gridY			= grid.m_gridY;
		m_enabled		= grid.m_enabled;
		m_useGlobalGrid = grid.m_useGlobalGrid;
	}
	return grid;
}


KSimGrid::~KSimGrid()
{
	delete m_color;
}


/** Draws the grid */
void KSimGrid::draw(QPainter * p, const QSize &size) const
{
	draw(p, QRect(QPoint(0,0),size));
}
void KSimGrid::draw(QPainter * p, const QRect &rect) const
{
	
	if (!m_enabled || (m_style == GridNone))
		return;
		
	int x,y,dx,dy;
	
	if (isGlobalGridEnabled())
	{
		//Default grid size
		dx = gridX;
		dy = gridY;
	}
	else
	{
		//User grid size
		dx = m_gridX;
		dy = m_gridY;
	}	
	
	p->save();
	p->setPen(QPen(*m_color, 0));
	
	if (m_style == GridDots)
	{
		// Dots on crosspoints
		for (y=rect.y(); y < rect.height(); y+=dy)
		{
			for (x=rect.x(); x < rect.width(); x+=dx)
			{
				p->drawPoint(x,y);
			}
		}
	}
	else
	{
		Qt::PenStyle penStyle;
		
		switch(m_style)
		{
			/** GRIDSTYLE != PenStyle !!! */
			case GridDotLine:
				penStyle = Qt::DotLine;
				break;

			case GridDashLine:
				penStyle = Qt::DashLine;
				break;

			case GridDashDotLine:
				penStyle = Qt::DashDotLine;
				break;

			case GridDashDotDotLine:
				penStyle = Qt::DashDotDotLine;
				break;

			case GridSolidLine:
				penStyle = Qt::SolidLine;
				break;

			default:
				KSIMDEBUG_VAR("Unknown grid style", (int)m_style);
				penStyle = Qt::SolidLine;
				break;
		}
		
		p->setPen(QPen(*m_color, 0, penStyle));
		for (y=rect.y(); y < rect.height(); y+=dy)
		{
			p->drawLine(rect.x(),y,rect.width(),y);
		}
		for (x=rect.x(); x < rect.width(); x+=dx)
		{
			p->drawLine(x,rect.y(),x,rect.height());
		}
	}
	p->restore();
}
	
void KSimGrid::setEnable(bool enable)
{
	m_enabled = enable;
}

bool KSimGrid::isEnabled() const
{
	return m_enabled;
}

/** Sets the grid style */
void KSimGrid::setStyle(GRIDSTYLE style)
{
	m_style = style;
}

/** Returns the grid style */
GRIDSTYLE KSimGrid::getStyle() const
{
	return m_style;
}
	
/** set the grid color */
void KSimGrid::setColor(const QColor & color)
{
	*m_color = color;
}

/** returns the grid color */
const QColor & KSimGrid::getColor() const
{
	return *m_color;
}

/** set style and color */
void KSimGrid::set(GRIDSTYLE style, const QColor & color)
{
	m_style = style;
	*m_color = color;
}

/** Enable the usage of the global grid spaces */
void KSimGrid::enableGlobalGrid(bool enable)
{
	m_useGlobalGrid = enable;
}
bool KSimGrid::isGlobalGridEnabled() const
{
	return m_useGlobalGrid;
}


/** Sets the spaces between grid lines/dots */
void KSimGrid::setGridX(unsigned int gridX_)
{
	m_gridX = gridX_;
}

void KSimGrid::setGridY(unsigned int gridY_)
{
	m_gridY = gridY_;
}

void KSimGrid::setGrid(unsigned int gridX_, unsigned int gridY_)
{
	m_gridX = gridX_;	
	m_gridY = gridY_;	
}

/** Returns the spaces between grid lines/dots */
unsigned int KSimGrid::getGridX() const
{
	return m_gridX;
}

unsigned int KSimGrid::getGridY() const
{
	return m_gridY;
}

/** Save grid property */
void KSimGrid::save(KSimData & file) const
{
	if (m_style != GridDots)
	{
		file.writeEntry(sGridStyle, (int)m_style);
	}
	if (*m_color != Qt::gray)
	{
		file.writeEntry(sGridColor, *m_color);
	}
	
	if (m_gridX != gridX)
	{
		file.writeEntry(sGridX, m_gridX);
	}
	
	if (m_gridY != gridY)
	{
		file.writeEntry(sGridY, m_gridY);
	}
	
	if (m_enabled == false)
	{
		file.writeEntry(sEnable, false);
	}
	
	if (m_useGlobalGrid == false)
	{
		file.writeEntry(sGlobaleEna, false);
	}
	
}

/** Load grid property */
void KSimGrid::load(KSimData & file)
{
	QColor defColor(Qt::gray);
	
	m_style = (GRIDSTYLE)file.readNumEntry(sGridStyle, (int)GridDots);
	*m_color = file.readColorEntry(sGridColor,&defColor);
	m_gridX = file.readNumEntry(sGridX, gridX);
	m_gridY = file.readNumEntry(sGridY, gridY);
	m_enabled = file.readBoolEntry(sEnable, true);
	m_useGlobalGrid = file.readBoolEntry(sGlobaleEna, true);
	
}

