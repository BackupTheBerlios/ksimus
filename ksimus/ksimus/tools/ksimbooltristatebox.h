/***************************************************************************
                       ksimbooltristatebox.h  -  description
                             -------------------
    begin                : Thu Nov 14 2002
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

#ifndef KSIMBOOLTRISTATEBOX_H
#define KSIMBOOLTRISTATEBOX_H

// C-Includes

// QT-Includes
#include <qcombobox.h>

// KDE-Includes

// Project-Includes
#include "ksimbooltristate.h"

// Forward declaration

/**A input widget for tristate data type. Derived  from QComboBox.
  *@author Rasmus Diekenbrock
  */

class KSimBoolTristateBox : public QComboBox
{
   Q_OBJECT
public: 

	

	/** Constructs a boolen input box. */
	KSimBoolTristateBox(QWidget *parent=0, const char *name=0);
	KSimBoolTristateBox(bool state, bool active, QWidget *parent=0, const char *name=0);
	KSimBoolTristateBox(KSimBoolTristate state, QWidget *parent=0, const char *name=0);
	/** The destructor. */
	~KSimBoolTristateBox();
	
	/** Sets the text for the false state of the @ref ComboBox. */
	void setFalseText(const QString & name);
	/** Returns the text for the false state of the @ref ComboBox. */
	QString getFalseText() const;
		
	/** Sets the text for the true state of the @ref ComboBox. */
	void setTrueText(const QString & name);
	/** Returns the text for the true state of the @ref ComboBox. */
	QString getTrueText() const;
		
	/** Sets the text for the active state of the @ref ComboBox. */
	void setActiveText(const QString & name);
	/** Returns the text for the active state of the @ref ComboBox. */
	QString getActiveText() const;

	/** Sets the state of the @ref ComboBox. */
	void setValue(KSimBoolTristate state);
	/** Sets the state of the @ref ComboBox. */
	void setValue(bool state, bool active);
	/** Returns the state of the @ref ComboBox. */
	KSimBoolTristate getValue() const;
	/** Returns the state of the @ref ComboBox. */
	KSimBoolTristate value() const { return getValue(); };

private slots:
	/** For internal use. */
	void slotActivated(int);
	/** For internal use. */
	void slotHighlighted(int);

private:
	/** For internal use. */
	void init();
	
signals:
	/** Emitted, if the combo box changes the state. */
	void activated(KSimBoolTristate state);	
	/** Emitted, if the combo box changes the highlighted state. */
	void highlighted(KSimBoolTristate state);
};

#endif
