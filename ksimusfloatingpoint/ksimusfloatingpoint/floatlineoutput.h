/***************************************************************************
                          floatlineoutput.h  -  description
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

#ifndef FLOATLINEOUTPUT_H
#define FLOATLINEOUTPUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/componentstyle.h"
#include "ksimus/compview.h"
#include "ksimus/compviewvbox.h"

// Forward declaration
class ConnectorFloatIn;
class QLabel;



namespace KSimLibFloatingPoint
{


extern const ComponentInfo FloatLineOutputInfo;


//#######################################################################
//#######################################################################


/**A simple floating output.
  *@author Rasmus Diekenbrock
  */

class FloatLineOutput : public ComponentStyle
{
	Q_OBJECT
	
public:
	FloatLineOutput(CompContainer * container, const ComponentInfo * ci);
//	~FloatLineOutput();

	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();

  ConnectorFloatIn * getInput() const { return m_input; };
	

protected: // Protected attributes
	double m_number;
	
	ConnectorFloatIn * m_input;

signals:
	void signalSetNumber(double newNumber);
};

//#######################################################################
//#######################################################################

class FloatLineOutputView : public CompViewSize
{

	Q_OBJECT

public:	
	FloatLineOutputView(FloatLineOutput * comp, eViewType viewType);
//	virtual ~FloatLineOutputView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	FloatLineOutput * getLineOutput() const { return (FloatLineOutput *)getComponent(); };


protected:
	virtual void resize();

	
private:


signals:
};

//#######################################################################
//#######################################################################

class FloatLineOutputWidgetView : public CompViewVBox
{
	Q_OBJECT

friend class FloatLineOutputView;
	
public:
	FloatLineOutputWidgetView(FloatLineOutputView * cv, QWidget *parent=0, const char *name=0);
//	~FloatLineOutputWidgetView();

	FloatLineOutput * getLineOutput() const { return (FloatLineOutput *)getCompView()->getComponent(); };

protected:
	QLabel * m_label;
};

//#######################################################################
//#######################################################################


};  //namespace KSimLibFloatingPoint
#endif
