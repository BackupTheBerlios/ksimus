/***************************************************************************
                          convertboolinteger.h  -  description
                             -------------------
    begin                : Sun Sep 7 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef CONVERTBOOLINTEGER_H
#define CONVERTBOOLINTEGER_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/ksimbaseint.h"

// Forward declaration
class QLabel;
class KSimBaseIntEdit;
class ConnectorBoolIn;
class ConnectorIntegerOut;

namespace KSimLibInteger
{

/**Component for converting a boolean value in an integer value.
  *
  *@author Rasmus Diekenbrock
  */

class ConvertBoolInteger : public Component
{
public:
	/** Constructs the component. */
	ConvertBoolInteger(CompContainer * container, const ComponentInfo * ci);
	
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
	  * This function creeates a @ref ConvertBoolFloatPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	ConnectorBoolIn * getInput() { return m_input; };
	ConnectorIntegerOut * getOutput() { return m_output; };
	
	const KSimBaseInt & getFalseValue() const { return m_falseValue; };
	void setFalseValue(const KSimBaseInt & value);
	
	const KSimBaseInt & getTrueValue() const { return m_trueValue; };
	void setTrueValue(const KSimBaseInt & value);

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

	static const KSimBaseInt DefaultFalseValue;
	static const KSimBaseInt DefaultTrueValue;

private:
	
	ConnectorBoolIn * m_input;
	ConnectorIntegerOut * m_output;
	
	KSimBaseInt m_falseValue;
	KSimBaseInt m_trueValue;
	bool m_recursionLocked;

	class View;
	class PropertyGeneralWidget;
};

//###############################################################

/** The view of the @ref ConvertBoolInteger.
	*
  * @author Rasmus Diekenbrock
  */
class ConvertBoolInteger::View : public CompView
{
public:
	View(ConvertBoolInteger * comp, eViewType viewType, const char * name);
//	~View();
	
	virtual void draw(QPainter * p);
	
	ConvertBoolInteger* getConvertBoolInteger() { return (ConvertBoolInteger*) CompView::getComponent(); };
};

//###############################################################
/** The property widget of the @ref ConvertBoolInteger.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertBoolInteger::PropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
public:
	PropertyGeneralWidget(ConvertBoolInteger * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ConvertBoolInteger* getConvertBoolInteger() { return (ConvertBoolInteger*) getComponent(); };
	
	
protected:
	KSimBaseIntEdit * m_trueValue;
	QLabel * m_trueValueLabel;
	KSimBaseIntEdit * m_falseValue;
	QLabel * m_falseValueLabel;

};


//###############################################################


};  //namespace KSimLibInteger
#endif
