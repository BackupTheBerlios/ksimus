/***************************************************************************
                          datarecorderview.h  -  description
                             -------------------
    begin                : Thu May 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DATARECORDERVIEW_H
#define DATARECORDERVIEW_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/compview.h"
#include "ksimus/compviewwidget.h"

// Forward declaration


namespace KSimLibDataRecorder
{

/**The view of the data recorder
  *@author Rasmus Diekenbrock
  */


//###############################################################
//###############################################################


class DataRecorderView : public CompView
{
	Q_OBJECT

public:	
	DataRecorderView(Component * comp, eViewType viewType);
	virtual ~DataRecorderView();
	
	virtual void draw(QPainter * p);
//	virtual QWidget * createCompViewWidget(QWidget * parent);

protected:

};




};  //namespace KSimLibDataRecorder

#endif
