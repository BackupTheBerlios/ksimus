/***************************************************************************
                          boolean7segment.h  -  description
                             -------------------
    begin                : Wed Dec 26 2001
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

#ifndef BOOLEAN7SEGMENT_H
#define BOOLEAN7SEGMENT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/componentstyle.h"
#include "ksimus/compview.h"
#include "ksimus/compviewhbox.h"
#include "ksimus/connectorboolin.h"


// Forward declaration
class QLCDNumber;



namespace KSimLibBoolean
{


extern const ComponentInfo Boolean7SegmentInfo;


//#######################################################################
//#######################################################################


/**A 7 segment display.
  *@author Rasmus Diekenbrock
  */

class Boolean7Segment : public ComponentStyle
{
	Q_OBJECT
	
public:
	Boolean7Segment(CompContainer * container, const ComponentInfo * ci);
//	~Boolean7Segment();

	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();

  ConnectorBoolIn * getInput1() const { return m_in1; };
  ConnectorBoolIn * getInput2() const { return m_in2; };
  ConnectorBoolIn * getInput4() const { return m_in4; };
  ConnectorBoolIn * getInput8() const { return m_in8; };
	

protected: // Protected attributes
	int m_number;
	
	ConnectorBoolIn * m_in1;
	ConnectorBoolIn * m_in2;
	ConnectorBoolIn * m_in4;
	ConnectorBoolIn * m_in8;

signals:
	void signalSetNumber(int newNumber);
};

//#######################################################################
//#######################################################################

class Boolean7SegmentView : public CompViewSize
{
	Q_OBJECT

public:	
	Boolean7SegmentView(Boolean7Segment * comp, eViewType viewType);
//	virtual ~Boolean7SegmentView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	Boolean7Segment * get7Seg() const { return (Boolean7Segment *)getComponent(); };

protected:
	virtual void resize();

private:

signals:
};

//#######################################################################
//#######################################################################

class Boolean7SegmentWidgetView : public CompViewHBox
{
	Q_OBJECT

friend class Boolean7SegmentView;
	
public:
	Boolean7SegmentWidgetView(Boolean7SegmentView * cv, QWidget *parent=0, const char *name=0);
//	~Boolean7SegmentWidgetView();

	Boolean7Segment * get7Seg() const { return (Boolean7Segment *)getCompView()->getComponent(); };

public slots:

protected:
	QLCDNumber * m_lcd;
};


//###############################################################

}; //namespace KSimLibBoolean

#endif
