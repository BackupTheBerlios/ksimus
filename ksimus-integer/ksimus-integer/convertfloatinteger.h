/***************************************************************************
                          convertfloatinteger.h  -  description
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

#ifndef CONVERTFLOATINTEGER_H
#define CONVERTFLOATINTEGER_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"



// Forward declaration
class ConnectorFloatIn;
class ConnectorIntegerOut;
class QLabel;
class QComboBox;

namespace KSimLibInteger
{

/**Component for converting a floating point value in an integer value.
  *@author Rasmus Diekenbrock
  */

class ConvertFloatInteger : public Component
{
public:

	enum eRoundType { eRound = 0, eFloor, eCeil };

	/** Creates the component.
	  */
	ConvertFloatInteger(CompContainer * container, const ComponentInfo * ci);
//	~ConvertFloatInteger();
	
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
	ConnectorIntegerOut * getOutput() { return m_output; };

	void setRoundType(eRoundType type);
	eRoundType getRoundType() const { return m_roundType; };

		
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

	static const eRoundType DefaultRoundType;

protected:

	ConnectorFloatIn * m_input;
	ConnectorIntegerOut * m_output;

	eRoundType m_roundType;
	bool m_recursionLocked;

	class View;
	class PropertyGeneralWidget;
};

//###############################################################

/** View class of @ref ConvertFloatInteger.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertFloatInteger::View : public CompView
{
public:
	View(ConvertFloatInteger * comp, eViewType viewType);
//	~View();
	
	virtual void draw(QPainter * p);
	
	ConvertFloatInteger* getConvertFloatInteger() { return (ConvertFloatInteger*) CompView::getComponent(); };

protected:
	
private:	
};

//###############################################################

/** The property widget for @ref ConvertFloatInteger components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertFloatInteger::PropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
public:
	PropertyGeneralWidget(ConvertFloatInteger * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ConvertFloatInteger* getConvertFloatInteger() { return (ConvertFloatInteger*) ComponentPropertyGeneralWidget::getComponent(); };
	
protected:
	QLabel * m_roundTypeLabel;
	QComboBox * m_roundType;

};


//###############################################################


};  //namespace KSimLibInteger
#endif
