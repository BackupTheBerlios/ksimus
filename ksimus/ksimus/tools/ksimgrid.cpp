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
#include <kconfigbase.h>

// include project files
#include "resource.h"
#include "ksimdebug.h"
#include "ksimgrid.h"

static const char * sGridStyle	= "Style";
static const char * sGridColor	= "Color";
static const char * sGridX 		= "SpaceX";
static const char * sGridY		= "SpaceY";
static const char * sEnable		= "Enable";
static const char * sGlobaleEna	= "GlobalGrid";

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
		
	int x,y;
	p->save();
	p->setPen(QPen(*m_color, 0));
	
	if (m_style == GridDots)
	{
		// Dots on crosspoints
		for (y=rect.y(); y < rect.height(); y+=m_gridY)
		{
			for (x=rect.x(); x < rect.width(); x+=m_gridX)
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
		for (y=rect.y(); y < rect.height(); y+=m_gridY)
		{
			p->drawLine(rect.x(),y,rect.width(),y);
		}
		for (x=rect.x(); x < rect.width(); x+=m_gridX)
		{
			p->drawLine(x,rect.y(),x,rect.height());
		}
	}
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
void KSimGrid::save(KConfigBase & file) const
{
	file.writeEntry(sGridStyle, (int)m_style);
	file.writeEntry(sGridColor, *m_color);
	
	file.writeEntry(sGridX, m_gridX);
	file.writeEntry(sGridY, m_gridY);
	file.writeEntry(sEnable, m_enabled);
	file.writeEntry(sGlobaleEna, m_useGlobalGrid);
	
}

/** Load grid property */
void KSimGrid::load(KConfigBase & file)
{
	QColor defColor(Qt::lightGray);
	
	m_style = (GRIDSTYLE)file.readNumEntry(sGridStyle, (int)GridNone);
	*m_color = file.readColorEntry(sGridColor,&defColor);
	m_gridX = file.readNumEntry(sGridX, m_gridX);
	m_gridY = file.readNumEntry(sGridY, m_gridY);
	m_enabled = file.readBoolEntry(sEnable, m_enabled);
	m_useGlobalGrid = file.readBoolEntry(sGlobaleEna, m_useGlobalGrid);
	
}

