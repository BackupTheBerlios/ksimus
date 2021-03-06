/***************************************************************************
                          convertfloatbool.h  -  description
                             -------------------
    begin                : Sun Feb 10 2002
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

#ifndef CONVERTFLOATBOOL_H
#define CONVERTFLOATBOOL_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"



// Forward declaration
class ConnectorFloatIn;
class ConnectorBoolOut;
class QLabel;
class KSimDoubleEdit;

namespace KSimLibFloatingPoint
{

/**Component for converting a floating point value in a boolean value.
  *
  *@author Rasmus Diekenbrock
  */

class ConvertFloatBool : public Component
{

	Q_OBJECT

public:
	/** Creates the component.
	  */
	ConvertFloatBool(CompContainer * container, const ComponentInfo * ci);
//	~ConvertFloatBool();
	
	/** Resets the component state. */
	virtual void reset();
	
	/** Executes the simulation of this component */
	virtual void calculate();
	/** save module properties. */
	virtual void save(KSimData & file) const;
	/** load module properties.
		Returns true if successful. */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	ConnectorFloatIn * getInput() { return m_input; };
	ConnectorBoolOut * getOutput() { return m_output; };
	
	double getFalseThreshold() const { return m_falseThreshold; };
	void setFalseThreshold(double limit);
	
	double getTrueThreshold() const { return m_trueThreshold; };
	void setTrueThreshold(double limit);
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:

	ConnectorFloatIn * m_input;
	ConnectorBoolOut * m_output;
	
	double m_falseThreshold;
	double m_trueThreshold;
	bool m_recursionLocked;
		
};

//###############################################################

/** View class of @ref ConvertFloatBool.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertFloatBoolView : public CompView
{

	Q_OBJECT

public:
	ConvertFloatBoolView(ConvertFloatBool * comp, eViewType viewType, const char * name);
//	~ConvertFloatBoolView();
	
	virtual void draw(QPainter * p);
	
	ConvertFloatBool* getComponent() { return (ConvertFloatBool*) CompView::getComponent(); };

protected:
	
private:	
};

//###############################################################

/** The property widget for @ref ConvertFloatBool components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertFloatBoolPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	ConvertFloatBoolPropertyGeneralWidget(ConvertFloatBool * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ConvertFloatBool* getComponent() { return (ConvertFloatBool*) ComponentPropertyGeneralWidget::getComponent(); };
	
	
protected:
	KSimDoubleEdit * m_trueThreshold;
	QLabel * m_trueThresholdLabel;
	KSimDoubleEdit * m_falseThreshold;
	QLabel * m_falseThresholdLabel;

	QLabel * m_levelLabel;
	QLabel * m_level;
	QLabel * m_hysteresisLabel;
	QLabel * m_hysteresis;

private slots:

	void valChanged();

};


//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
