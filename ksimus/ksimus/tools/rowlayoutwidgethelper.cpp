/***************************************************************************
                          rowlayoutwidgethelper.cpp  -  description
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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "rowlayoutwidgethelper.h"
#include "rowlayoutwidget.h"

// Forward declaration


//#######################################################################
//#######################################################################

RowLayoutWidgetHelperObj::RowLayoutWidgetHelperObj(RowLayoutWidget* parent, RowLayoutWidgetHelperType type)
	:	QObject(parent),
		m_type(type)
{
}

RowLayoutWidgetHelperType RowLayoutWidgetHelperObj::getType() const
{
	return m_type;
}

//#######################################################################
//#######################################################################


RowLayoutWidgetHelperRowStretch::RowLayoutWidgetHelperRowStretch(RowLayoutWidget* parent, int stretch)
	:	RowLayoutWidgetHelperObj(parent, RowLayoutSetRowStretch),
		m_stretch(stretch)
{
}

int RowLayoutWidgetHelperRowStretch::getStretch() const
{
	return m_stretch;
}

//#######################################################################
//#######################################################################

