/***************************************************************************
                          convertboolfloat.h  -  description
                             -------------------
    begin                : Mon Feb 11 2002
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

#ifndef CONVERTBOOLFLOAT_H
#define CONVERTBOOLFLOAT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1out.h"
#include "ksimus/ksimtime.h"
#include "ksimus/componentinfo.h"

// Forward declaration
class QLabel;
class KSimDoubleEdit;
class ConnectorBoolIn;

namespace KSimLibFloatingPoint
{


const ComponentInfo * getConvertBoolFloatInfo();


//###############################################################

/**Component for converting a boolean value in a floating point value.
  *@author Rasmus Diekenbrock
  */
/**A waveform generator.
	*
  * @author Rasmus Diekenbrock
  */

class ConvertBoolFloat : public KSimLibFloatingPoint::Float1Out
{
public:
	/** Constructs the component. */
	ConvertBoolFloat(CompContainer * container, const ComponentInfo * ci);
	
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

	ConnectorBoolIn * getInput() { return m_input;	};
	
	double getFalseValue() const { return m_falseValue; };
	void setFalseValue(double value);
	
	double getTrueValue() const { return m_trueValue; };
	void setTrueValue(double value);

private:
	
	ConnectorBoolIn * m_input;
	
	double m_falseValue;
	double m_trueValue;
	
};

//###############################################################

/** The view for the floating point waveform generator.
	*
  * @author Rasmus Diekenbrock
  */
class ConvertBoolFloatView : public KSimLibFloatingPoint::Float1OutView
{
public:
	ConvertBoolFloatView(ConvertBoolFloat * comp, eViewType viewType);
//	~ConvertBoolFloatView();
	
	virtual void draw(QPainter * p);
	
	ConvertBoolFloat* getComponent() { return (ConvertBoolFloat*) Float1OutView::getComponent(); };
};

//###############################################################
/** The property widget for the boolean to floating point convert.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertBoolFloatPropertyGeneralWidget : public KSimLibFloatingPoint::Float1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	ConvertBoolFloatPropertyGeneralWidget(ConvertBoolFloat * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ConvertBoolFloat* getComponent() { return (ConvertBoolFloat*) ComponentPropertyGeneralWidget::getComponent(); };
	
	
protected:
	KSimDoubleEdit * m_trueValue;
	QLabel * m_trueValueLabel;
	KSimDoubleEdit * m_falseValue;
	QLabel * m_falseValueLabel;

};


//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
