/***************************************************************************
                          ksimiopinselectionwidget.h  -  description
                             -------------------
    begin                : Sun Apr 6 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef KSIMIOPINSELECTIONWIDGET_H
#define KSIMIOPINSELECTIONWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"
#include "ksimiopin.h"

// Forward declaration
class QListView;

/**Select a pin from a pin list.
  *@author Rasmus Diekenbrock
  */

class KSimIoPinSelectionWidget : public PropertyWidget
{
   Q_OBJECT
public: 
	KSimIoPinSelectionWidget(const KSimIoPin::List & pinList, QWidget *parent=0, const char *name=0);
	~KSimIoPinSelectionWidget();

	const KSimIoPin * getCurrentPin() const;

private:
	class ListItem;
	QListView * m_list;
};

#endif
