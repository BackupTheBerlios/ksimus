/***************************************************************************
                          componentlayout.h  -  description
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

#ifndef COMPLAYOUT_H
#define COMPLAYOUT_H

// C-Includes

// QT-Includes
#include <qstring.h>
#include <qpixmap.h>

// KDE-Includes

// Project-Includes
#include "types.h"
#include "componentaddon.h"


// Forward declaration
class QPainter;
class ConnectorBase;
class ConnectorPack;
class CompView;
class Component;
class ComponentLayout;
class ComponentLayoutBlock;
class ComponentLayoutBlockContentBase;
class ComponentLayoutItemBase;
class ComponentLayoutSide;



//#######################################################################
//#######################################################################

/** The base class which contains the simplest layout items like connectors, spacings or what else.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutItemBase
{

friend class ComponentLayoutSide;

public:
	/** The destructor */
	virtual ~ComponentLayoutItemBase();

protected:
	/** The protected constructor.
	  *
	  * @param layoutSide The parent layout. This layout has to be a ComponentLayoutSide layout. */
	ComponentLayoutItemBase(ComponentLayoutSide * layoutSide);
	/** Returns the number of grids which this item needs. This implementation returns 0. */
	virtual int getRequiredSize() const;
	/** Returns the stretch factor of this item. This implementation returns 0. */
	virtual int getStretchFactor() const;
	/** Returns the number of connectors of this item. This implementation returns 0. */
	virtual int getConnectorCount() const;
	/** Sets the positions of the connector contained by this item. */
	virtual void updateLayout(const QPoint & pos);
	/* Call first if layout update is started. Used to preclalculate local properties. */
	virtual void preCalc();

	/** Returns the "layout side". */
	ComponentLayoutSide * getSide() const { return m_layoutSide; };
	/** Returns the layout block. */
	ComponentLayoutBlock * getLayoutBlock() const;
	/** Returns the component layout. */
	ComponentLayout * getLayout() const;
	/** Returns the component view. */
	CompView * getCompView() const;
	/** Returns the component. */
	Component * getComponent() const;

	/** Sets the layout dirty. The positions will be calculated before next drawing. */
	void setDirty();
	/** Returns true if the layout has a horizontal orientation */
	bool isHorizontal() const;

	/** Returns the next layout item. This is part of the list implementation of layout items. */
	ComponentLayoutItemBase * getNextLayoutItem() const { return m_nextLayoutItem; };
	/** Sets the next layout item. */
	void setNextLayoutItem(ComponentLayoutItemBase * next);

private:
	/** The parent layout side . */
	ComponentLayoutSide * m_layoutSide;
	/** The next item in the list. */
	ComponentLayoutItemBase * m_nextLayoutItem;
};


//#######################################################################
//#######################################################################

/** The layout item which represents a conntector.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutItemConnector : public ComponentLayoutItemBase
{

public:
	/** The Constructor.
	  * @param layoutSide The parent layout side.
	  * @param conn       The connector to add to the layout.
	  * @param addSpace   Additional space after the connector (default = 1). */
	ComponentLayoutItemConnector(ComponentLayoutSide * layoutSide, ConnectorBase * conn, int addSpace = 1);

protected:
	/** Returns the number of grids which this item needs. */
	virtual int getRequiredSize() const;
	/** Returns the number of connectors of this item. */
	virtual int getConnectorCount() const;
	/** Sets the positions of the connector contained by this item. */
	virtual void updateLayout(const QPoint & pos);

private:
	ConnectorBase * m_conn;
	int m_addSpace;
};

//#######################################################################
//#######################################################################

/** The layout item which represents a conntector pack.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutItemConnectorPack : public ComponentLayoutItemBase
{

public:
	/** The Constructor.
	  * @param layoutSide The parent layout side.
	  * @param connPack   The connector pack to add to the layout.
	  * @param addSpace   Additional space after the connector (default = 1). */
	ComponentLayoutItemConnectorPack(ComponentLayoutSide * layoutSide, ConnectorPack * connPack, int addSpace = 1);

protected:
	/** Returns the number of grids which this item needs. */
	virtual int getRequiredSize() const;
	/** Returns the number of connectors of this item. This implementation returns 0. */
	virtual int getConnectorCount() const;
	/** Sets the positions of the connector contained by this item. */
	virtual void updateLayout(const QPoint & pos);

private:
	ConnectorPack * m_connPack;
	int m_addSpace;
};

//#######################################################################
//#######################################################################

/** The layout item which represents a simple fixed size space between two layout items.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutItemSpace : public ComponentLayoutItemBase
{
public:
	/** The Constructor.
	  * @param layoutSide The parent layout side.
	  * @param space      The number of grids to add between the last item and the next. */
	ComponentLayoutItemSpace(ComponentLayoutSide * layoutSide, int space);

protected:
	/** Returns the number of grids which this item needs.*/
	virtual int getRequiredSize() const;

private:
	int m_space;
};

//#######################################################################
//#######################################################################

/** The layout item which represents a stretchable space between two layout items.
  * A larger stretch factor results in a larger part of the size which is avaliable for stretching.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutItemStretch : public ComponentLayoutItemBase
{
public:
	/** The Constructor.
	  * @param layoutSide The parent layout side.
	  * @param stretch    The stretch factor of the item. */
	ComponentLayoutItemStretch(ComponentLayoutSide * layoutSide, int stretchFactor);

protected:
	/** Returns the stretch factor of this item. */
	virtual int getStretchFactor() const;

private:
	int m_stretchFactor;
};

//#######################################################################
//#######################################################################

class ComponentLayoutSide
{
friend class ComponentLayoutBlock;

public:
	ComponentLayoutSide(ComponentLayoutBlock * layoutBlock, ConnOrientationType orientation);
	~ComponentLayoutSide();

	void addLayoutItem(ComponentLayoutItemBase * newItem);
	void addConnector(ConnectorBase * conn, int addSpace = 1);
	void addConnectorPack(ConnectorPack * connPack, int addSpace = 1);
	void addSpace(int space);
	void addStretch(int stretch);

	/** Returns the number of connectors of this item. */
	int getConnectorCount() const;
	bool hasConnector() const;
	bool isHorizontal() const;


	ComponentLayout * getLayout() const;
	ComponentLayoutBlock * getLayoutBlock() const { return m_layoutBlock; };
	ConnOrientationType getOrientation() const { return m_orientation; };
	void setDirty();

protected:
	void updateLayout(const QPoint & startPos, int size) const;
	/* Call first if layout update is started. Used to preclalculate local properties. */
	void preCalc();
	int getRequiredSize() const;
	int getStretchFactor() const;

private:
	ComponentLayoutBlock * m_layoutBlock;
	ComponentLayoutItemBase * m_firstLayoutItem;
	ConnOrientationType m_orientation;
};

//#######################################################################
//#######################################################################

class ComponentLayoutBlockContentBase
{
friend class ComponentLayoutBlock;
public:
	virtual ~ComponentLayoutBlockContentBase();

	/** Sets the layout dirty. */
	void setDirty();
	/** Set the alignment of the drawn text or pixmap. */
	void setAlign(int align);
	/** Returns the alignment of the drawn text or pixmap. */
	int getAlign() const { return m_align; };

	ComponentLayoutBlock * getLayoutBlock() const { return m_parentBlock; };

protected:
	ComponentLayoutBlockContentBase(ComponentLayoutBlock * parentBlock, int align = Qt::AlignCenter);
	/* Call first if layout update is started. Used to preclalculate local properties. */
	virtual void preCalc();
	/** Reutrns the minimum required size. */
	virtual QSize getRequiredSize() const = 0;
	/** Returns the rect where the content has to be drawn. */
	virtual QRect getContentPlace() const;
	/** Draws the content. */
	virtual void draw(QPainter *p) const = 0;
	virtual void updateLayout(const QRect & rect);

	ComponentLayoutBlock * m_parentBlock;
	ComponentLayoutBlockContentBase * m_nextContent;
	int m_align;
};

//#######################################################################
//#######################################################################

class ComponentLayoutBlockContentText : public ComponentLayoutBlockContentBase
{
public:
	ComponentLayoutBlockContentText(ComponentLayoutBlock * parentBlock);
	ComponentLayoutBlockContentText(ComponentLayoutBlock * parentBlock, const QString & text, int align = Qt::AlignCenter, double rot = 0.0, eFont = FONT_10);

	/** Set a text which will be drawn in the frame.  A previously set pixmap will be discarded. */
	void setText(const QString & text);
	/** Returns the text which will be drawn in the frame. */
	const QString & getText() const { return m_text; };

	/** Sets the rotation of the text. Only 0, 90, 180 and 270 is allowed! */
	void setRotation(double rot);
	/** Returns the text rotation. */
	double getRotation() const { return m_rot; };

	/** Selects an embedded font. */
	void setFont(eFont font);
	/** Returns the selected embedded font. */
	eFont getFont() const { return m_font; };


protected:
	/** Reutrns the minimum required size. */
	virtual QSize getRequiredSize() const;
	/** Draws the content. */
	virtual void draw(QPainter *p) const;

	QString m_text;
	double m_rot;
	eFont m_font;
};

//#######################################################################
//#######################################################################

class ComponentLayoutBlockContentPixmap : public ComponentLayoutBlockContentBase
{
public:
	ComponentLayoutBlockContentPixmap(ComponentLayoutBlock * parentBlock);
	ComponentLayoutBlockContentPixmap(ComponentLayoutBlock * parentBlock, const QPixmap & pixmap, int align = Qt::AlignCenter);

	/** Set a pixmap which will be drawn in the frame.  A previously set pixmap will be discarded. */
	void setPixmap(const QPixmap & pixmap);
	/** Returns the pixmap which will be drawn in the frame. */
	const QPixmap & getPixmap() const { return m_pixmap; };

protected:
	/** Reutrns the minimum required size. */
	virtual QSize getRequiredSize() const;
	/** Draws the content. */
	virtual void draw(QPainter *p) const;

	QPixmap m_pixmap;
	int m_xoffs;
	int m_yoffs;
};

//#######################################################################
//#######################################################################

class ComponentLayoutBlock
{
friend class ComponentLayout;
friend class ComponentLayoutBlockContentBase;
public:
	ComponentLayoutBlock(ComponentLayout * layout);
	virtual ~ComponentLayoutBlock();

	ComponentLayout * getLayout() const { return m_layout; };
	ComponentLayoutBlock * getNext() const { return m_nextBlock; };

	const QRect & getPlace() const;

	void setDirty() const;

	ComponentLayoutSide * getTop();
	ComponentLayoutSide * getRight();
	ComponentLayoutSide * getBottom();
	ComponentLayoutSide * getLeft();

	bool hasTop() const    { return (m_top != 0); };
	bool hasRight() const  { return (m_right != 0); };
	bool hasBottom() const { return (m_bottom != 0); };
	bool hasLeft() const   { return (m_left != 0); };

	void setMinSize(const QSize & minSize);
	void setMinSize(int width, int height);
	QSize getMinSize() const { return m_minSize; };
	void setStretch(int stretchFactor);
	int getStretchFactor() const;

	/** Enables the drawing of the top bar. */
	void enableTopBar(bool ena);
	/** Returns true, if the drawing of the top bar is enabled. */
	bool isTopBarEnabled() const { return m_flags.drawTopBar; };

	/** Enables the drawing of the right bar. */
	void enableRightBar(bool ena);
	/** Returns true, if the drawing of the right bar is enabled. */
	bool isRightBarEnabled() const { return m_flags.drawRightBar; };

	/** Enables the drawing of the bottom bar. */
	void enableBottomBar(bool ena);
	/** Returns true, if the drawing of the bottom bar is enabled. */
	bool isBottomBarEnabled() const { return m_flags.drawBottomBar; };

	/** Enables the drawing of the left bar. */
	void enableLeftBar(bool ena);
	/** Returns true, if the drawing of the left bar is enabled. */
	bool isLeftBarEnabled() const { return m_flags.drawLeftBar; };

	/** Hides the block if ena is true and the block contains no visible connector. */
	void hideWithoutConnector(bool ena);
	/** Returns true, if a empty block will be hidden. */
	bool isHideWithoutConnector() const { return m_flags.hideWithoutConnector; };

	bool isHorizontal() const { return m_flags.horizontal; };
	
	bool hasConnectors() const;

protected:
	void addContent(ComponentLayoutBlockContentBase * content);
	/* Call first if layout update is started. Used to preclalculate local properties. */
	virtual void preCalc();
	virtual QSize getRequiredSize() const;
	QSize getRequiredSizeSide() const;
	QSize getRequiredSizeContent() const;
	/** Returns the rect where the content has to be drawn. */
	virtual QRect getContentPlace() const;
	virtual void updateLayout(const QRect & rect);
	virtual void draw(QPainter *p) const;
	/** Draws the content. */
	void drawContent(QPainter *p) const;
	
	static const int m_frameSpace;
	/** Contains the parent layout. */
	ComponentLayout * m_layout;
	ComponentLayoutSide * m_top;
	ComponentLayoutSide * m_right;
	ComponentLayoutSide * m_bottom;
	ComponentLayoutSide * m_left;
	ComponentLayoutBlock * m_nextBlock;
	ComponentLayoutBlockContentBase * m_firstContent;
	QRect m_place;
	QSize m_minSize;
	int m_stretchFactor;
	/** Some internal flags. */
	struct
	{
		unsigned int drawTopBar           :1;
		unsigned int drawRightBar         :1;
		unsigned int drawBottomBar        :1;
		unsigned int drawLeftBar          :1;
		unsigned int horizontal           :1;
		unsigned int first                :1;
		unsigned int last                 :1;
		unsigned int hideWithoutConnector :1;
		unsigned int withoutConnector     :1;
	} m_flags;
};


//#######################################################################
//#######################################################################

class ComponentLayoutControlBlock : public ComponentLayoutBlock 
{
public:
	ComponentLayoutControlBlock(ComponentLayout * layout);
	virtual ~ComponentLayoutControlBlock();
	
protected:
	/** Returns the rect where the content has to be drawn. */
	virtual QRect getContentPlace() const;
	virtual QSize getRequiredSize() const;
	virtual void draw(QPainter *p) const;
};

//###########################################################################################
//###########################################################################################

class ComponentLayout : public ComponentAddOn
{
friend class ComponentLayoutBlock;

  Q_OBJECT

public:
	/** Constructs a component layout.
	  *
	  * Construct a component layout, add connectors and other layout elements and forgot it :).
	  * @param component Component for which the layout is.
	  */
	ComponentLayout(CompView * compView, bool horizontal);
	/** The destructor. Deletes also all added layout items. */
	~ComponentLayout();

	QSize getMinSize() const { return m_minSize; };
	void setMinSize(const QSize & size);
	void setMinSize(unsigned int width, unsigned int height) { setMinSize(QSize(width, height)); };

	bool isDirty() const { return m_flags.dirty; };
	bool isHorizontal() const { return m_flags.horizontal; };

	CompView * getCompView() const { return m_compView; };

	/** Draw the sheet view.
		* The default implementation does nothing.
		*/
	virtual void drawSheetView (QPainter *p) const;
	/** Draw the user view.
		* The default implementation does nothing.
		*/
	virtual void drawUserView (QPainter *p) const;


public slots:
	void setDirty();
	virtual void updateLayout();

protected:
	void addBlock(ComponentLayoutBlock * pNewBlock);
	/* Call first if layout update is started. Used to preclalculate local properties. */
	void preCalc();
	QSize getRequiredSizeChilds() const;
	QSize getRequiredSize() const;
	int getAllStretch() const;
	bool hasConnectorTop() const { return m_flags.hasTopConn; };
	bool hasConnectorRight() const { return m_flags.hasRightConn; };
	bool hasConnectorBottom() const { return m_flags.hasBottomConn; };
	bool hasConnectorLeft() const { return m_flags.hasLeftConn; };
	
	void updateConnectorSpacings();
	void calcLayout(const QSize & size, const QSize & requiredSizeSize);
	
	CompView * m_compView;
	QSize m_minSize;
	struct
	{
		unsigned int dirty : 1;
		unsigned int horizontal : 1;
		unsigned int hasTopConn : 1;
		unsigned int hasRightConn : 1;
		unsigned int hasBottomConn : 1;
		unsigned int hasLeftConn : 1;
	} m_flags;
	ComponentLayoutBlock * m_firstBlock;
};

//###########################################################################################
//###########################################################################################

class ComponentLayoutFixed : public ComponentLayout
{
  Q_OBJECT

public:
	/** Constructs a component layout.
	  *
	  * Construct a component layout, add connectors and other layout elements and forgot it :).
	  * @param component Component for which the layout is.
	  */
	ComponentLayoutFixed(CompView * compView, bool horizontal);

public slots:
	virtual void updateLayout();
};

//###########################################################################################
//###########################################################################################

class ComponentLayoutSimple : public ComponentLayout
{
  Q_OBJECT

public:
	/** Constructs a component layout.
	  *
	  * Construct a component layout, add connectors and other layout elements and forget it :).
	  * @param component Component for which the layout is.
	  */
	ComponentLayoutSimple(CompView * compView);

	ComponentLayoutBlock * getBlock() { return m_block; };
	
	ComponentLayoutSide * getTop() { return m_block->getTop(); };
	ComponentLayoutSide * getRight() { return m_block->getRight(); };
	ComponentLayoutSide * getBottom() { return m_block->getBottom(); };
	ComponentLayoutSide * getLeft() { return m_block->getLeft(); };

private:
	ComponentLayoutBlock * m_block;

};

//###########################################################################################
//###########################################################################################

class ComponentLayoutVerticalCtrl : public ComponentLayout
{
  Q_OBJECT

public:
	/** Constructs a component layout with vertical orientation. It  contains a control block and a normal block.
	  *
	  * Construct a component layout, add connectors and other layout elements and forget it :).
	  * @param component Component for which the layout is.
	  */
	ComponentLayoutVerticalCtrl(CompView * compView);
	
	ComponentLayoutControlBlock * getCtrlBlock() const { return m_ctrlBlock; } ;
	ComponentLayoutBlock * getFuncBlock() const { return m_funcBlock; } ;

private:
	ComponentLayoutControlBlock * m_ctrlBlock;
	ComponentLayoutBlock * m_funcBlock;

};

#endif


