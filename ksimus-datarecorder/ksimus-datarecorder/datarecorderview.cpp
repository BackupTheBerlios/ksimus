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
#include <qpopupmenu.h>
#include <qcursor.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "datarecorderview.h"
#include "datarecorder.h"

// Forward declaration


namespace KSimLibDataRecorder
{

DataRecorderView::DataRecorderView(Component * comp, eViewType viewType)
	: CompView(comp,viewType)
{
	setPlace(QRect(0, 0, 6*gridX, 5*gridY));
	enableRotation(true);
	enableConnectorSpacingTop(false);
	enableConnectorSpacingRight(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
}

DataRecorderView::~DataRecorderView()
{
}
void DataRecorderView::draw(QPainter * p)
{
	drawFrame(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Data\nRec");

	CompView::draw(p);
}

eHitType DataRecorderView::isHit(int x, int y) const
{
	eHitType res = CompView::isHit(x, y);
	
	if (getComponent()->isRunning() && (res != NO_HIT))
	{
		res = SPECIAL_HIT;
	}
	return res;
}

static QPopupMenu * onlinePopup = 0;

/** Manage the mouse pressed event if required */
void DataRecorderView::mousePress(QMouseEvent * ev, QPainter * p)
{
	CompView::mousePress(ev, p);
	
	if (onlinePopup) delete onlinePopup;
	
	onlinePopup = new QPopupMenu(0, "dataRecorderPopup");
	CHECK_PTR(onlinePopup);
	
	onlinePopup->insertItem(i18n("DataRecorder", "&Open graph"), getComponent(), SLOT(slotOpenWidget()));
	
	onlinePopup->popup(QCursor::pos());
}

};  //namespace KSimLibDataRecorder

#include "datarecorderview.moc"
