/***************************************************************************
                          complayout.cpp  -  description
                             -------------------
    begin                : Sat Nov 24 2001
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

// KDE-Includes

// Project-Includes
#include "ksimdebug.h"
#include "connectorbase.h"
#include "component.h"
#include "compcontainer.h"
#include "compview.h"
#include "resource.h"
#include "componentlayout.h"
#include "connectorpack.h"

// Forward declaration

//#######################################################################
//#######################################################################

CompLayoutBaseItem::CompLayoutBaseItem(CompLayoutBase * parentLayout)
	:	m_parentLayout(parentLayout)
{
}

CompLayoutBaseItem::~CompLayoutBaseItem()
{
}

ComponentLayout * CompLayoutBaseItem::getBaseLayout() const
{
	return getParent()->getBaseLayout();
}

Component * CompLayoutBaseItem::getComponent() const
{
	return getBaseLayout()->getComponent();
}

//#######################################################################
//#######################################################################

class CompLayoutConnector : public CompLayoutBaseItem
{
public:
	CompLayoutConnector(CompLayoutBase * parentLayout, ConnectorBase * conn, int additionalSpace = 1)
		:	CompLayoutBaseItem(parentLayout),
			m_conn(conn),
			m_space(QMAX(0,additionalSpace))
	{};
	
	ConnectorBase * getConnector() const { return m_conn;};
	int getAdditionalSpace() const { return m_space;};
	
	virtual int getSize() const;
	virtual int getStretchFactor() const;
	virtual int position(int & size, int & sumStretch, QPoint startPos);


private:
	ConnectorBase * m_conn;
	int m_space;
};

int CompLayoutConnector::getSize() const
{
	if (getConnector()->isHidden())
	{
		return 0;
	}
	else
	{
		return 1 + getAdditionalSpace();
	}
}

int CompLayoutConnector::getStretchFactor() const
{
	return 0;
}

int CompLayoutConnector::position(int & /*size*/, int & /*sumStretch*/, QPoint startPos)
{
	if (getConnector()->isHidden())
	{
		getConnector()->setGridPos(QPoint());
		return 0;
	}
	else
	{
		getConnector()->setGridPos(startPos);
		return 1 + getAdditionalSpace();
	}
}

//#######################################################################
//#######################################################################

class CompLayoutSpace : public CompLayoutBaseItem
{
public:
	CompLayoutSpace(CompLayoutBase * parentLayout, int space)
		: CompLayoutBaseItem(parentLayout),
			m_space(QMAX(0,space))
	{};
	
	int getSpace() const { return m_space;};
	
	
	virtual int getSize() const;
	virtual int getStretchFactor() const;
	virtual int position(int & size, int & sumStretch, QPoint startPos);

private:
	int m_space;
};

int CompLayoutSpace::getSize() const
{
	return getSpace();
}

int CompLayoutSpace::getStretchFactor() const
{
	return 0;
}

int CompLayoutSpace::position(int & /*size*/, int & /*sumStretch*/, QPoint /*startPos*/)
{
	return getSpace();
}

//#######################################################################
//#######################################################################

class CompLayoutStretch : public CompLayoutBaseItem
{
public:
	CompLayoutStretch(CompLayoutBase * parentLayout, int factor)
		: CompLayoutBaseItem(parentLayout),
			m_factor(QMAX(1,factor))
	{};
	
	int getFactor() const { return m_factor;};
	
	virtual int getSize() const;
	virtual int getStretchFactor() const;
	virtual int position(int & size, int & sumStretch, QPoint startPos);

private:
	int m_factor;
};

int CompLayoutStretch::getSize() const
{
	return 0;
}

int CompLayoutStretch::getStretchFactor() const
{
	return getFactor();
}

int CompLayoutStretch::position(int & size, int & sumStretch, QPoint /*startPos*/)
{
	if (sumStretch == 0)
	{
		return 0;
		KSIMDEBUG("sumStretch == 0");
	}
	else
	{
		int stretch = (int)(((double)(size * getFactor()) / sumStretch) + 0.5);
		size -= stretch;
		sumStretch -= getFactor();
		return stretch;
	}
}

//#######################################################################
//#######################################################################

class CompLayoutConnPack : public CompLayoutBaseItem
{
public:
	CompLayoutConnPack(CompLayoutBase * parentLayout, ConnectorPack * connPack, int additionalSpace = 1)
		: CompLayoutBaseItem(parentLayout),
			m_connPack(connPack),
		  m_space(QMAX(0,additionalSpace))
	{
	};
	
	ConnectorPack * getConnectorPack() const { return m_connPack;};
	int getAdditionalSpace() const { return m_space;};
	
	
	virtual int getSize() const;
	virtual int getStretchFactor() const;
	virtual int position(int & size, int & sumStretch, QPoint startPos);

private:
	ConnectorPack * m_connPack;
	int m_space;
};

int CompLayoutConnPack::getSize() const
{
	int i = 0;
	
	FOR_EACH_CONNECTOR(it, *getConnectorPack()->getConnList())
	{
		if (!it.current()->isHidden())
		{
			i+= 1;
		}
	}
	return i * ( 1 + getAdditionalSpace());
}

int CompLayoutConnPack::getStretchFactor() const
{
	return 0;
}

int CompLayoutConnPack::position(int & /*size*/, int & /*sumStretch*/, QPoint startPos)
{
	int i = 0;
	
	FOR_EACH_CONNECTOR(it, *getConnectorPack()->getConnList())
	{
		if (!it.current()->isHidden())
		{
			it.current()->setGridPos(startPos);
			i+= 1 + getAdditionalSpace();
			getParent()->addPosition(startPos, 1 + getAdditionalSpace());
		}
	}
	return i;
}



//#######################################################################
//#######################################################################

#define BASE_LAYOUT 0x0001

	
CompLayoutBase::CompLayoutBase(CompLayoutBase * parentLayout, ConnOrientationType orientation)
	:	CompLayoutBaseItem(parentLayout),
		m_orientation(orientation),
		m_flags(0)
{
	m_parent.layout = parentLayout;
	
	m_itemList = new CompLayoutList();
	m_itemList->setAutoDelete(true);
}

CompLayoutBase::CompLayoutBase(ComponentLayout * parent, ConnOrientationType orientation)
	:	CompLayoutBaseItem(0),
		m_orientation(orientation),
		m_flags(BASE_LAYOUT)
{
	m_parent.base = parent;
	
	m_itemList = new CompLayoutList();
	m_itemList->setAutoDelete(true);
}

CompLayoutBase::~CompLayoutBase()
{
	delete m_itemList;
}

CompLayoutBase * CompLayoutBase::getParentLayout() const
{
	if (m_flags & BASE_LAYOUT)
	{
		return 0;
	}
	else
	{
		return m_parent.layout;
	}
};

ComponentLayout * CompLayoutBase::getBaseLayout() const
{
	if (m_flags & BASE_LAYOUT)
	{
		return m_parent.base;
	}
	else
	{
		return m_parent.layout->getBaseLayout();
	}
}

Component * CompLayoutBase::getComponent() const
{
	return getBaseLayout()->getComponent();
}

void CompLayoutBase::addLayout(CompLayoutBase * layout)
{
	if (layout->getOrientation() != getOrientation())
	{
		layout->setOrientation(getOrientation());
		
		getBaseLayout()->logError("CompLayoutBase::addLayout Orientation missmatch!");
	}
	
	getItemList()->append(layout);
}

void CompLayoutBase::addConnector(ConnectorBase * conn, int additionalSpace = 1)
{
	CompLayoutBaseItem * item = new CompLayoutConnector(this, conn, additionalSpace);
	CHECK_PTR(item);
	
	getItemList()->append(item);
	
	conn->setOrientation(getOrientation());
	QObject::connect(conn, SIGNAL(signalProperty()),getBaseLayout(),SLOT(updateLayout()));
}

void CompLayoutBase::addSpace(int space)
{
	CompLayoutBaseItem * item = new CompLayoutSpace(this, space);
	CHECK_PTR(item);
	
	getItemList()->append(item);
}

void CompLayoutBase::addStretch(int factor)
{
	CompLayoutBaseItem * item = new CompLayoutStretch(this, factor);
	CHECK_PTR(item);
	
	getItemList()->append(item);
}

void CompLayoutBase::addConnectorPack(ConnectorPack * connPack, int space = 1)
{
	CompLayoutBaseItem * item = new CompLayoutConnPack(this, connPack, space);
	CHECK_PTR(item);
	
	connPack->setOrientation(getOrientation());
	
	getItemList()->append(item);
	QObject::connect(connPack, SIGNAL(signalAddConnector(ConnectorBase *)),getBaseLayout(),SLOT(updateLayout()));
	QObject::connect(connPack, SIGNAL(signalDeletedConnector()),getBaseLayout(),SLOT(updateLayout()));
}

int CompLayoutBase::getSize() const
{
	int res = 0;
	
	FOR_EACH_LAYOUTITEM(it,*getItemList())
	{
		res += it.current()->getSize();
	}
	return res;
}

int CompLayoutBase::getStretchFactor() const
{
	int res = 0;
	
	FOR_EACH_LAYOUTITEM(it,*getItemList())
	{
		res += it.current()->getStretchFactor();
	}
	return res;
}

int CompLayoutBase::position(int & size, int & sumStretch, QPoint startPos)
{
	int res = 0;
	
	FOR_EACH_LAYOUTITEM(it,*getItemList())
	{
		int offset = it.current()->position(size, sumStretch, startPos);
		addPosition(startPos, offset);
		res += offset;
	}
	return res;
}

void CompLayoutBase::addPosition(QPoint & pos, int add)
{
	switch(getOrientation())
	{
		case CO_TOP:
		case CO_BOTTOM:
			pos.rx() += add;
			break;
		case CO_LEFT:
		case CO_RIGHT:
			pos.ry() += add;
			break;
		
		default:
			KSIMDEBUG_VAR("Unknown orientation", getOrientation());
	}
}

//#######################################################################
//#######################################################################


//#######################################################################
//#######################################################################


ComponentLayout::ComponentLayout(Component * component)
	:	ComponentAddOn(component, QString("Component Layout")),
		m_minSize(QSize(5,5)),
		m_currentSize(QSize()),
		m_fixedSize(false)
{
	
	getAction().disable(KSimAction::ALL);
	
	
	m_left   = new CompLayoutBase(this, CO_LEFT);
	CHECK_PTR(m_left);
	m_right  = new CompLayoutBase(this, CO_RIGHT);
	CHECK_PTR(m_right);
	m_top    = new CompLayoutBase(this, CO_TOP);
	CHECK_PTR(m_top);
	m_bottom = new CompLayoutBase(this, CO_BOTTOM);
	CHECK_PTR(m_bottom);


	connect(getComponent()->getSheetView(), SIGNAL(signalResize(const QSize &)),
					this, SLOT(slotResizeView()));
}


ComponentLayout::~ComponentLayout()
{
	delete m_bottom;
	delete m_top;
	delete m_right;
	delete m_left;
}

void ComponentLayout::updateLayout()
{
	int leftSize, rightSize, topSize, bottomSize;
	int leftSumStretch, rightSumStretch, topSumStretch, bottomSumStretch;
	
	bool leftConn, rightConn, topConn, bottomConn;
	int sizeX, sizeY;
	
	
	// Component side used?
	leftConn = getLeft()->getItemList()->count() != 0;
	rightConn = getRight()->getItemList()->count() != 0;
	topConn = getTop()->getItemList()->count() != 0;
	bottomConn = getBottom()->getItemList()->count() != 0;
	
	getComponent()->getSheetView()->enableConnectorSpacingLeft(leftConn);
	getComponent()->getSheetView()->enableConnectorSpacingRight(rightConn);
	getComponent()->getSheetView()->enableConnectorSpacingTop(topConn);
	getComponent()->getSheetView()->enableConnectorSpacingBottom(bottomConn);
	
	
	leftSize         = getLeft()->getSize();
	leftSumStretch   = getLeft()->getStretchFactor();
	rightSize        = getRight()->getSize();
	rightSumStretch  = getRight()->getStretchFactor();
	topSize          = getTop()->getSize();
	topSumStretch    = getTop()->getStretchFactor();
	bottomSize       = getBottom()->getSize();
	bottomSumStretch = getBottom()->getStretchFactor();

  if (isFixedSize())
  {
  	sizeX = getComponent()->getSheetView()->getPlace().width() / gridX;
  	sizeY = getComponent()->getSheetView()->getPlace().height() / gridY;
  }
  else
  {
  	sizeX = QMAX(topSize,bottomSize);
		sizeY = QMAX(leftSize,rightSize);
		
		if (leftConn) sizeX++;
		if (rightConn) sizeX++;
		if (topConn) sizeY++;
		if (bottomConn) sizeY++;

  	sizeX = QMAX(sizeX,getMinSize().width());
		sizeY = QMAX(sizeY,getMinSize().height());
		
		m_currentSize = QSize(sizeX * gridX, sizeY * gridY);
		
		QRect newPlace(getComponent()->getSheetView()->getPos(), m_currentSize);
		QSize mapSize(getComponent()->getContainer()->getSheetSize());
		
		if (getComponent()->getContainer()->isVisible())
		{
			if (newPlace.bottom() > mapSize.height())
				newPlace.moveBy(0, mapSize.height() - newPlace.bottom());
		
			if (newPlace.right() > mapSize.width())
				newPlace.moveBy(mapSize.width() - newPlace.right(), 0);
		
			if (newPlace.top() < 0)
				newPlace.moveBy(0, - newPlace.top());
		
			if (newPlace.left() < 0)
				newPlace.moveBy( -newPlace.left(), 0);
		}
		
		getComponent()->getSheetView()->setPlace(newPlace);
	}
	
 	int i;
 	i = sizeY - leftSize;
 	getLeft()->position(i, leftSumStretch, QPoint(0,topConn?1:0));
 	i = sizeY - rightSize;
 	getRight()->position(i, rightSumStretch, QPoint(sizeX-1,topConn?1:0));
 	i = sizeX - topSize;
 	getTop()->position(i, topSumStretch, QPoint(leftConn?1:0,0));
 	i = sizeX - bottomSize;
 	getBottom()->position(i, bottomSumStretch, QPoint(leftConn?1:0,sizeY-1));
}


void ComponentLayout::slotResizeView()
{
	if (getComponent()->getSheetView()->getPlace().size() != m_currentSize)
	{
		updateLayout();
	}
}



void ComponentLayout::setMinSize(const QSize & minSize)
{
	m_minSize = minSize;
}
	

void ComponentLayout::setFixedSize(bool fixed)
{
  m_fixedSize = fixed;
}


