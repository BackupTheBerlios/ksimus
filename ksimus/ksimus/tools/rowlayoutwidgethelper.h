/***************************************************************************
                          rowlayoutwidgethelper.h  -  description
                             -------------------
    begin                : Sun May 26 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

#ifndef ROWLAYOUTWIDGETHELPER_H
#define ROWLAYOUTWIDGETHELPER_H


// C-Includes

// QT-Includes
#include <qobject.h>
#include <qwidget.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class RowLayoutWidget;

/**Some helper classes for RowLayoutWidget.
  *@author Rasmus Diekenbrock
  */

enum RowLayoutWidgetHelperType
{
	RowLayoutNewRow,
	RowLayoutAddEmptyCell,
	RowLayoutSetRowStretch
};

//#######################################################################
//#######################################################################

class RowLayoutWidgetHelperObj : public QObject
{
   Q_OBJECT
public:
	RowLayoutWidgetHelperObj(RowLayoutWidget* parent, RowLayoutWidgetHelperType type);
//	~RowLayoutWidgetHelperObj();
	
	RowLayoutWidgetHelperType getType() const;
	
private:
	RowLayoutWidgetHelperType m_type;
};

//#######################################################################
//#######################################################################

class RowLayoutWidgetHelperRowStretch : public RowLayoutWidgetHelperObj
{
   Q_OBJECT
public:
	RowLayoutWidgetHelperRowStretch(RowLayoutWidget* parent, int stretch);
//	~RowLayoutWidgetHelperRowStretch();
	
	int getStretch() const;
	
private:
	int m_stretch;
};

//#######################################################################
//#######################################################################

#endif
