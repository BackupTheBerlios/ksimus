/***************************************************************************
                          ksimdoubleedit.h  -  description
                             -------------------
    begin                : Sun Nov 26 2000
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

#ifndef KSIMDOUBLEEDIT_H
#define KSIMDOUBLEEDIT_H

#include <qlineedit.h>

class QDoubleValidator;

/**A QLineEdit for Double Type
  *@author Rasmus Diekenbrock
  */

class KSimDoubleEdit : public QLineEdit
{
   Q_OBJECT
public: 
	KSimDoubleEdit(double bottom, double top, int decimals, QWidget *parent, const char *name);
	~KSimDoubleEdit();

	void setRange(double bottom, double top, int decimals);
	double bottom() const;
	double top() const;
	int decimals() const;
	double value();
	
public slots:
	void setValue(double value);
	void restoreValue();

protected slots:
	void slotTextChanged(const QString & text);


signals:
	void valueChanged(double value);	

protected:
	QDoubleValidator * m_dVal;
	double m_lastValue;
};

#endif
