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
	DataRecorderView(Component * comp, eViewType viewType, const char * name);
	virtual ~DataRecorderView();
	
	virtual void draw(QPainter * p);
//	virtual QWidget * createCompViewWidget(QWidget * parent);
	/** Hit point x,y the component ?
		NO_HIT
		NORMAL_HIT        - component is hit, KSimEditor controls mouse action
		CONNCTOR_HIT      - connector of a component is hit
		WIRE_HIT          - wire is hit, KSimEditor controls mouse action
		SPECIAL_HIT       - component is hit, component controls mouse action
	*/
	
	virtual eHitType isHit(int x, int y) const;
	
	/** Manage the mouse pressed event if required  */
	virtual void mousePress(QMouseEvent *ev, QPainter *p);

protected:

};




};  //namespace KSimLibDataRecorder

#endif
