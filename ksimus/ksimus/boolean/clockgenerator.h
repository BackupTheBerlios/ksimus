/***************************************************************************
                          clockgenerator.h  -  description
                             -------------------
    begin                : Sun Mar 4 2001
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

#ifndef CLOCKGENERATOR_H
#define CLOCKGENERATOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"
#include "ksimtime.h"
#include "ksimtimeserver.h"
#include "componentpropertybasewidget.h"

// Forward declaration
class ConnectorBoolOut;
class KSimTimeSpinBox;

/**A clock generator
  *@author Rasmus Diekenbrock
  */


class ClockGenerator : public Component
{

friend class ClockGeneratorPropertyWidget;

Q_OBJECT

public:
	ClockGenerator(CompContainer * container, const ComponentInfo * ci);
	~ClockGenerator();
	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();
	
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	bool m_result;
	ConnectorBoolOut * m_out;
	KSimTime m_startDelay;
	KSimTime m_highTime;
	KSimTime m_lowTime;
	KSimTime m_time;

private:
	static const char * const sStartDelay;
	static const char * const sHighTime;
	static const char * const sLowTime;

};

//###############################################################

class ClockGeneratorView : public CompView
{
public:
	ClockGeneratorView(Component * comp, eViewType viewType, const char * name);
	
	virtual void draw(QPainter * p);
};

//###############################################################

class ClockGeneratorPropertyWidget : public ComponentPropertyBaseWidget
{
	Q_OBJECT

public:
	ClockGeneratorPropertyWidget(ClockGenerator * comp, QWidget *parent=0, const char *name=0);
	~ClockGeneratorPropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ClockGenerator * getClockGen() { return (ClockGenerator *)getComponent();	};


protected:

	KSimTimeSpinBox * m_startDelay;
	KSimTimeSpinBox * m_highTime;
	KSimTimeSpinBox * m_lowTime;

};


#endif
