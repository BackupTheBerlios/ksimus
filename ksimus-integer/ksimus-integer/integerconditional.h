/***************************************************************************
                          integerconditional.h  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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

#ifndef INTEGERCONDITIONAL_H
#define INTEGERCONDITIONAL_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/boolean1out.h"



// Forward declaration
class ConnectorIntegerIn;
class QLabel;
class QComboBox;
class QPixmap;

namespace KSimLibInteger
{

//###############################################################

/**Class for compare integer values.
  *@author Rasmus Diekenbrock
  */

class IntegerConditional : public Boolean1Out
{
public:
	enum eConditionalType { eLesser = 0, eLesserEqual, eEqual, eLargerEqual, eLarger };

	/** Creates the component.
	  */
	IntegerConditional(CompContainer * container, const ComponentInfo * ci);
//	~IntegerConditional();
	
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Returns true, if the parameter type is a proper reload type. This is useful if a component
	  * supports more than one types and the type decides the different functionality (e.g.
	  * @ref BooleanButton). The implementation returns true, if type is on of the conditionals.*/
	virtual bool isProperReloadType(const QString & type) const;
	/** Executes the simulation of this component */
	virtual void calculate();
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref SimpleConditionalPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	ConnectorIntegerIn * getInputA() { return m_inputA; };
	ConnectorIntegerIn * getInputB() { return m_inputB; };
	
	eConditionalType getConditionalType() const { return m_conditionalType; };
	void setConditionalType(eConditionalType newCond);
	void setConditionalType(const QString & type);

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticLesserInfo();
	static const ComponentInfo * getStaticLesserEqualInfo();
	static const ComponentInfo * getStaticEqualInfo();
	static const ComponentInfo * getStaticLargerEqualInfo();
	static const ComponentInfo * getStaticLargerInfo();

protected:
	class View;
	class PropertyGeneralWidget;
	ConnectorIntegerIn * m_inputA;
	ConnectorIntegerIn * m_inputB;
	
	eConditionalType m_conditionalType;
};

//###############################################################

/** View class for @ref IntegerConditional views.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerConditional::View : public CompView
{
friend class IntegerConditional::PropertyGeneralWidget;

public:
	View(IntegerConditional * comp, eViewType viewType);
	~View();

	virtual void draw(QPainter * p);
	
	IntegerConditional* getIntegerConditional() { return (IntegerConditional*) CompView::getComponent(); };

	// Pixmaps
	static const char * AltB_xpm[];
	static const char * AlteqB_xpm[];
	static const char * AeqB_xpm[];
	static const char * AgteqB_xpm[];
	static const char * AgtB_xpm[];

protected:
	static unsigned int sInstanceCount;
	static QPixmap * sPixmapLesser;
	static QPixmap * sPixmapLesserEqual;
	static QPixmap * sPixmapEqual;
	static QPixmap * sPixmapLargerEqual;
	static QPixmap * sPixmapLarger;
	static int sPixmapOffsetTop;
	static int sPixmapOffsetLeft;
	
private:
};

//###############################################################

/** The property widget for @ref SimpleConditional components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerConditional::PropertyGeneralWidget : public Boolean1OutPropertyGeneralWidget
{
public:
	PropertyGeneralWidget(IntegerConditional * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerConditional* getIntegerConditional() { return (IntegerConditional*) Boolean1OutPropertyGeneralWidget::getComponent(); };
	
	
protected:
	QLabel * m_conditionalLabel;
	QComboBox * m_conditional;
};


//###############################################################


};  //namespace KSimLibInteger
#endif
