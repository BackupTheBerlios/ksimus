/***************************************************************************
                          ksimbooleanbox.h  -  description
                             -------------------
    begin                : Tue Dec 4 2001
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

#ifndef KSIMBOOLEANBOX_H
#define KSIMBOOLEANBOX_H

// C-Includes

// QT-Includes
#include <qcombobox.h>

// KDE-Includes

// Project-Includes

// Forward declaration

/**A input widget for boolean data types. Derived  from QComboBox.
  *@author Rasmus Diekenbrock
  */

class KSimBooleanBox : public QComboBox
{
   Q_OBJECT
public: 
	/** Constructs a boolen input box. */
	KSimBooleanBox(QWidget *parent=0, const char *name=0);
	KSimBooleanBox(bool value, QWidget *parent=0, const char *name=0);
	/** The destructor. */
	~KSimBooleanBox();
	
  /** Sets the text for the false state of the @ref ComboBox. */
  void setFalseText(const QString & name);
  /** Returns the text for the false state of the @ref ComboBox. */
  QString getFalseText() const;
		
  /** Sets the text for the true state of the @ref ComboBox. */
  void setTrueText(const QString & name);
  /** Returns the text for the true state of the @ref ComboBox. */
  QString getTrueText() const;
		
	/** Sets the current state of the @ref ComboBox. */
	void setValue(bool b);
	/** Returns the current state of the @ref ComboBox. */
	bool getValue() const;	

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
	void activated(bool b);	
	/** Emitted, if the combo box changes the highlighted state. */
	void highlighted(bool b);
};

#endif
