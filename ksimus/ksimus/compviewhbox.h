/***************************************************************************
                          compviewhbox.h  -  description
                             -------------------
    begin                : Sat Dec 29 2001
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

#ifndef COMPVIEWHBOX_H
#define COMPVIEWHBOX_H

// C-Includes

// QT-Includes
#include <qhbox.h>

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"


// Forward declaration


/** @ref QHBox class for compViews.
  * Use this class like @ref CompViewWidget but with more features.
  *@author Rasmus Diekenbrock
  */

class CompViewHBox : public QHBox
{
class CompViewHBoxPrivate;

   Q_OBJECT

public:
	CompViewHBox(CompView * cv, QWidget *parent=0, const char *name=0);
	~CompViewHBox();
	
	
	/** Returns the component view. */
	CompView * getCompView() const { return m_compView; };

	/** Returns the component view. */
	Component * getComponent() const { return getCompView()->getComponent(); };

public slots:
	/** Set the foreground color. */
	void setForegroundColor(const QColor & color);
	/** Set the background color. */
	void setBackgroundColor(const QColor & color);
	/** Enables / disables the frame. */
	void setFrameEnabled(bool enaFrame);


private slots:
	/** This slot delete this widget */
	void slotDelete(void);

protected:
	CompViewHBox(bool horizontal, CompView * cv, QWidget *parent=0, const char *name=0);

private:
	void init();
	CompViewHBoxPrivate * m_p;	
	CompView * m_compView;

signals:
	void destroyed(const QWidget * widget);

};

#endif
