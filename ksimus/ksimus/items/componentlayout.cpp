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
	if (getParent())
	{
//		KSIMDEBUG("CompLayoutBaseItem::~CompLayoutBaseItem()");
		getParent()->removeItem(this, false);
	}
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
	CompLayoutConnector(CompLayoutBase * parentLayout, ConnectorBase * conn, unsigned int additionalSpace = 1)
		:	CompLayoutBaseItem(parentLayout),
			m_conn(conn),
			m_space(additionalSpace)
	{};
	
	ConnectorBase * getConnector() const { return m_conn;};
	unsigned int getAdditionalSpace() const { return m_space;};
	
	virtual void getSize(unsigned int & size, unsigned int & connectors) const;
	virtual unsigned int getStretchFactor() const;
	virtual unsigned int position(unsigned int & size, unsigned int & sumStretch, QPoint startPos);


private:
	ConnectorBase * m_conn;
	unsigned int m_space;
};

void CompLayoutConnector::getSize(unsigned int & size, unsigned int & connectors) const
{
	if (!getConnector()->isHidden())
	{
		size += 1 + getAdditionalSpace();
		connectors ++;
	}
}

unsigned int CompLayoutConnector::getStretchFactor() const
{
	return 0;
}

unsigned int CompLayoutConnector::position(unsigned int & /*size*/, unsigned int & /*sumStretch*/, QPoint startPos)
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
	
	
	virtual void getSize(unsigned int & size, unsigned int & connectors) const;
	virtual unsigned int getStretchFactor() const;
	virtual unsigned int position(unsigned int & size, unsigned int & sumStretch, QPoint startPos);

private:
	unsigned int m_space;
};

void CompLayoutSpace::getSize(unsigned int & size, unsigned int & /*connectors*/) const
{
	size += getSpace();
}

unsigned int CompLayoutSpace::getStretchFactor() const
{
	return 0;
}

unsigned int CompLayoutSpace::position(unsigned int & /*size*/, unsigned int & /*sumStretch*/, QPoint /*startPos*/)
{
	return getSpace();
}

//#######################################################################
//#######################################################################

class CompLayoutStretch : public CompLayoutBaseItem
{
public:
	CompLayoutStretch(CompLayoutBase * parentLayout, unsigned int factor)
		: CompLayoutBaseItem(parentLayout),
			m_factor(QMAX(1,factor))
	{};
	
	unsigned int getFactor() const { return m_factor;};
	
	virtual void getSize(unsigned int & size, unsigned int & connectors) const;
	virtual unsigned int getStretchFactor() const;
	virtual unsigned int position(unsigned int & size, unsigned int & sumStretch, QPoint startPos);

private:
	unsigned int m_factor;
};

void CompLayoutStretch::getSize(unsigned int & /*size*/, unsigned int & /*connectors*/) const
{
}

unsigned int CompLayoutStretch::getStretchFactor() const
{
	return getFactor();
}

unsigned int CompLayoutStretch::position(unsigned int & size, unsigned int & sumStretch, QPoint /*startPos*/)
{
	if (sumStretch == 0)
	{
		return 0;
		KSIMDEBUG("sumStretch == 0");
	}
	else
	{
		unsigned int stretch = (unsigned int)(((double)(size * getFactor()) / sumStretch) + 0.5);
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
	unsigned int getAdditionalSpace() const { return m_space;};
	
	
	virtual void getSize(unsigned int & size, unsigned int & connectors) const;
	virtual unsigned int getStretchFactor() const;
	virtual unsigned int position(unsigned int & size, unsigned int & sumStretch, QPoint startPos);

private:
	ConnectorPack * m_connPack;
	unsigned int m_space;
};

void CompLayoutConnPack::getSize(unsigned int & size, unsigned int & connectors) const
{
	unsigned int i = 0;
	
	FOR_EACH_CONNECTOR(it, *getConnectorPack()->getConnList())
	{
		if (!it.current()->isHidden())
		{
			i+= 1;
		}
	}
	size += i * ( 1 + getAdditionalSpace());
	connectors += i;
}

unsigned int CompLayoutConnPack::getStretchFactor() const
{
	return 0;
}

unsigned int CompLayoutConnPack::position(unsigned int & /*size*/, unsigned int & /*sumStretch*/, QPoint startPos)
{
	unsigned int i = 0;
	
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
		
		getBaseLayout()->logError(QString::fromLatin1("CompLayoutBase::addLayout Orientation missmatch!"));
	}
	
	getItemList()->append(layout);
}

void CompLayoutBase::addConnector(ConnectorBase * conn, unsigned int additionalSpace = 1)
{
	CompLayoutBaseItem * item = new CompLayoutConnector(this, conn, additionalSpace);
	CHECK_PTR(item);
	
	getItemList()->append(item);
	
	conn->setOrientation(getOrientation());
	QObject::connect(conn, SIGNAL(signalProperty()),getBaseLayout(),SLOT(updateLayout()));
}

void CompLayoutBase::addSpace(unsigned int space)
{
	CompLayoutBaseItem * item = new CompLayoutSpace(this, space);
	CHECK_PTR(item);
	
	getItemList()->append(item);
}

void CompLayoutBase::addStretch(unsigned int factor)
{
	CompLayoutBaseItem * item = new CompLayoutStretch(this, factor);
	CHECK_PTR(item);
	
	getItemList()->append(item);
}

void CompLayoutBase::addConnectorPack(ConnectorPack * connPack, unsigned int space = 1)
{
	CompLayoutBaseItem * item = new CompLayoutConnPack(this, connPack, space);
	CHECK_PTR(item);
	
	connPack->setOrientation(getOrientation());
	
	getItemList()->append(item);
	QObject::connect(connPack, SIGNAL(signalAddConnector(ConnectorBase *)),getBaseLayout(),SLOT(updateLayout()));
	QObject::connect(connPack, SIGNAL(signalDeletedConnector()),getBaseLayout(),SLOT(updateLayout()));
}

bool CompLayoutBase::removeItem(CompLayoutBaseItem * layoutItem, bool del = true)
{
	if (del)
	{
		return getItemList()->removeRef(layoutItem);
	}
	else
	{
		if (-1 == getItemList()->findRef(layoutItem))
		{
			// not found
			return false;
		}
		else
		{
			getItemList()->take();
			return true;
		}
	}
}

void CompLayoutBase::getSize(unsigned int & size, unsigned int & connectors) const
{
	FOR_EACH_LAYOUTITEM(it,*getItemList())
	{
		it.current()->getSize(size, connectors);
	}
}

unsigned int CompLayoutBase::getStretchFactor() const
{
	unsigned int res = 0;
	
	FOR_EACH_LAYOUTITEM(it,*getItemList())
	{
		res += it.current()->getStretchFactor();
	}
	return res;
}

unsigned int CompLayoutBase::position(unsigned int & size, unsigned int & sumStretch, QPoint startPos)
{
	unsigned int res = 0;
	
	FOR_EACH_LAYOUTITEM(it,*getItemList())
	{
		unsigned int offset = it.current()->position(size, sumStretch, startPos);
		addPosition(startPos, offset);
		res += offset;
	}
	return res;
}

void CompLayoutBase::addPosition(QPoint & pos, unsigned int add)
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


ComponentLayout::ComponentLayout(CompView * sheetView)
	:	ComponentAddOn(sheetView->getComponent(), QString::fromLatin1("Component Layout")),
		m_sheetView(sheetView),
		m_minSize(QSize(5,5)),
		m_currentSize(QSize()),
		m_fixedSize(false)
{

	#ifdef DEBUG
		// Should only useful during development!
		if (sheetView == 0)
		{
			KSIMDEBUG(QString::fromLatin1("Add a layout to a compView == NULL"));
			ASSERT(sheetView != 0);
		}
	
		if (sheetView->getViewType() != SHEET_VIEW)
		{
			KSIMDEBUG(QString::fromLatin1("Add a layout to a compView which is not a SHEET_VIEW! (Component %1)").arg(sheetView->getComponent()->getName()));
		}
	#endif // DEBUG
	
	getAction().disable(KSimAction::ALL);
	
	
	m_left   = new CompLayoutBase(this, CO_LEFT);
	CHECK_PTR(m_left);
	m_right  = new CompLayoutBase(this, CO_RIGHT);
	CHECK_PTR(m_right);
	m_top    = new CompLayoutBase(this, CO_TOP);
	CHECK_PTR(m_top);
	m_bottom = new CompLayoutBase(this, CO_BOTTOM);
	CHECK_PTR(m_bottom);


	connect(getSheetView(), SIGNAL(signalResize(const QSize &)),
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
	unsigned int leftSize, rightSize, topSize, bottomSize;
	unsigned int leftConn, rightConn, topConn, bottomConn;
	unsigned int leftSumStretch, rightSumStretch, topSumStretch, bottomSumStretch;
	
	unsigned int sizeX, sizeY;
	
	leftSize = rightSize = topSize = bottomSize = 0;
	leftConn = rightConn = topConn = bottomConn = 0;
	
	
	getLeft()->getSize(leftSize, leftConn);
	leftSumStretch   = getLeft()->getStretchFactor();
	getRight()->getSize(rightSize, rightConn);
	rightSumStretch  = getRight()->getStretchFactor();
	getTop()->getSize(topSize, topConn);
	topSumStretch    = getTop()->getStretchFactor();
	getBottom()->getSize(bottomSize, bottomConn);
	bottomSumStretch = getBottom()->getStretchFactor();

  if (isFixedSize())
  {
  	sizeX = getSheetView()->getPlace().width() / gridX;
  	sizeY = getSheetView()->getPlace().height() / gridY;
  }
  else
  {
		// Component side used?
		getSheetView()->enableConnectorSpacingLeft(leftConn);
		getSheetView()->enableConnectorSpacingRight(rightConn);
		getSheetView()->enableConnectorSpacingTop(topConn);
		getSheetView()->enableConnectorSpacingBottom(bottomConn);
	
	
  	sizeX = QMAX(topSize,bottomSize);
		sizeY = QMAX(leftSize,rightSize);
		
		if (leftConn) sizeX++;
		if (rightConn) sizeX++;
		if (topConn) sizeY++;
		if (bottomConn) sizeY++;

  	sizeX = QMAX(sizeX,(unsigned int)getMinSize().width());
		sizeY = QMAX(sizeY,(unsigned int)getMinSize().height());
		
		m_currentSize = QSize(sizeX * gridX, sizeY * gridY);
		
		getSheetView()->setPlace(QRect(getSheetView()->getPos(), m_currentSize), true);
	}
	
 	unsigned int i;
 	i = sizeY - leftSize;
 	getLeft()->position(i, leftSumStretch, QPoint(0,topConn?1:0));
 	i = sizeY - rightSize;
 	getRight()->position(i, rightSumStretch, QPoint(sizeX-1,topConn?1:0));
 	i = sizeX - topSize;
 	getTop()->position(i, topSumStretch, QPoint(leftConn?1:0,0));
 	i = sizeX - bottomSize;
 	getBottom()->position(i, bottomSumStretch, QPoint(leftConn?1:0,sizeY-1));
 	
 	getComponent()->refresh();
}


void ComponentLayout::slotResizeView()
{
	if (getSheetView()->getPlace().size() != m_currentSize)
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


