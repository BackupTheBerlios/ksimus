/***************************************************************************
                          doublelayout.cpp  -  description
                             -------------------
    begin                : Sat Dec 15 2001
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
//#include <qnamespace.h>

// KDE-Includes

// Project-Includes
#include "doublelayout.h"
#include "componentlayout.h"
#include "ksimdebug.h"
#include "resource.h"

// Forward declaration




//#######################################################################
//#######################################################################

class DoubleLayoutChild : public CompLayoutBase
{
public:
	DoubleLayoutChild(CompLayoutBase * parentLayout, DoubleLayout * doubleLayout);
//	~DoubleLayoutChild();

	
	/** This functions returns the size of the layout element. The size is measured in grid (8 Pixels).
 	  * @param size       Add the size of the layout element to the variable size.
 	  * @param connectors Add the numbers of connectors to the variable connectors. Hidden connectors
 	  *                   are not counted.
	  */
	virtual void getSize(unsigned int & size, unsigned int & connectors) const;
	/** Returns the stretch factor of the layout element.
			DoubleLayoutChild supports no stretching!
	 */
	virtual unsigned int getStretchFactor() const;
	/** Positionized the layout element.
	 *  @param size       The number of grid to distribute for stretch elements.
	 *	@param sumStretch The sum of all stretch factors which have to split the size from above.
	 *  @param startPos   The first position usable by this layout element.
	 *  @return           The number of positions used by this layout element.
	 */
	virtual unsigned int position(unsigned int & size, unsigned int & sumStretch, QPoint startPos);


	DoubleLayout * getDoubleLayout() const { return m_doubleLayout; };
	/** Calls @ref CompLayoutBase::getSize */
	void getSizeCompLayoutBase(unsigned int & size, unsigned int & connectors) const;
	/** Calls @ref CompLayoutBase::getStretchFactor */
	unsigned int getStretchFactorCompLayoutBase() const;


	QPoint getStartPos() const { return m_startPos; };
	QPoint getLastPos() const  { return m_lastPos; };

protected:	
	

private:
	DoubleLayout * m_doubleLayout;
	QPoint m_startPos, m_lastPos;
};

//#######################################################################

DoubleLayoutChild::DoubleLayoutChild(CompLayoutBase * parentLayout, DoubleLayout * doubleLayout)
	: CompLayoutBase(parentLayout, parentLayout->getOrientation()),
		m_doubleLayout(doubleLayout)
{
}

void DoubleLayoutChild::getSize(unsigned int & size, unsigned int & connectors) const
{
	unsigned int dummy;
	
	getSizeCompLayoutBase(dummy, connectors);
	
	getDoubleLayout()->getSizeInternal(size);
}

unsigned int DoubleLayoutChild::getStretchFactor() const
{
	return getDoubleLayout()->getStretchFactorInternal();
}

unsigned int DoubleLayoutChild::position(unsigned int & size, unsigned int & sumStretch, QPoint startPos)
{
	unsigned int usedSize, commonSize = 0;
	m_startPos = m_lastPos = startPos;
	
	usedSize = CompLayoutBase::position(size, sumStretch, startPos);
	
	getDoubleLayout()->getSizeInternal(commonSize);
	
	if (usedSize > commonSize)
	{
		KSIMDEBUG("usedSize > commonSize");
	}
	else
	{
		usedSize = commonSize;
	}
	
	addPosition(m_lastPos, usedSize);
	
	return QMAX(usedSize, commonSize);
}


void DoubleLayoutChild::getSizeCompLayoutBase(unsigned int & size, unsigned int & connectors) const
{
	CompLayoutBase::getSize(size, connectors);
}

unsigned int DoubleLayoutChild::getStretchFactorCompLayoutBase() const
{
	return CompLayoutBase::getStretchFactor();
}


//#######################################################################
//#######################################################################


DoubleLayout::DoubleLayout(ComponentLayout * parentLayout, Qt::Orientation orient)
	: m_parentLayout(parentLayout),
		m_orientation(orient)
{
	CHECK_PTR(m_parentLayout);
	
	if (getOrientation() == Qt::Horizontal)
	{
		m_child1 = new DoubleLayoutChild(getParentLayout()->getLeft(), this);
		CHECK_PTR(m_child1);
		getParentLayout()->getLeft()->addLayout(m_child1);
		m_child2 = new DoubleLayoutChild(getParentLayout()->getRight(), this);
		CHECK_PTR(m_child2);
		getParentLayout()->getRight()->addLayout(m_child2);
	}
	else
	{
		m_child1 = new DoubleLayoutChild(getParentLayout()->getTop(), this);
		CHECK_PTR(m_child1);
		getParentLayout()->getTop()->addLayout(m_child1);
		m_child2 = new DoubleLayoutChild(getParentLayout()->getBottom(), this);
		CHECK_PTR(m_child2);
		getParentLayout()->getBottom()->addLayout(m_child2);
	}
}

DoubleLayout::~DoubleLayout()
{
	delete m_child2;
	delete m_child1;
}

CompLayoutBase * DoubleLayout::getLeft() const
{
	if (getOrientation() == Qt::Vertical)
	{
		KSIMDEBUG("Orientation is vertical. No left layout! Return top layout.");
	}
	return m_child1;
}

CompLayoutBase * DoubleLayout::getRight() const
{
	if (getOrientation() == Qt::Vertical)
	{
		KSIMDEBUG("Orientation is vertical. No right layout! Return bottom layout.");
	}
	return m_child2;
}

CompLayoutBase * DoubleLayout::getTop() const
{
	if (getOrientation() == Qt::Horizontal)
	{
		KSIMDEBUG("Orientation is horizontal. No top layout! Return left layout.");
	}
	return m_child1;
}

CompLayoutBase * DoubleLayout::getBottom() const
{
	if (getOrientation() == Qt::Horizontal)
	{
		KSIMDEBUG("Orientation is horizontal. No bottom layout! Return right layout.");
	}
	return m_child2;
}

void DoubleLayout::getSizeInternal(unsigned int & size) const
{
	unsigned int size1 = 0;
	unsigned int size2 = 0;
	unsigned int dummy = 0;
	
	m_child1->getSizeCompLayoutBase(size1, dummy);
	m_child2->getSizeCompLayoutBase(size2, dummy);
	
	size += QMAX(size1, size2);
}
	
unsigned int DoubleLayout::getStretchFactorInternal() const
{
	unsigned int stretch1;
	unsigned int stretch2;

	stretch1 = m_child1->getStretchFactorCompLayoutBase();
	stretch2 = m_child2->getStretchFactorCompLayoutBase();
	
	return QMAX(stretch1, stretch2);
}

QRect DoubleLayout::getRectGrid(bool withConn) const
{
	QPoint topLeft(m_child1->getStartPos());
	QPoint bottomRight(m_child2->getLastPos());
	
	if (getOrientation() == Qt::Horizontal)
	{
		if (topLeft.y() == bottomRight.y())
		{
			return QRect();
		}
		if (!withConn)
		{
			topLeft.rx() += 1;
		}
		return QRect(topLeft, bottomRight);
	}
	else // getOrientation() == Qt::Vertical
	{
		if (topLeft.x() == bottomRight.x())
		{
			return QRect();
		}
		if (!withConn)
		{
			topLeft.ry() += 1;
		}
		return QRect(topLeft, bottomRight);
	}
}

QRect DoubleLayout::getRect(bool withConn) const
{
	QRect rect(getRectGrid(withConn));
	
	if (rect.isValid())
	{
		rect.rLeft() *= gridY;
		rect.rRight() *= gridY;
		rect.rRight() -= 1;
		rect.rTop() *= gridX;
		rect.rBottom() *= gridX;
		rect.rBottom() -= 1;
	}
	return rect;
}



//#######################################################################
//#######################################################################

