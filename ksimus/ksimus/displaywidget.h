/***************************************************************************
                          displaywidget.h  -  description
                             -------------------
    begin                : Fri Sep 8 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

// C-Includes

// QT-Includes
#include <qwidget.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class CompViewList;

/**The DisplayWidget displays compoent views
  *@author Rasmus Diekenbrock
  */

class QPixmap;

class DisplayWidget : public QWidget  {
   Q_OBJECT
public: 
	DisplayWidget(CompViewList * viewList, QWidget *parent=0, const char *name=0);
	~DisplayWidget();

    /** Redraw the drawMap with all components, wires and selections
    	updateDrawMap() doesn't update the widget. update() will do that.*/
	void updateDrawMap();

public slots: // Public slots
	/** This slot delete this widget */
	void slotDelete(void);

protected:
	virtual void paintEvent(QPaintEvent * ev );
	virtual void resizeEvent ( QResizeEvent * ev);

	/** Contains the drawing */
	QPixmap * drawMap;

	CompViewList * displayList;

signals:
//	void destroyed();
	void destroyed(const QWidget * widget);

};

#endif
