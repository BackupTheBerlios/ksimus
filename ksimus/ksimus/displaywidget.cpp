/***************************************************************************
                          displaywidget.cpp  -  description
                             -------------------
    begin                : Fri Sep 8 2000
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

#include <qpixmap.h>
#include <qpainter.h>

#include "compview.h"
#include "displaywidget.h"

DisplayWidget::DisplayWidget(CompViewList * viewList, QWidget *parent, const char *name )
	:	QWidget(parent,name),
		drawMap(new QPixmap),
		displayList(viewList)
{
	CHECK_PTR(drawMap);
}

DisplayWidget::~DisplayWidget()
{
	delete drawMap;
	emit destroyed(this);
}


/** Redraw the drawMap with all components, wires and selections
	updateDrawMap() doesn't update the widget. update() will do that.*/
void DisplayWidget::updateDrawMap()
{
	CHECK_PTR(drawMap);

	QPainter p (drawMap);
	drawMap->fill(backgroundColor()/*lightGray*/);
	
	FOR_EACH_COMPVIEW(it, *displayList)
	{
		it.current()->draw(&p);
	}
}	


void DisplayWidget::paintEvent(QPaintEvent *)
{
	bitBlt(this, 0, 0, drawMap);
}	

void DisplayWidget::resizeEvent(QResizeEvent * ev)
{
	drawMap->resize(ev->size());
	updateDrawMap();
}	

/** This slot delete this widget */
void DisplayWidget::slotDelete(void)
{
	delete this;
}
