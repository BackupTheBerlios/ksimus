/***************************************************************************
                          ksimgrid.h  -  description
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

#ifndef KSIMGRID_H
#define KSIMGRID_H

#include <qcolor.h>

class QPainter;
class QSize;
class QRect;

class KSimData;


/**Manage grid
  *@author Rasmus Diekenbrock
  */


enum GRIDSTYLE { 	GridNone = -1, GridDots , GridDotLine, GridDashLine,
									GridDashDotLine, GridDashDotDotLine, GridSolidLine };

class KSimGrid
{

public:
	KSimGrid(GRIDSTYLE style = GridNone, const QColor & color = Qt::lightGray);
	KSimGrid(const KSimGrid & grid);
	~KSimGrid();
	
	/** Draws the grid */
	void draw(QPainter * p, const QSize &size) const;
	void draw(QPainter * p, const QRect &rect) const;
	
	/** Enable grid */
	void setEnable(bool enable);
	bool isEnabled() const;
	
	/** Sets the grid style */
	void setStyle(GRIDSTYLE style);
	/** Returns the grid style */
	GRIDSTYLE getStyle() const;
	
	/** set the grid color */
	void setColor(const QColor & color);
	/** returns the grid color */
	const QColor & getColor() const;
	
	/** set style and color */
	void set(GRIDSTYLE style, const QColor & color);

	/** Enable the usage of the global grid spaces */
	void enableGlobalGrid(bool enable = true);
	bool isGlobalGridEnabled() const;
	
	/** Sets the spaces between grid lines/dots */
	void setGridX(unsigned int gridX_);
	void setGridY(unsigned int gridY_);
	void setGrid(unsigned int gridX_, unsigned int gridY_);
	/** Returns the spaces between grid lines/dots */
	unsigned int getGridX() const;
	unsigned int getGridY() const;

	/** Save grid property */
	void save(KSimData & file) const;
	/** Load grid property */
	void load(KSimData & file);
	

	const KSimGrid & operator=(const KSimGrid & grid);
	
protected:
	GRIDSTYLE m_style;
	QColor * m_color;
	unsigned int m_gridX;
	unsigned int m_gridY;
	bool m_enabled;
	bool m_useGlobalGrid;
};

#endif
