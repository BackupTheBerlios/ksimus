/***************************************************************************
                          complayout.h  -  description
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
#include <qlist.h>

// KDE-Includes

// Project-Includes
#include "types.h"
#include "componentaddon.h"


// Forward declaration
class CompLayoutConnector;
class CompLayoutSpace;
class CompLayoutStretch;
class ConnectorBase;
class ComponentLayout;
class ConnectorPack;
class CompLayoutBase;


//#######################################################################
//#######################################################################

/** Smallest item class for component layouts. All layout elements are based on this class.
  * This class defines the base API of all layout classes. The class itself does nothing.
  * @author Rasmus Diekenbrock
  */

class CompLayoutBaseItem
{
public:

	/** Constructor. Does nothing. */
	CompLayoutBaseItem(CompLayoutBase * parentLayout);
	/** Destructor. Does nothing. */
	virtual ~CompLayoutBaseItem();
	
	/** This functions returns the size of the layout element. The size is measured in grid (8 Pixels).
	 */
	virtual int getSize() const = 0;
	/** Returns the stretch factor of the layout element. In most case the factor is 0
	 */
	virtual int getStretchFactor() const = 0;
	/** Positionized the layout element.
	 *  @param size       The number of grid to distribute for stretch elements.
	 *	@param sumStretch The sum of all stretch factors which have to split the size from above.
	 *  @param startPos   The first position usable by this layout element.
	 */
	virtual int position(int & size, int & sumStretch, QPoint startPos) = 0;
	
	/** Returns the parent layout.*/
	CompLayoutBase * getParent() const { return m_parentLayout; };
  /** Returns the main layout. */
  ComponentLayout * getBaseLayout() const;
  /** Returns the related component. */
  Component * getComponent() const;

private:
	CompLayoutBase * m_parentLayout;
};

//#######################################################################
//#######################################################################

/** A list which contains @ref CompLayoutBaseItem elements.
  * @author Rasmus Diekenbrock
  */

class CompLayoutList : public QList<CompLayoutBaseItem>
{};


#define FOR_EACH_LAYOUTITEM(_it_,_layoutList_)	\
		for(QListIterator<CompLayoutBaseItem> _it_(_layoutList_);_it_.current();++_it_)


//#######################################################################
//#######################################################################


/**Base class for component layouts.
   This class manages the position of the connectors.
  *@author Rasmus Diekenbrock
  */


class CompLayoutBase : public CompLayoutBaseItem
{
friend class ComponentLayout;

public:
	/** Constructs a layout element.
			@param parentLayout The parent layout element.
			@param orientation  Sets the connector orientation of this layout. Has to be equal as the
			                    orientation of the parent layout.
	 */
	CompLayoutBase(CompLayoutBase * parentLayout, ConnOrientationType orientation);
	/** The destructor. Deletes all layout element which where added to this element.
	 */
	virtual ~CompLayoutBase();

  /** Adds a sub layout. The layout must have th same orientation.
   */
  void addLayout(CompLayoutBase * layout);
  /** Adds a connector to the layout element. The orentation of the connector is set by the function.
   *  It is not necassary to set a proper connector position (because this is the task auf this class).
   *  Create a connector and add it, there is nothing else to do.
   *  @param conn            The connector to add.
   *  @param additionalSpace A additional space after the connector.
   */
  void addConnector(ConnectorBase * conn, int additionalSpace = 1);
  /** Adds a space to the layout item.
    * @param space Space size measured in grids.
    */
  void addSpace(int space);
  /** Adds a stretch item to the layout element. The size of a stretch item depends on the unused space
    * of the component side and the stretch factor.
    * @param factor   The factor is relative to other stretch factors of the layout element. Higher stretch
                      factors get more space.
    */
  void addStretch(int factor);
  /** Adds a connector pack to the layout element. The required space of this element is count of connector *
      (1 + space).
    * @param connPack           The connector pack to add.
    * @param additionalSpace    Space between two connectors and after the last connector.
    */
  void addConnectorPack(ConnectorPack * connPack, int additionalSpace = 1);

  /** Returns the orientations of the element. */
  ConnOrientationType getOrientation() const { return m_orientation; };

  /** Returns the list of containing elements.*/
  CompLayoutList * getItemList() const { return m_itemList; };
	
	/** This functions returns the size of the layout element. The size is measured in grid (8 Pixels).
	 */
	virtual int getSize() const;
	/** Returns the stretch factor of the layout element. In most case the factor is 0
	 */
	virtual int getStretchFactor() const;
	/** Positionized the layout element.
	 *  @param size       The number of grid to distribute for stretch elements.
	 *	@param sumStretch The sum of all stretch factors which have to split the size from above.
	 *  @param startPos   The first position usable by this layout element.
	 */
	virtual int position(int & size, int & sumStretch, QPoint startPos);


  /** Returns the parent layout. Returns a null pointer if the layout have no parent. */
  CompLayoutBase * getParentLayout() const;
  /** Returns the main layout. */
  ComponentLayout * getBaseLayout() const;
  /** Returns the related component. */
  Component * getComponent() const;

	/** Moves the position "pos" by "add" in the direction given by the orientation. */
  void addPosition(QPoint & pos, int add);


protected:
	/** Constructor for internal purpose. */
	CompLayoutBase(ComponentLayout * parent, ConnOrientationType orientation);
	/** Sets the orientation. For internal use only. */
  void setOrientation(ConnOrientationType orient) { m_orientation = orient; };

private:
	/** Contains the parent. */
	union
	{
		CompLayoutBase * layout;
		ComponentLayout * base;
	} m_parent;
	
	/** Contains the orientation. */
	ConnOrientationType m_orientation;
	/** Conatins all child layout elements. */
	CompLayoutList * m_itemList;
	/** Some internal flags. */
	Q_UINT32 m_flags;

};

//#######################################################################
//#######################################################################

/**A class for sub layout elements at the left side of the component.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutLeft : public CompLayoutBase
{
public:
	/** Constructs a sub layout element for the left component side. */
	ComponentLayoutLeft(CompLayoutBase * parentLayout)
	:	CompLayoutBase(parentLayout, CO_LEFT)
	{};
};

/**A class for sub layout elements at the right side of the component.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutRight : public CompLayoutBase
{
public:
	/** Constructs a sub layout element for the right component side. */
	ComponentLayoutRight(CompLayoutBase * parentLayout)
	:	CompLayoutBase(parentLayout, CO_RIGHT)
	{};
};

/**A class for sub layout elements at the top side of the component.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutTop : public CompLayoutBase
{
public:
	/** Constructs a sub layout element for the top component side. */
	ComponentLayoutTop(CompLayoutBase * parentLayout)
	:	CompLayoutBase(parentLayout, CO_TOP)
	{};
};

/**A class for sub layout elements at the bottom side of the component.
  *@author Rasmus Diekenbrock
  */

class ComponentLayoutBottom : public CompLayoutBase
{
public:
	/** Constructs a sub layout element for the bottom component side. */
	ComponentLayoutBottom(CompLayoutBase * parentLayout)
	:	CompLayoutBase(parentLayout, CO_BOTTOM)
	{};
};

//#######################################################################
//#######################################################################

/** A class for component layout. Add one element to your component. Add the layout elements to
  * one of the four layouts. Each layout represents one side of the component.
  * @author Rasmus Diekenbrock
  */

class ComponentLayout : public ComponentAddOn
{
  Q_OBJECT

public:
	/** Constructs a component layout.
	  *
	  * Construct a component layout, add connectors and other layout elements and forgot it :).
	  * @param component Component for which the layout is.
	  */
	ComponentLayout(Component * component);
	/** The destructor. Deletes also all added layout items. Do not use the destructor, this is done
	  * by KSimus. */
	~ComponentLayout();
	
	
	/** Returns the layout for the left side. */
	CompLayoutBase * getLeft() const   { return m_left; };
	/** Returns the layout for the right side. */
	CompLayoutBase * getRight() const  { return m_right; };
	/** Returns the layout for the top side. */
	CompLayoutBase * getTop() const    { return m_top; };
	/** Returns the layout for the bottom side. */
	CompLayoutBase * getBottom() const { return m_bottom; };
	

  /** Returns the minimum size. The default minimum size is QSize(5,5).*/
  const QSize & getMinSize() const { return m_minSize; };
  /** Sets the minimum size. */
  void setMinSize(const QSize & minSize);
  /** Sets the minimum size. */
  void setMinSize(int width, int height) { setMinSize(QSize(width, height)); };
	
  /** Returns the current component size.*/
  const QSize & getSize() const { return m_currentSize; };

  /** Set component size fixed. If true, the layout dont change the size of the component. */
  void setFixedSize(bool fixed);
  /** Returns, if the component size is set to fix. */
  bool isFixedSize() const { return m_fixedSize; };



public slots:
	/** Calculates uns sets new component size and connector positions. */
	void updateLayout();
	/** Actually calls @ref updateLayout().*/
	void slotResizeView();



private:
	CompLayoutBase * m_left;
	CompLayoutBase * m_right;
	CompLayoutBase * m_top;
	CompLayoutBase * m_bottom;
	QSize m_minSize;
	QSize m_currentSize;
	bool m_fixedSize;

};



#endif
