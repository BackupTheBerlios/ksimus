/***************************************************************************
                          compviewwidget.h  -  description
                             -------------------
    begin                : Tue Jan 30 2001
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

#ifndef COMPVIEWWIDGET_H
#define COMPVIEWWIDGET_H

// C-Includes

// QT-Includes
#include <qwidget.h>

// KDE-Includes

// Project-Includes
#include "compview.h"

// Forward declaration

/**Widget class for CompViews
  *@author Rasmus Diekenbrock
  */

class CompViewWidget : public QWidget
{
class CompViewWidgetPrivate;

   Q_OBJECT
public: 
	CompViewWidget(CompView * cv, QWidget *parent=0, const char *name=0);
	~CompViewWidget();
	
	/** Returns the component view. */
	Component * getComponent() const { return getCompView()->getComponent(); };

	/** Returns the component view. */
	CompView * getCompView() const { return m_compView; };

		
public slots:
	/** Set the foreground color. */
	void setForegroundColor(const QColor & color);
	/** Set the background color. */
	void setBackgroundColor(const QColor & color);
	/** Sets the font. */
	void setSpecialFont(const QFont & font);

private slots:
	/** This slot delete this widget */
	void slotDelete(void);

private:
	CompViewWidgetPrivate * m_p;	
	CompView * m_compView;

signals:
	void destroyed(const QWidget * widget);

};

#endif
