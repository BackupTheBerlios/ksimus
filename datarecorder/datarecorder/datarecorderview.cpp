/***************************************************************************
                          datarecorderview.cpp  -  description
                             -------------------
    begin                : Thu May 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
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
#include "ksimus/resource.h"
#include "datarecorderview.h"

// Forward declaration


DataRecorderView::DataRecorderView(Component * comp, eViewType viewType)
	: CompView(comp,viewType)
{
	setPlace(QRect(0, 0, 7*gridX, 5*gridY));
}

DataRecorderView::~DataRecorderView()
{
}
void DataRecorderView::draw(QPainter * p)
{
	QRect rect(getPlace().topLeft()+QPoint(gridX+1,1),
							getPlace().bottomRight()-QPoint(gridX-0,1));
	
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);
	
	p->drawText(rect, AlignCenter, "Data\nRec");

	CompView::draw(p);
}
