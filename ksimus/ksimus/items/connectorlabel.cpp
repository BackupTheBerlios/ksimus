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
#include "ksimdebug.h"

// Project-Includes
#include "connectorlabel.h"

// Forward declaration

//###############################################################
//###############################################################

ConnectorLabel::ConnectorLabel(ConnectorBase * conn, const QString & descr)
	: ComponentAddOn(conn->getComponent()),
		m_conn(conn),
		m_descr(descr),
		m_spacing(1)
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
	QPoint pos = m_conn->getPos();
	
	p->save();
	QFont newFont("helvetica",8);
	p->setFont(newFont);
	int height = p->fontMetrics().height();
	int width = p->fontMetrics().width(m_descr);
	
	
	switch (m_conn->getOrientation())
	{
		case CO_TOP:
			pos.rx() += -width/2 - 1;
			pos.ry() += gridY/2 - 1 + getSpacing();
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;
		
		case CO_RIGHT:
			pos.rx() += -width - gridX/2 - 2 - getSpacing();
			pos.ry() += -height + gridY/2 + 1;
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;
		
		case CO_BOTTOM:
			pos.rx() += -width/2 - 1;
			pos.ry() += - height - gridY/2 - getSpacing();
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;
			
		case CO_LEFT:
			pos.rx() +=  gridX/2 + getSpacing();
			pos.ry() += -height + gridY/2 + 1;
			p->drawText(pos.x(), pos.y(), width, height, AlignCenter, m_descr);
			break;		
	
		default:
			KSIMDEBUG_VAR("Unknown connector orientation",m_conn->getOrientation());
			break;
	}
	p->restore();
}
