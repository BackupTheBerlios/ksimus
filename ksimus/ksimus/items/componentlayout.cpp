/***************************************************************************
                          componentlayout.cpp  -  description
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

//#define NO_KSIMDEBUG 1

// C-Includes
//#include <stdio.h>

// QT-Includes
#include <qpainter.h>

// KDE-Includes

// Project-Includes
#include "ksimdebug.h"
#include "ksimusdoc.h"
#include "connectorbase.h"
#include "component.h"
#include "compcontainer.h"
#include "compview.h"
#include "resource.h"
#include "componentlayout.h"
#include "connectorpack.h"
#include "ksimembfont.h"

// Forward declaration


#define GRID                (isHorizontal() ? gridX : gridY)
#define TO_NEXT_GRID_X(x)   (((x) + gridX - 1) / gridX)
#define TO_NEXT_GRID_Y(y)   (((y) + gridY - 1) / gridY)
#define FROM_GRID_X(x)      ((x) * gridX)
#define FROM_GRID_Y(y)      ((y) * gridY)
#define FIT_TO_GRID(a)      ((((a) + GRID - 1) / GRID ) * GRID)

#if 1
	#define COMP_LAYOUT_DEBUG(var)
#else
	#define COMP_LAYOUT_DEBUG(var)     KSIMDEBUG(var)
#endif



//#######################################################################
//#######################################################################

KSIMDEBUG_INSTANCE_COUNTER(ComponentLayoutItemBase);

ComponentLayoutItemBase::ComponentLayoutItemBase(ComponentLayoutSide * layoutSide)
	:	m_layoutSide(layoutSide),
		m_nextLayoutItem(0)
{
	KSIMDEBUG_ENTRYEXIT;
	
	KSIMASSERT(layoutSide);
	setDirty();  // We need a recalc
	
	KSIMDEBUG_INSTANCE_INC(ComponentLayoutItemBase);
}

ComponentLayoutItemBase::~ComponentLayoutItemBase()
{
	KSIMDEBUG_ENTRYEXIT;
	
	KSIMDEBUG_INSTANCE_DEC(ComponentLayoutItemBase);
}

int ComponentLayoutItemBase::getRequiredSize() const
{
	return 0;
}

int ComponentLayoutItemBase::getStretchFactor() const
{
	return 0;
}

int ComponentLayoutItemBase::getConnectorCount() const
{
	return 0;
}

void ComponentLayoutItemBase::updateLayout(const QPoint & /*pos*/)
{
}

void ComponentLayoutItemBase::preCalc()
{
}

ComponentLayoutBlock * ComponentLayoutItemBase::getLayoutBlock() const
{
	return getSide()->getLayoutBlock();
}

ComponentLayout * ComponentLayoutItemBase::getLayout() const
{
	return getSide()->getLayout();
}

CompView * ComponentLayoutItemBase::getCompView() const
{
	return getLayout()->getCompView();
}

Component * ComponentLayoutItemBase::getComponent() const
{
	return getCompView()->getComponent();
}

void ComponentLayoutItemBase::setDirty()
{
	getLayout()->setDirty();
};


bool ComponentLayoutItemBase::isHorizontal() const
{
	return getLayoutBlock()->isHorizontal();
}


void ComponentLayoutItemBase::setNextLayoutItem(ComponentLayoutItemBase * next)
{
	KSIMASSERT(next);
	KSIMASSERT(m_nextLayoutItem == 0);

	m_nextLayoutItem = next;
}

//#######################################################################
//#######################################################################

ComponentLayoutItemConnector::ComponentLayoutItemConnector(ComponentLayoutSide * layoutSide, ConnectorBase * conn, int addSpace)
	:	ComponentLayoutItemBase(layoutSide),
		m_conn(conn),
		m_addSpace(QMAX(addSpace,0))
{
	KSIMASSERT(conn);
	conn->setOrientation(layoutSide->getOrientation());
//	QObject::connect(conn, SIGNAL(signalProperty()),getLayout(),SLOT(updateLayout()));
	QObject::connect(conn, SIGNAL(signalProperty()),getLayout(),SLOT(setDirty()));
}

int ComponentLayoutItemConnector::getRequiredSize() const
{
	if (m_conn->isHidden())
	{
		return 0;
	}
	return (m_addSpace + 1) * GRID;
}

int ComponentLayoutItemConnector::getConnectorCount() const
{
	return (m_conn->isHidden()) ? 0 : 1;
}

void ComponentLayoutItemConnector::updateLayout(const QPoint & pos)
{
	if (!m_conn->isHidden())
	{
		m_conn->setGridPos(pos.x() / gridX, pos.y() / gridY);
	}
}

//#######################################################################
//#######################################################################

ComponentLayoutItemConnectorPack::ComponentLayoutItemConnectorPack(ComponentLayoutSide * layoutSide, ConnectorPack * connPack, int addSpace)
	:	ComponentLayoutItemBase(layoutSide),
		m_connPack(connPack),
		m_addSpace(QMAX(addSpace,0))
{
	KSIMASSERT(m_connPack);

	connPack->setOrientation(layoutSide->getOrientation());
//	QObject::connect(connPack, SIGNAL(signalAddConnector(ConnectorBase *)),getLayout(),SLOT(updateLayout()));
//	QObject::connect(connPack, SIGNAL(signalDeletedConnector()),getLayout(),SLOT(updateLayout()));
	QObject::connect(connPack, SIGNAL(signalAddConnector(ConnectorBase *)),getLayout(),SLOT(setDirty()));
	QObject::connect(connPack, SIGNAL(signalDeletedConnector()),getLayout(),SLOT(setDirty()));
}

int ComponentLayoutItemConnectorPack::getRequiredSize() const
{
	int i = 0;

	FOR_EACH_CONNECTOR(it, *m_connPack->getConnList())
	{
		if (!it.current()->isHidden())
		{
			i+= 1;
		}
	}
	return i * ( 1 + m_addSpace) * GRID;
}

int ComponentLayoutItemConnectorPack::getConnectorCount() const
{
	return m_connPack->getConnList()->count();
}

void ComponentLayoutItemConnectorPack::updateLayout(const QPoint & pos)
{
	int x = pos.x() / gridX;
	int y = pos.y() / gridY;
	if (getSide()->isHorizontal())
	{
		FOR_EACH_CONNECTOR(it, *m_connPack->getConnList())
		{
			if (!it.current()->isHidden())
			{
				it.current()->setGridPos(x,y);
				x += 1 + m_addSpace;
			}
		}
	}
	else
	{
		FOR_EACH_CONNECTOR(it, *m_connPack->getConnList())
		{
			if (!it.current()->isHidden())
			{
				it.current()->setGridPos(x,y);
				y += 1 + m_addSpace;
			}
		}
	}
}


//#######################################################################
//#######################################################################

ComponentLayoutItemSpace::ComponentLayoutItemSpace(ComponentLayoutSide * layoutSide, int space)
	:	ComponentLayoutItemBase(layoutSide),
		m_space(QMAX(space, 0))
{
}

int ComponentLayoutItemSpace::getRequiredSize() const
{
	return m_space * GRID;
}

//#######################################################################
//#######################################################################

ComponentLayoutItemStretch::ComponentLayoutItemStretch(ComponentLayoutSide * layoutSide, int stretchFactor)
	:	ComponentLayoutItemBase(layoutSide),
		m_stretchFactor(QMAX(stretchFactor,0))
{
}

int ComponentLayoutItemStretch::getStretchFactor() const
{
	return m_stretchFactor;
}

//#######################################################################
//#######################################################################

KSIMDEBUG_INSTANCE_COUNTER(ComponentLayoutSide);

ComponentLayoutSide::ComponentLayoutSide(ComponentLayoutBlock * layoutBlock, ConnOrientationType orientation)
	:	m_layoutBlock(layoutBlock),
		m_firstLayoutItem(0),
		m_orientation(orientation)
{
	KSIMASSERT(layoutBlock);
	
	KSIMDEBUG_INSTANCE_INC(ComponentLayoutSide);
}

ComponentLayoutSide::~ComponentLayoutSide()
{
	const ComponentLayoutItemBase * item = m_firstLayoutItem;
	while(item != 0)
	{
		const ComponentLayoutItemBase * const delItem = item;
		item = item->getNextLayoutItem();
		delete delItem;
	}
	KSIMDEBUG_INSTANCE_DEC(ComponentLayoutSide);
}

void ComponentLayoutSide::addLayoutItem(ComponentLayoutItemBase * newItem)
{
	KSIMASSERT(newItem);

	if (m_firstLayoutItem)
	{
		ComponentLayoutItemBase * item = m_firstLayoutItem;
		while(item->getNextLayoutItem() != 0)
		{
			item = item->getNextLayoutItem();
		}
		item->setNextLayoutItem(newItem);
	}
	else
	{
		m_firstLayoutItem = newItem;
	}
	setDirty();
}


void ComponentLayoutSide::addConnector(ConnectorBase * conn, int addSpace)
{
	KSIMASSERT(conn);
	ComponentLayoutItemBase * newItem = new ComponentLayoutItemConnector(this, conn, addSpace);
	Q_CHECK_PTR(newItem);
	addLayoutItem(newItem);
}

void ComponentLayoutSide::addConnectorPack(ConnectorPack * connPack, int addSpace)
{
	KSIMASSERT(connPack);
	ComponentLayoutItemBase * newItem = new ComponentLayoutItemConnectorPack(this, connPack, addSpace);
	Q_CHECK_PTR(newItem);
	addLayoutItem(newItem);
}

void ComponentLayoutSide::addSpace(int space)
{
	ComponentLayoutItemBase * newItem = new ComponentLayoutItemSpace(this, space);
	Q_CHECK_PTR(newItem);
	addLayoutItem(newItem);
}

void ComponentLayoutSide::addStretch(int stretch)
{
	ComponentLayoutItemBase * newItem = new ComponentLayoutItemStretch(this, stretch);
	Q_CHECK_PTR(newItem);
	addLayoutItem(newItem);
}

int ComponentLayoutSide::getRequiredSize() const
{
	int fixedSize = 0;
	const ComponentLayoutItemBase * item = m_firstLayoutItem;
	while(item != 0)
	{
		fixedSize += item->getRequiredSize();
		item = item->getNextLayoutItem();
	}
	return fixedSize;
}

int ComponentLayoutSide::getStretchFactor() const
{
	int stretch = 0;
	const ComponentLayoutItemBase * item = m_firstLayoutItem;
	while(item != 0)
	{
		stretch += item->getStretchFactor();
		item = item->getNextLayoutItem();
	}
	return stretch;
}

int ComponentLayoutSide::getConnectorCount() const
{
	int connCount = 0;
	const ComponentLayoutItemBase * item = m_firstLayoutItem;
	while(item != 0)
	{
		connCount += item->getConnectorCount();
		item = item->getNextLayoutItem();
	}
	return connCount;
}

bool ComponentLayoutSide::hasConnector() const
{
	const ComponentLayoutItemBase * item = m_firstLayoutItem;
	while(item != 0)
	{
		if (item->getConnectorCount() != 0)
			return true;
		item = item->getNextLayoutItem();
	}
	return false;
}

bool ComponentLayoutSide::isHorizontal() const
{
	bool hori = false;
	switch(getOrientation())
	{
		case CO_TOP:
		case CO_BOTTOM:
			hori = true;
			break;

		case CO_LEFT:
		case CO_RIGHT:
			break;

		default:
			KSIMDEBUG_VAR("Unkown orientation", getOrientation());
			break;
	}
	return hori;
}

void ComponentLayoutSide::updateLayout(const QPoint & startPos, int size) const
{
	const int sumFixedSize = getRequiredSize();
	int availableStretchFactor = getStretchFactor();
	int availableStretch = 0;
	int offset = 0;

	if (size >= sumFixedSize)
	{
		availableStretch = size - sumFixedSize;
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("Need more space as available size = %1 sumFixedSpace = %2").arg(size).arg(sumFixedSize));
	}

	ComponentLayoutItemBase * item = m_firstLayoutItem;
	while(item != 0)
	{
		if (isHorizontal())
		{
			item->updateLayout(startPos + QPoint(offset, 0));
		}
		else
		{
			item->updateLayout(startPos + QPoint(0, offset));
		}
		const int stretchFactor = item->getStretchFactor();
		const int stretch = ((availableStretchFactor !=0) && (availableStretch !=0))
		                    ? FIT_TO_GRID((availableStretch * stretchFactor + availableStretchFactor/2) / availableStretchFactor)
		                    : 0;
		availableStretch -= stretch;
		availableStretchFactor -= stretchFactor;
		offset += stretch + item->getRequiredSize();

		item = item->getNextLayoutItem();
	}
}

void ComponentLayoutSide::preCalc()
{
	ComponentLayoutItemBase * item = m_firstLayoutItem;
	while(item != 0)
	{
		item->preCalc();
		item = item->getNextLayoutItem();
	}
}

void ComponentLayoutSide::setDirty()
{
	getLayout()->setDirty();
};

ComponentLayout * ComponentLayoutSide::getLayout() const
{
	return getLayoutBlock()->getLayout();
}

//#######################################################################
//#######################################################################

KSIMDEBUG_INSTANCE_COUNTER(ComponentLayoutBlockContentBase);

ComponentLayoutBlockContentBase::ComponentLayoutBlockContentBase(ComponentLayoutBlock * parentBlock, int align)
	:	m_parentBlock(parentBlock),
		m_nextContent(0),
		m_align(align)
{
	KSIMASSERT(parentBlock != 0);
	parentBlock->addContent(this);
	setDirty();
	
	KSIMDEBUG_INSTANCE_INC(ComponentLayoutBlockContentBase);
}

ComponentLayoutBlockContentBase::~ComponentLayoutBlockContentBase()
{
	KSIMDEBUG_INSTANCE_DEC(ComponentLayoutBlockContentBase);
}

void ComponentLayoutBlockContentBase::setDirty()
{
	getLayoutBlock()->getLayout()->setDirty();
}

void ComponentLayoutBlockContentBase::setAlign(int align)
{
	m_align = align;
	setDirty();
}

QRect ComponentLayoutBlockContentBase::getContentPlace() const
{
	return getLayoutBlock()->getContentPlace();
}

void ComponentLayoutBlockContentBase::updateLayout(const QRect & /*rect*/)
{
}

void ComponentLayoutBlockContentBase::preCalc()
{
}

//#######################################################################
//#######################################################################

ComponentLayoutBlockContentText::ComponentLayoutBlockContentText(ComponentLayoutBlock * parentBlock)
	:	ComponentLayoutBlockContentBase(parentBlock),
		m_text(QString::null),
		m_rot(00.0),
		m_font(FONT_10)
{
}

ComponentLayoutBlockContentText::ComponentLayoutBlockContentText(ComponentLayoutBlock * parentBlock, const QString & text, int align, double rot, eFont font)
	:	ComponentLayoutBlockContentBase(parentBlock, align),
		m_text(text),
		m_rot(rot),
		m_font(font)
{
}

void ComponentLayoutBlockContentText::setText(const QString & text)
{
	m_text = text;
	setDirty();
}

void ComponentLayoutBlockContentText::setRotation(double rot)
{
	// Only angles 0°, 90°, 180° and 270° are allowed.
	if ((rot <= 45.0) || (rot > 315.0))
	{
		m_rot = 0.0;
	}
	else if (rot <= 135.0)
	{
		m_rot = 90.0;
	}
	else if (rot <= 225.0)
	{
		m_rot = 180.0;
	}
	else
	{
		m_rot = 270.0;
	}
	setDirty();
}

void ComponentLayoutBlockContentText::setFont(eFont font)
{
	m_font = font;
	setDirty();
}

QSize ComponentLayoutBlockContentText::getRequiredSize() const
{
	return KSimEmbFont::getFont(getFont())->getSize(m_rot, getAlign(), getText());
}

void ComponentLayoutBlockContentText::draw(QPainter *p) const
{
	KSIMDEBUG_ENTRYEXIT;
	
	KSimEmbFont::getFont(getFont())->drawText(p, getContentPlace(), m_rot, getAlign(), getText());
}


//#######################################################################
//#######################################################################

ComponentLayoutBlockContentPixmap::ComponentLayoutBlockContentPixmap(ComponentLayoutBlock * parentBlock)
	:	ComponentLayoutBlockContentBase(parentBlock),
		m_xoffs(0),
		m_yoffs(0)
{
}

ComponentLayoutBlockContentPixmap::ComponentLayoutBlockContentPixmap(ComponentLayoutBlock * parentBlock, const QPixmap & pixmap, int align)
	:	ComponentLayoutBlockContentBase(parentBlock, align),
		m_pixmap(pixmap),
		m_xoffs(0),
		m_yoffs(0)
{
}

void ComponentLayoutBlockContentPixmap::setPixmap(const QPixmap & pixmap)
{
	m_pixmap = pixmap;
	setDirty();
}

QSize ComponentLayoutBlockContentPixmap::getRequiredSize() const
{
	return getPixmap().size();
}

void ComponentLayoutBlockContentPixmap::draw(QPainter *p) const
{
	KSIMDEBUG_ENTRYEXIT;
	
	const QRect & r(getContentPlace());
	const int xoffs = ((getAlign() & Qt::AlignRight) == Qt::AlignRight)
	                   ? r.right() - getPixmap().width()                        // Right
	                   : ((getAlign() & Qt::AlignHCenter) == Qt::AlignHCenter)
	                       ? r.left() + (r.width() - getPixmap().width()) / 2   // HCenter
	                       : r.left();                                          // Left

	const int yoffs = ((getAlign() & Qt::AlignBottom) == Qt::AlignBottom)
	                   ? r.bottom() - getPixmap().height()                       // Top
	                   : ((getAlign() & Qt::AlignVCenter) == Qt::AlignVCenter)
	                       ? r.top() + (r.height() - getPixmap().height()) / 2   // VCenter
	                       : r.top();                                            // Bottom
	
	p->drawPixmap(xoffs, yoffs, getPixmap());
}


//#######################################################################
//#######################################################################

KSIMDEBUG_INSTANCE_COUNTER(ComponentLayoutBlock);

const int ComponentLayoutBlock::m_frameSpace = 3;

ComponentLayoutBlock::ComponentLayoutBlock(ComponentLayout * layout)
	:	m_layout(layout),
		m_top(0),
		m_right(0),
		m_bottom(0),
		m_left(0),
		m_nextBlock(0),
		m_firstContent(0),
		m_place(),
		m_minSize(3,3),
		m_stretchFactor(1)

{
	KSIMDEBUG_ENTRYEXIT;
	
	KSIMASSERT(layout != 0);

	m_flags.drawTopBar = 1;
	m_flags.drawRightBar = 1;
	m_flags.drawBottomBar = 1;
	m_flags.drawLeftBar = 1;
	m_flags.horizontal = layout->isHorizontal();
	m_flags.hideWithoutConnector = 1;
	m_flags.withoutConnector = 0;
	
	layout->addBlock(this);
	
	KSIMDEBUG_INSTANCE_INC(ComponentLayoutBlock);
}

ComponentLayoutBlock::~ComponentLayoutBlock()
{
	KSIMDEBUG_ENTRYEXIT;
	
	const ComponentLayoutBlockContentBase * content = m_firstContent;
	while(content != 0)
	{
		const ComponentLayoutBlockContentBase * const delContent = content;
		content = content->m_nextContent;
		delete delContent;
	}
	delete m_left;
	delete m_bottom;
	delete m_right;
	delete m_top;
	
	KSIMDEBUG_INSTANCE_DEC(ComponentLayoutBlock);
}

ComponentLayoutSide * ComponentLayoutBlock::getTop()
{
	if (!m_top)
	{
		m_top = new ComponentLayoutSide(this, CO_TOP);
		Q_CHECK_PTR(m_top);
	}
	return m_top;
}

ComponentLayoutSide * ComponentLayoutBlock::getRight()
{
	if (!m_right)
	{
		m_right = new ComponentLayoutSide(this, CO_RIGHT);
		Q_CHECK_PTR(m_right);
	}
	return m_right;
}

ComponentLayoutSide * ComponentLayoutBlock::getBottom()
{
	if (!m_bottom)
	{
		m_bottom = new ComponentLayoutSide(this, CO_BOTTOM);
		Q_CHECK_PTR(m_bottom);
	}
	return m_bottom;
}
ComponentLayoutSide * ComponentLayoutBlock::getLeft()
{
	if (!m_left)
	{
		m_left = new ComponentLayoutSide(this, CO_LEFT);
		Q_CHECK_PTR(m_left);
	}
	return m_left;
}

void ComponentLayoutBlock::setDirty() const
{
	getLayout()->setDirty();
};

void ComponentLayoutBlock::updateLayout(const QRect & rect)
{
	if (m_top)    m_top->   updateLayout(rect.topLeft()    - QPoint(0,gridY), rect.width());
	if (m_right)  m_right-> updateLayout(rect.topRight()   + QPoint(gridX,0), rect.height());
	if (m_bottom) m_bottom->updateLayout(rect.bottomLeft() + QPoint(0,gridY), rect.width());
	if (m_left)   m_left->  updateLayout(rect.topLeft()    - QPoint(gridX,0), rect.height());
	m_place = rect;
	
	if (m_flags.horizontal)
	{
		if (!m_flags.first)   m_place.rLeft()  -= 1;
		if (!m_flags.last)    m_place.rRight() += 1;
	}
	else
	{
		if (!m_flags.first)   m_place.rTop()    -= 1;
		if (!m_flags.last)    m_place.rBottom() += 1;
	}
	
	ComponentLayoutBlockContentBase * content = m_firstContent;
	while(content != 0)
	{
		content->updateLayout(m_place);
		content = content->m_nextContent;
	}
}


void ComponentLayoutBlock::preCalc()
{
	if (m_top)    m_top->preCalc();
	if (m_right)  m_right->preCalc();
	if (m_bottom) m_bottom->preCalc();
	if (m_left)   m_left->preCalc();
	ComponentLayoutBlockContentBase * content = m_firstContent;
	while(content != 0)
	{
		content->preCalc();
		content = content->m_nextContent;
	}
	
	m_flags.withoutConnector = !hasConnectors();
}

const QRect & ComponentLayoutBlock::getPlace() const
{
	return m_place;
}

QRect ComponentLayoutBlock::getContentPlace() const
{
	// Left 3 pixel space around the border.
	QRect r(getPlace());
	r.rTop() += m_frameSpace;
	r.rLeft() += m_frameSpace;
	r.rBottom() -= m_frameSpace;
	r.rRight() -= m_frameSpace;
	return r;
}

void ComponentLayoutBlock::setMinSize(const QSize & minSize)
{
	if (m_minSize != minSize)
	{
		m_minSize = minSize;
		setDirty();
	}
}

void ComponentLayoutBlock::setMinSize(int width, int height)
{
	setMinSize(QSize(width, height));
}

void ComponentLayoutBlock::setStretch(int stretchFactor)
{
	if (stretchFactor < 0)
	{
		stretchFactor = 0;
	}
	
	if (m_stretchFactor != stretchFactor)
	{
		m_stretchFactor = stretchFactor;
		setDirty();
	}
}

int ComponentLayoutBlock::getStretchFactor() const 
{ 
	if (m_flags.withoutConnector && m_flags.hideWithoutConnector)
	{
		return 0;
	}
	return m_stretchFactor; 
}

QSize ComponentLayoutBlock::getRequiredSize() const
{
	if (m_flags.withoutConnector && m_flags.hideWithoutConnector)
	{
		return QSize(0,0);
	}
	return QSize(FROM_GRID_X(m_minSize.width()), FROM_GRID_Y(m_minSize.height()))
	            .expandedTo(getRequiredSizeSide())
	            .expandedTo(getRequiredSizeContent());
}

QSize ComponentLayoutBlock::getRequiredSizeSide() const
{
	int w = 0;
	int h = 0;

	if (m_top)
	{
		const int i = m_top->getRequiredSize();
		if (i > w) w = i;
	}
	if (m_right)
	{
		const int i = m_right->getRequiredSize();
		if (i > h) h = i;
	}
	if (m_bottom)
	{
		const int i = m_bottom->getRequiredSize();
		if (i > w) w = i;
	}
	if (m_left)
	{
		const int i = m_left->getRequiredSize();
		if (i > h) h = i;
	}
	return QSize(w, h);
}

QSize ComponentLayoutBlock::getRequiredSizeContent() const
{
	QSize size(0,0);
	
	ComponentLayoutBlockContentBase * content = m_firstContent;
	while(content != 0)
	{
		size = size.expandedTo(content->getRequiredSize());
		content = content->m_nextContent;
	}
	size.setWidth(TO_NEXT_GRID_X(size.width() + 2 * m_frameSpace)); 
	size.setHeight(TO_NEXT_GRID_Y(size.height() + 2 * m_frameSpace)); 
	
	return size;
}

void ComponentLayoutBlock::addContent(ComponentLayoutBlockContentBase * content)
{
	KSIMASSERT(content);
	
	setDirty();
	ComponentLayoutBlockContentBase * c = m_firstContent;
	if (c == 0)
	{
		m_firstContent = content;
	}
	else
	{
		while (c->m_nextContent != 0)
		{
			c->setDirty();
			c = c->m_nextContent;
		};
		c->setDirty();
		c->m_nextContent = content;
	}
}

void ComponentLayoutBlock::draw (QPainter *p) const
{
	KSIMDEBUG_ENTRYEXIT;
	
	if (m_flags.withoutConnector && m_flags.hideWithoutConnector)
	{
		return;
	}
	
	p->save();

	p->setPen(Qt::black);
	const QRect r(getPlace());
	p->setBrush(Qt::black);
	
	if (m_flags.drawTopBar)    p->drawRect(r.left(), r.top(), r.width(), 2);
	if (m_flags.drawRightBar)  p->drawRect(r.left()+r.width()-2, r.top(), 2, r.height());
	if (m_flags.drawBottomBar) p->drawRect(r.left(), r.top()+r.height()-2, r.width(), 2);
	if (m_flags.drawLeftBar)   p->drawRect(r.left(), r.top(), 2, r.height());

	drawContent(p);
	
	p->restore();
}

void ComponentLayoutBlock::drawContent(QPainter *p) const
{
	const ComponentLayoutBlockContentBase * content = m_firstContent;
	while(content != 0)
	{
		content->draw(p);
		content = content->m_nextContent;
	}
}


void ComponentLayoutBlock::enableTopBar(bool ena)
{
	m_flags.drawTopBar = ena;
}

void ComponentLayoutBlock::enableRightBar(bool ena)
{
	m_flags.drawRightBar = ena;
}

void ComponentLayoutBlock::enableBottomBar(bool ena)
{
	m_flags.drawBottomBar = ena;
}

void ComponentLayoutBlock::enableLeftBar(bool ena)
{
	m_flags.drawLeftBar = ena;
}

void ComponentLayoutBlock::hideWithoutConnector(bool ena)
{
	m_flags.hideWithoutConnector = ena;
	setDirty();
}

bool ComponentLayoutBlock::hasConnectors() const
{
	if (m_top && m_top->hasConnector())
	{
		return true;
	}
	if (m_right && m_right->hasConnector())
	{
		return true;
	}
	if (m_bottom && m_bottom->hasConnector())
	{
		return true;
	}
	if (m_left && m_left->hasConnector())
	{
		return true;
	}
	return false;
}

//#######################################################################
//#######################################################################

ComponentLayoutControlBlock::ComponentLayoutControlBlock(ComponentLayout * layout)
	:	ComponentLayoutBlock(layout)
{
	KSIMDEBUG_ENTRYEXIT;
}

ComponentLayoutControlBlock::~ComponentLayoutControlBlock()
{
	KSIMDEBUG_ENTRYEXIT;
}

QRect ComponentLayoutControlBlock::getContentPlace() const
{
	QRect r = ComponentLayoutBlock::getContentPlace();
//	r.rBottom() -= gridY;

	return r;
}

QSize ComponentLayoutControlBlock::getRequiredSize() const
{
	if (m_flags.withoutConnector && m_flags.hideWithoutConnector)
	{
		return QSize(0,0);
	}
	return QSize(FROM_GRID_X(m_minSize.width()), FROM_GRID_Y(m_minSize.height()))
	            .expandedTo(getRequiredSizeSide()/* + QSize(0, gridY)*/)
	            .expandedTo(getRequiredSizeContent() + QSize(0, gridY));
}

void ComponentLayoutControlBlock::draw(QPainter *p) const
{

	if (m_flags.withoutConnector && m_flags.hideWithoutConnector)
	{
		return;
	}
	
	QRect rect(getPlace());

	p->save();
	p->setPen(Qt::black);
	p->setBrush(Qt::black);
	const int x = rect.left();
	const int y = rect.top();
	const int w = rect.width();
	const int h = rect.height();
	
	#define INS_W (gridX/2)
	#define INS_H (gridY/2)
	p->drawRect(x,                 y,                  w,         2        );
	
	p->drawRect(x,                 y,                  2,         h - INS_H);
	p->drawRect(x,                 y + h - INS_H - 2,  INS_W + 2, 2        );
	p->drawRect(x + INS_W,         y + h - INS_H - 2,  2,         INS_H + 2);
	
	p->drawRect(x + w - 2,         y,                  2,         h - INS_H);
	p->drawRect(x + w - 2,         y + h - INS_H - 2,  -INS_W,    2        );
	p->drawRect(x + w - 2 - INS_W, y + h - INS_H - 2,  2,         INS_H + 2);
	
	#undef INS_W
	#undef INS_H

	drawContent(p);
	p->restore();
}

//###########################################################################################
//###########################################################################################

static const QString ComponentLayout_AddOnName("Component Layout");

KSIMDEBUG_INSTANCE_COUNTER(ComponentLayout);

ComponentLayout::ComponentLayout(CompView * compView, bool horizontal)
	:	ComponentAddOn(compView->getComponent(), ComponentLayout_AddOnName, true),
		m_compView(compView),
		m_minSize(3,3),
		m_firstBlock(0)
{
	KSIMDEBUG_ENTRYEXIT;
	KSIMASSERT(compView);
	
	m_flags.dirty = 0;
	setDirty();
	m_flags.horizontal = horizontal;
	m_flags.hasTopConn = false;
	m_flags.hasRightConn = false;
	m_flags.hasBottomConn = false;
	m_flags.hasLeftConn = false;
	
	connect(compView, SIGNAL(signalResize(const QSize& )), this, SLOT(setDirty()));
	
	getAction().disable(KSimAction::ALL);
	if (compView->getViewType() == SHEET_VIEW)
	{
		getAction().enable(KSimAction::DRAWSHEETVIEW);
	}
	else if (compView->getViewType() == USER_VIEW)
	{
		getAction().enable(KSimAction::DRAWUSERVIEW);
	}
	KSIMDEBUG_INSTANCE_INC(ComponentLayout);
}

ComponentLayout::~ComponentLayout()
{
	KSIMDEBUG_ENTRYEXIT;
	
	const ComponentLayoutBlock * block = m_firstBlock;
	while(block != 0)
	{
		const ComponentLayoutBlock * const delBlock = block;
		block = block->getNext();
		delete delBlock;
	}
	KSIMDEBUG_INSTANCE_DEC(ComponentLayout);
}

void ComponentLayout::addBlock(ComponentLayoutBlock * pNewBlock)
{
	KSIMDEBUG_ENTRYEXIT;
	KSIMASSERT(pNewBlock);

	pNewBlock->m_flags.last = true;
	if (m_firstBlock)
	{
		ComponentLayoutBlock * pBlock = m_firstBlock;
		while(pBlock->m_nextBlock != 0)
		{
			pBlock = pBlock->m_nextBlock;
		}
		pBlock->m_flags.last = false;
		pBlock->m_nextBlock = pNewBlock;
		pNewBlock->m_flags.first = false;
	}
	else
	{
		m_firstBlock = pNewBlock;
		pNewBlock->m_flags.first = true;
	}
}

void ComponentLayout::setDirty()
{
	KSIMDEBUG_ENTRYEXIT;
	
	if (m_flags.dirty == 0)
	{
		m_flags.dirty = 1;
		connect(getComponent()->getDoc(), SIGNAL(signalPolish()), this, SLOT(updateLayout()));
	}
};

void ComponentLayout::setMinSize(const QSize & size)
{
	KSIMDEBUG_ENTRYEXIT;
	
	m_minSize = size;
	setDirty();
}

QSize ComponentLayout::getRequiredSizeChilds() const
{
	int w = 0;
	int h = 0;
	const ComponentLayoutBlock * pBlock = m_firstBlock;

	if (isHorizontal())
	{
		h = FROM_GRID_Y(m_minSize.height());
		while (pBlock)
		{
			const QSize size(pBlock->getRequiredSize());
			w += size.width();
			if (h < size.height())
				h = size.height();
			pBlock = pBlock->getNext();
		}
	}
	else
	{
		w = FROM_GRID_X(m_minSize.width());
		while (pBlock)
		{
			const QSize size(pBlock->getRequiredSize());
			h += size.height();
			if (w < size.width())
				w = size.width();
			pBlock = pBlock->getNext();
		}
	}

	COMP_LAYOUT_DEBUG(QString("ComponentLayout::getRequiredSizeChilds() w=%1 h=%2").arg(w).arg(h));
	return QSize(w,h);
}

QSize ComponentLayout::getRequiredSize() const
{
	int w = 0;
	int h = 0;
	const ComponentLayoutBlock * pBlock = m_firstBlock;

	if (isHorizontal())
	{
		h = FROM_GRID_Y(m_minSize.height());
		while (pBlock)
		{
			const QSize size(pBlock->getRequiredSize());
			w += size.width();
			if (h < size.height())
				h = size.height();
			pBlock = pBlock->getNext();
		}
		if (w < FROM_GRID_X(m_minSize.width()))
			w = FROM_GRID_X(m_minSize.width());
	}
	else
	{
		w = FROM_GRID_X(m_minSize.width());
		while (pBlock)
		{
			const QSize size(pBlock->getRequiredSize());
			h += size.height();
			if (w < size.width())
				w = size.width();
			pBlock = pBlock->getNext();
		}
		if (h < FROM_GRID_Y(m_minSize.height()))
			h = FROM_GRID_Y(m_minSize.height());
	}

	COMP_LAYOUT_DEBUG(QString("ComponentLayout::getRequiredSize() w=%1 h=%2").arg(w).arg(h));
	return QSize(w,h);
}

int ComponentLayout::getAllStretch() const
{
	int stretch = 0;
	const ComponentLayoutBlock * pBlock = m_firstBlock;
	while (pBlock)
	{
		stretch += pBlock->getStretchFactor();
		pBlock = pBlock->getNext();
	}
	//KSIMDEBUG(QString("ComponentLayout::getAllStretch() stretch=%1").arg(stretch));
	return stretch;
}

void ComponentLayout::updateConnectorSpacings()
{
	KSIMDEBUG_ENTRYEXIT;

	getCompView()->enableConnectorSpacingTop   (hasConnectorTop());
	getCompView()->enableConnectorSpacingRight (hasConnectorRight());
	getCompView()->enableConnectorSpacingBottom(hasConnectorBottom());
	getCompView()->enableConnectorSpacingLeft  (hasConnectorLeft());
}


void ComponentLayout::updateLayout()
{
	KSIMDEBUG_ENTRYEXIT;
	
	KSIMASSERT(m_flags.dirty == 1);
	
	if (m_flags.dirty == 1)
	{
		m_flags.dirty = 0;
		bool res = disconnect(getComponent()->getDoc(), SIGNAL(signalPolish()), this, SLOT(updateLayout()));
		KSIMASSERT(res);
	}
	
	preCalc();

	const QSize requiredSize(getRequiredSizeChilds());

	QSize compSize(FROM_GRID_X(m_minSize.width()), FROM_GRID_X(m_minSize.height()));
	if (compSize.rwidth() < requiredSize.width())
		compSize.rwidth() = requiredSize.width();
	if (compSize.rheight() < requiredSize.height())
		compSize.rheight() = requiredSize.height();
		
	QSize newCompSize = compSize;

	if (hasConnectorTop())    newCompSize.rheight() += gridY;
	if (hasConnectorRight())  newCompSize.rwidth() += gridX;
	if (hasConnectorBottom()) newCompSize.rheight() += gridY;
	if (hasConnectorLeft())   newCompSize.rwidth() += gridX;

//	KSIMDEBUG(QString("ComponentLayout::updateLayout() compSize.w=%1 h=%2").arg(compSize.width()).arg(compSize.height()));
	getCompView()->setPlace(QRect(getCompView()->getPos(), newCompSize), true);
	updateConnectorSpacings();
	
	calcLayout(compSize, requiredSize);
}

void ComponentLayout::calcLayout(const QSize & size, const QSize & requiredSize)
{
	COMP_LAYOUT_DEBUG(QString("ComponentLayout::calcLayout size w=%1 h=%2").arg(size.width()).arg(size.height()));
	COMP_LAYOUT_DEBUG(QString("ComponentLayout::calcLayout requiredSize w=%1 h=%2").arg(requiredSize.width()).arg(requiredSize.height()));
	
	ComponentLayoutBlock * pBlock = m_firstBlock;
	int availableStretchFactor = getAllStretch();
	
	if (isHorizontal())
	{
		int x = hasConnectorLeft() ? gridX : 0;
		const int y = hasConnectorTop() ? gridY : 0;
		int availableStretch = QMAX(0, size.width() - requiredSize.width());
		while (pBlock)
		{
			const int stretchFactor = pBlock->getStretchFactor();
			const int stretch = ((availableStretchFactor !=0) && (availableStretch !=0))
			                    ? FIT_TO_GRID((availableStretch * stretchFactor + availableStretchFactor/2) / availableStretchFactor)
			                    : 0;
			availableStretch -= stretch;
			availableStretchFactor -= stretchFactor;
			const int width = pBlock->getRequiredSize().width() + stretch;
			QRect rect(x, y, width, size.height());
			pBlock->updateLayout(rect);
			x += width;
			pBlock = pBlock->getNext();
		}
	}
	else
	{
		const int x = hasConnectorLeft() ? gridX : 0;
		int y = hasConnectorTop() ? gridY : 0;
		int availableStretch = QMAX(0, size.height() - requiredSize.height());
		COMP_LAYOUT_DEBUG(QString("ComponentLayout::updateLayout() availableStretch=%1 availableStretchFactor=%2").arg(availableStretch).arg(availableStretchFactor));
		COMP_LAYOUT_DEBUG(QString("ComponentLayout::updateLayout() y=%1").arg(y));
		while (pBlock)
		{
			const int stretchFactor = pBlock->getStretchFactor();
			const int stretch = ((availableStretchFactor !=0) && (availableStretch !=0))
			                    ? FIT_TO_GRID((availableStretch * stretchFactor + availableStretchFactor/2) / availableStretchFactor)
			                    : 0;
			availableStretch -= stretch;
			availableStretchFactor -= stretchFactor;
			COMP_LAYOUT_DEBUG(QString("ComponentLayout::updateLayout() loop availableStretch=%1 availableStretchFactor=%2").arg(availableStretch).arg(availableStretchFactor));
			COMP_LAYOUT_DEBUG(QString("ComponentLayout::updateLayout() loop stretch=%1").arg(stretch));
			const int height = pBlock->getRequiredSize().height() + stretch;
			QRect rect(x, y, size.width(), height);
			pBlock->updateLayout(rect);
			y += height;
			COMP_LAYOUT_DEBUG(QString("ComponentLayout::updateLayout() y=%1").arg(y));
			COMP_LAYOUT_DEBUG(QString("ComponentLayout::updateLayout() height=%1").arg(height));
			pBlock = pBlock->getNext();
		}
	}
}


void ComponentLayout::preCalc()
{
	KSIMDEBUG_ENTRYEXIT;
	
	m_flags.hasTopConn = false;
	m_flags.hasRightConn = false;
	m_flags.hasBottomConn = false;
	m_flags.hasLeftConn = false;
	
	ComponentLayoutBlock * pBlock = m_firstBlock;
	while (pBlock)
	{
		pBlock->preCalc();
		
		if ( !m_flags.hasTopConn 
		  && pBlock->hasTop()
		  && pBlock->getTop()->hasConnector() )
		{
			m_flags.hasTopConn = true;
		}
		
		if ( !m_flags.hasRightConn 
		  && pBlock->hasRight()
		  && pBlock->getRight()->hasConnector() )
		{
			m_flags.hasRightConn = true;
		}
		
		if ( !m_flags.hasBottomConn 
		  && pBlock->hasBottom()
		  && pBlock->getBottom()->hasConnector() )
		{
			m_flags.hasBottomConn = true;
		}
		
		if ( !m_flags.hasLeftConn 
		  && pBlock->hasLeft()
		  && pBlock->getLeft()->hasConnector() )
		{
			m_flags.hasLeftConn = true;
		}
		
		pBlock = pBlock->getNext();
	}
}

void ComponentLayout::drawSheetView (QPainter *p) const
{
	KSIMDEBUG_ENTRYEXIT;
	KSIMASSERT(p);
	
	ComponentLayoutBlock * pBlock = m_firstBlock;
	while (pBlock)
	{
		pBlock->draw(p);
		pBlock = pBlock->getNext();
	}
}

void ComponentLayout::drawUserView (QPainter *p) const
{
	KSIMDEBUG_ENTRYEXIT;
	KSIMASSERT(p);
	
	ComponentLayoutBlock * pBlock = m_firstBlock;
	while (pBlock)
	{
		pBlock->draw(p);
		pBlock = pBlock->getNext();
	}
}

//###########################################################################################
//###########################################################################################

ComponentLayoutFixed::ComponentLayoutFixed(CompView * compView, bool horizontal)
	:	ComponentLayout(compView, horizontal)
{
}

void ComponentLayoutFixed::updateLayout()
{
	KSIMDEBUG_ENTRYEXIT;
	
	KSIMASSERT(m_flags.dirty == 1);
	
	if (m_flags.dirty == 1)
	{
		m_flags.dirty = 0;
		bool res = disconnect(getComponent()->getDoc(), SIGNAL(signalPolish()), this, SLOT(updateLayout()));
		KSIMASSERT(res);
	}
	
	preCalc();

	const QSize requiredSize(getRequiredSizeChilds());
	
	QSize compSize(getCompView()->getPlace().size());
	
	// Remove connector spacings
	if (hasConnectorTop())    compSize.rheight() -= gridY;
	if (hasConnectorRight())  compSize.rwidth() -= gridX;
	if (hasConnectorBottom()) compSize.rheight() -= gridY;
	if (hasConnectorLeft())   compSize.rwidth() -= gridX;
	
	if ( (requiredSize.height() > compSize.height())
	  || (requiredSize.width() > compSize.width()) )
	{
		compSize = requiredSize.expandedTo(compSize);
	}
	
	QSize newCompSize(compSize);
	if (hasConnectorTop())    newCompSize.rheight() += gridY;
	if (hasConnectorRight())  newCompSize.rwidth() += gridX;
	if (hasConnectorBottom()) newCompSize.rheight() += gridY;
	if (hasConnectorLeft())   newCompSize.rwidth() += gridX;
	getCompView()->setPlace(QRect(getCompView()->getPos(), newCompSize), true);
	
	updateConnectorSpacings();
	
	calcLayout(compSize, requiredSize);
}

//###########################################################################################
//###########################################################################################

ComponentLayoutSimple::ComponentLayoutSimple(CompView * compView)
	:	ComponentLayout(compView, false)
{
	m_block = new ComponentLayoutBlock(this);
	Q_CHECK_PTR(m_block);
}


//###########################################################################################
//###########################################################################################

ComponentLayoutVerticalCtrl::ComponentLayoutVerticalCtrl(CompView * compView)
	:	ComponentLayout(compView, false)
{
	m_ctrlBlock = new ComponentLayoutControlBlock(this);
	Q_CHECK_PTR(m_ctrlBlock);
	m_funcBlock = new ComponentLayoutBlock(this);
	Q_CHECK_PTR(m_funcBlock);
}


//###########################################################################################
//###########################################################################################



#undef COMP_LAYOUT_DEBUG

#undef GRID
#undef TO_NEXT_GRID_X
#undef TO_NEXT_GRID_Y
#undef FROM_GRID_X
#undef FROM_GRID_Y
#undef FIT_TO_GRID

#include "componentlayout.moc"
