/***************************************************************************
                          controlstop.h  -  description
                             -------------------
    begin                : Thu Mar 28 2002
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

#ifndef CONTROLSTOP_H
#define CONTROLSTOP_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"
#include "componentinfo.h"
#include "componentpropertygeneralwidget.h"



// Forward declaration
class ConnectorBoolInEdge;
class QLabel;
class QSpinBox;


/**This component stops the simulation if triggered.
  *@author Rasmus Diekenbrock
  */

class ControlStop : public Component
{

public:
	/** Creates a component which can stop the simulation.
	  */
	ControlStop(CompContainer * container, const ComponentInfo * ci);
	~ControlStop();

	/** Returns the output connector.
	  */
	ConnectorBoolInEdge * getTriggerConnector() const { return m_trigger; };

	/** Resets the component state. */
	virtual void reset();
	
	/** Executes the simulation of this component */
	virtual void calculate();
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	
	/** Sets the number of edges until the simulation is stopped. */
	void setNumberOfEdges(int number);
	/** Returns the number of edges until the simulation is stopped. */
	int getNumberOfEdges() const;
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Float1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	ConnectorBoolInEdge * m_trigger;
	int m_numberOfEdges;
	int m_edgeCnt;
};

//###############################################################

/** The view of the component ControlStop.
  *
  * @author Rasmus Diekenbrock
  */


class ControlStopView : public CompView
{

public:
	ControlStopView(ControlStop * comp, eViewType viewType);
	~ControlStopView();
	
	virtual void draw(QPainter * p);
	
protected:
private:	
	static QPixmap * pIcon;
	static unsigned int compViewCnt;
	static int offsetX, offsetY, width, height;
};

//###############################################################
//###############################################################


class ControlStopPropertyWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	ControlStopPropertyWidget(ControlStop * comp, QWidget *parent=0, const char *name=0);
	~ControlStopPropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ControlStop* getComponent() { return (ControlStop*) ComponentPropertyGeneralWidget::getComponent(); };
	
protected:
	QLabel * m_counterLabel;
	QSpinBox  * m_counter;
};

#endif
