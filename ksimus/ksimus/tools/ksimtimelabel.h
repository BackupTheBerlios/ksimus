/***************************************************************************
                          ksimtimelabel.h  -  description
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

#ifndef KSIMTIMELABEL_H
#define KSIMTIMELABEL_H

#include <qlabel.h>

/**Display the KSimTime
  *@author Rasmus Diekenbrock
  */

class KSimTimeBase;

class KSimTimeLabel : public QLabel
{
   Q_OBJECT
public: 
	KSimTimeLabel(const KSimTimeBase * time, int precision, QWidget *parent=0, const char *name=0);
	KSimTimeLabel(const KSimTimeBase * time, QWidget *parent=0, const char *name=0);
	~KSimTimeLabel();
	
	int precision() const { return m_precision; };
	void setPrecision(int precision) { m_precision = precision;};

public slots:
	void slotTimeChanged();
	
protected:
	const KSimTimeBase * m_time;
	int m_precision;
};

#endif
