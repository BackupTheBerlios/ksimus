/***************************************************************************
                          moduleeditor.h  -  description
                             -------------------
    begin                : Fri Jul 21 2000
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

#ifndef MODULEEDITOR_H
#define MODULEEDITOR_H

// C-Includes

// QT-Includes
#include <qwidget.h>

// KDE-Includes

// Project-Includes
#include "moduledata.h"

// Forward declaration
class ModuleDialog;
class QPixmap;
class DisplayWidget;


/**
  *@author Rasmus Diekenbrock
  */


class ModuleEditor : public QWidget
{
public: 
	ModuleEditor(ModuleDialog * dialog, QWidget * parent, const char * name = 0);
	~ModuleEditor();
	/** Redraw the drawMap next paintEvent */
	void updateDrawMap();
	/** Redraw the drawMap immediatly */
	void updateDrawMapNow();

protected:
	/** Draw the Connectors
		Called from updateDrawMap() */
	void drawConn(QPainter *p);
	/** Draw a connectors area */
	void drawConnArea(QPainter *p, const QRect & area);
	/** Calculate window size and module offset
		@see viewSize,viewOfsX,viewOfsY
	*/
	void calGeometric(const QSize & widgetSize);
	/** redraw the module contents */
	void redrawMap();
	/** Setup the drawMap for the pixmap view */
	void updateDrawMapPixmapView();
	/** Setup the drawMap for the user view */
	void updateDrawMapUserView();
	/** Setup the m_drawMap for the message view */
	void updateDrawMapMessage(const QString & message);
	
	virtual void paintEvent(QPaintEvent * ev );
	virtual void mousePressEvent(QMouseEvent * ev);
	virtual void mouseMoveEvent(QMouseEvent * ev);
	virtual void mouseReleaseEvent(QMouseEvent * ev);
	void connMouseEvent(QMouseEvent * ev);
	
	void drawDragRect();
	
	/* The corresponding dialog */
	ModuleDialog * m_dialog;
	
	/** Contains the view. */
	QPixmap * m_drawMap;
	
	/** Size of the editor window. */
	QSize m_viewSize;
	/** Offset of the module view in editor window. */
	int m_viewOffsetX, m_viewOffsetY;
	/** Area for the top connectors. */
	QRect m_topArea;
	/** Area for the bottom connectors. */
	QRect m_bottomArea;
	/** Area for the left connectors. */
	QRect m_leftArea;
	/** Area for the right connectors. */
	QRect m_rightArea;
	
	/** True, if a update of the drawMap is required. */
	bool m_updateRequired;
	
	/** Flag mouse buttons. */
	bool m_lmbDown, m_rmbDown;
	
	/** Widget for the user view. */
	DisplayWidget * m_userView;

};

#endif
