/***************************************************************************
                          booleanconstant.cpp  -  description
                             -------------------
    begin                : Mon Dec 17 2001
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
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/compview.h"
#include "booleanconstant.h"

// Forward declaration

namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanConstant(container, ci);
}

const ComponentInfo * getBooleanConstantTrue()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Const True"),
	                                QString::fromLatin1("Boolean/Input/Const True"),
	                                i18n("Component", "Boolean/Input/Const True"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-const-true"));
	return &Info;
}

const ComponentInfo * getBooleanConstantFalse()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Const False"),
	                                QString::fromLatin1("Boolean/Input/Const False"),
	                                i18n("Component", "Boolean/Input/Const False"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-const-false"));
	return &Info;
}

//###############################################################
//###############################################################

/** Comp view for boolan constants.
  * @author Rasmus Diekenbrock
  */


class BooleanConstantView : public CompView
{
public:
	BooleanConstantView(BooleanConstant * comp, eViewType viewType);
//	~BooleanConstantView();
	
	virtual void draw(QPainter * p);
	
	ConnectorBoolOut * getConnector() const { return ((BooleanConstant *)getComponent())->getOutputConnector(); };
	
	void setConnPos();

	
protected:
	
private:	
};

BooleanConstantView::BooleanConstantView(BooleanConstant * comp, eViewType viewType)
	:	CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 3*gridX, 3*gridY));
	enableRotation(true);

	setConnPos();
}


void BooleanConstantView::setConnPos()
{
	if (getComponent()->getInfo() == getBooleanConstantTrue())
	{
		// True
		getConnector()->setGridPos(1,2);
		getConnector()->setOrientation(CO_BOTTOM);
	}
	else
	{
		// False
		getConnector()->setGridPos(1,0);
		getConnector()->setOrientation(CO_TOP);
	}
}



void BooleanConstantView::draw(QPainter * p)
{
	CompView::draw(p);
	
	QPoint pos(0,0);
	
	if (getComponent()->getInfo() == getBooleanConstantTrue())
	{
		#define dia 12
		// True
		p->setPen(QPen(black,2));
		p->setBrush(NoBrush);
		pos.rx() += 3*gridX/2;	// to middle
		pos.ry() += 3*gridY/2 - 2;
		p->drawEllipse(pos.x()-dia/2,pos.y()-dia/2,dia,dia);
		p->drawLine(pos.x(), pos.y()-dia/4, pos.x(), pos.y()+dia/4);
		p->drawLine(pos.x()-dia/4, pos.y(), pos.x()+dia/4, pos.y());
		
		#undef dia
	}
	else
	{
		// False
		p->setPen(QPen(black,2));
		p->setBrush(NoBrush);
		pos.rx() += 3*gridX/2;	// to middle
		pos.ry() += 3*gridY/2;
		p->drawLine(pos.x(), pos.y()-4, pos.x(), pos.y()+4);
		p->drawLine(pos.x()-6, pos.y()+4, pos.x()+6, pos.y()+4);
	}
}

//############################################################################
//############################################################################


BooleanConstant::BooleanConstant(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci)
{
	m_out = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Boolean-Connector", "Output"));
	CHECK_PTR(m_out);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanConstantView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::UPDATEOUTPUT);
	getAction().disable(KSimAction::CALCULATE);
	
}

/*BooleanConstant::~BooleanConstant()
{}*/

void BooleanConstant::reset()
{
	if (getInfo() == getBooleanConstantTrue())
	{
  	getOutputConnector()->setOutput(true);
  }
  else
	{
  	getOutputConnector()->setOutput(false);
  }
}

//###############################################################

}; //namespace KSimLibBoolean

