/***************************************************************************
                          ksimlineedit.h  -  description
                             -------------------
    begin                : Sun Apr 22 2001
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

#ifndef KSIMLINEEDIT_H
#define KSIMLINEEDIT_H

// C-Includes

// QT-Includes
#include <qlineedit.h>

// KDE-Includes

// Project-Includes

// Forward declaration

/**This line edit emit a signal if data is changed and focus is lost or return is pressed.
  *@author Rasmus Diekenbrock
  */

class KSimLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	KSimLineEdit( QWidget *parent, const char *name=0 );
	KSimLineEdit( const QString &, QWidget *parent, const char *name=0 );
//	~KSimLineEdit();

protected:
	virtual void focusOutEvent(QFocusEvent *);

private slots:
	void slotChanged();

signals:
	void changed();
	void changed(const QString &);
};

#endif
