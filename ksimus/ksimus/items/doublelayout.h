/***************************************************************************
                          doublelayout.h  -  description
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

#ifndef DOUBLELAYOUT_H
#define DOUBLELAYOUT_H

// C-Includes

// QT-Includes
#include <qrect.h>

// KDE-Includes

// Project-Includes
#include "componentlayout.h"


// Forward declaration
class DoubleLayoutChild;


//#######################################################################
//#######################################################################

/**A syncronized double layout.
  *@author Rasmus Diekenbrock
  */


class DoubleLayout
{
public: 
	DoubleLayout(ComponentLayout * parentLayout, Qt::Orientation orient);
	~DoubleLayout();

	
	/** Returns the parent layout. */
	ComponentLayout * getParentLayout() const { return m_parentLayout; };
	/** Returns the orientation of the layout element. */
	Qt::Orientation getOrientation() const { return  m_orientation;	};

	
	/** This functions returns the size of both layout elements. The size is measured in grid (8 Pixels).
 	  * @param size       Add the size of the layout element to the variable size.
	  */
	void getSizeInternal(unsigned int & size) const;
	/** Returns the stretch factor of both layout elements.
			DoubleLayout supports no stretching yet!
	 */
	unsigned int getStretchFactorInternal() const;
	
	
	/** Returns the layout for the left side. */
	CompLayoutBase * getLeft() const;
	/** Returns the layout for the right side. */
	CompLayoutBase * getRight() const;
	/** Returns the layout for the top side. */
	CompLayoutBase * getTop() const;
	/** Returns the layout for the bottom side. */
	CompLayoutBase * getBottom() const;

	/** Returns the place which is inside this layout class. If withConn is true the place contains also
	  * the connectors. The place is measured in grids.*/
	QRect getRectGrid(bool withoutConn) const;
	/** Returns the place which is inside this layout class. If withConn is true the place contains also
	  * the connectors. The place is measured in pixels.*/
	QRect getRect(bool withoutConn) const;


protected:	
	

private:
	ComponentLayout * m_parentLayout;
	Qt::Orientation m_orientation;
	DoubleLayoutChild * m_child1;
	DoubleLayoutChild * m_child2;
};

#endif
