/***************************************************************************
                          connectorlabel.cpp  -  description
                             -------------------
    begin                : Fri Nov 2 2001
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

// KSimus-Includes
#include "resource.h"
#include "connectorbase.h"
#include "connectorboolinedge.h"
#include "ksimdebug.h"

// Project-Includes
#include "connectorlabel.h"

// Forward declaration

//###############################################################

// defines for m_flags

#define CONN_MASK         0x0000FFFFL
#define CONN_GENERIC      0x00000000L
#define CONN_BOOL_IN_EDGE 0x00000001L


#define isGenericConn()     ((m_flags & CONN_MASK) == CONN_GENERIC)
#define isBoolInEdge()      ((m_flags & CONN_MASK) == CONN_BOOL_IN_EDGE)

#define getBoolInEdgeConn() ((ConnectorBoolInEdge*)m_conn)

//###############################################################

ConnectorLabel::ConnectorLabel(ConnectorBase * conn, const QString & descr)
	: ComponentAddOn(conn->getComponent(), QString("Connector Label " + descr)),
		m_conn(conn),
		m_descr(descr),
		m_spacing(1),
		m_flags(CONN_GENERIC)
{
	init();
}
	
ConnectorLabel::ConnectorLabel(ConnectorBoolInEdge * conn, const QString & descr)
	: ComponentAddOn(conn->getComponent(), QString("Connector Label " + descr)),
		m_conn(conn),
		m_descr(descr),
		m_spacing(1),
		m_flags(CONN_BOOL_IN_EDGE)
{
	init();
}

void ConnectorLabel::init()
{
	getAction().disable(KSimAction::ALL);
	getAction().enable(KSimAction::DRAWSHEETVIEW);
	
	connect(m_conn, SIGNAL(destroyed()), SLOT(slotConnDeleted()));
}


void ConnectorLabel::setSpacing(int spacing)
{
	m_spacing = spacing;
}

int ConnectorLabel::getSpacing() const
{
	return m_spacing;
}

void ConnectorLabel::slotConnDeleted()
{
	delete this;
}

/** Draw the sheet view.
	* The default implementation does nothing.
	*/
void ConnectorLabel::drawSheetView (QPainter *p) const
{
	if(m_conn->isHidden()) return;
	
	QPoint pos = m_conn->getPos();
	int spacing = getSpacing();
	bool edgeIn = (isBoolInEdge() && getBoolInEdgeConn()->isEdgeSensitive());
		
	p->save();
	QFont newFont("helvetica",8);
	p->setFont(newFont);
	p->setPen(black);
	int height = p->fontMetrics().height();
	int width = p->fontMetrics().width(m_descr);
	
	if (edgeIn)
	{
		spacing += 5;
	}
	
	switch (m_conn->getOrientation())
	{
		case CO_TOP:
			if (edgeIn)
			{
				QPoint center (pos + QPoint (-1, gridY/2 + 1));
				p->drawLine(center.x()+2, center.y(), center.x(), center.y()+5);
				p->drawLine(center.x()-2, center.y(), center.x(), center.y()+5);
			}
			pos.rx() += -width/2 - 1;
			pos.ry() += gridY/2 - 1 + spacing;
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;
		
		case CO_RIGHT:
			if (edgeIn)
			{
				QPoint center (pos + QPoint (- gridX/2 - 2, -1));
				p->drawLine(center.x(), center.y()+2, center.x()-5, center.y());
				p->drawLine(center.x(), center.y()-2, center.x()-5, center.y());
			}
			pos.rx() += -width - gridX/2 - 2 - spacing;
			pos.ry() += -height + gridY/2 + 1;
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;
		
		case CO_BOTTOM:
			if (edgeIn)
			{
				QPoint center (pos + QPoint (-1, - gridY/2 - 2));
				p->drawLine(center.x()+2, center.y(), center.x(), center.y()-5);
				p->drawLine(center.x()-2, center.y(), center.x(), center.y()-5);
			}
			pos.rx() += -width/2 - 1;
			pos.ry() += - height - gridY/2 - spacing;
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;
			
		case CO_LEFT:
			if (edgeIn)
			{
				QPoint center (pos + QPoint (gridX/2 +1 , 0));
				p->drawLine(center.x(), center.y()+2, center.x()+5, center.y());
				p->drawLine(center.x(), center.y()-2, center.x()+5, center.y());
			}
			pos.rx() +=  gridX/2 + spacing;
			pos.ry() += -height + gridY/2 + 1;
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;		
	
		default:
			KSIMDEBUG_VAR("Unknown connector orientation",m_conn->getOrientation());
			break;
	}
	p->restore();
}
