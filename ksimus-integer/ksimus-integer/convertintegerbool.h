/***************************************************************************
                          convertintegerbool.h  -  description
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

#ifndef CONVERTINTEGERBOOL_H
#define CONVERTINTEGERBOOL_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/ksimbaseint.h"



// Forward declaration
class ConnectorIntegerIn;
class ConnectorBoolOut;
class QLabel;
class KSimBaseIntEdit;

namespace KSimLibInteger
{

/**Component for converting an integer value in a boolean value.
  *@author Rasmus Diekenbrock
  */

class ConvertIntegerBool : public Component
{
public:
	/** Creates the component.
	  */
	ConvertIntegerBool(CompContainer * container, const ComponentInfo * ci);
//	~ConvertIntegerBool();
	
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

	ConnectorIntegerIn * getInput() { return m_input; };
	ConnectorBoolOut * getOutput() { return m_output; };
	
	const KSimBaseInt & getFalseThreshold() const { return m_falseThreshold; };
	void setFalseThreshold(const KSimBaseInt & limit);

	const KSimBaseInt & getTrueThreshold() const { return m_trueThreshold; };
	void setTrueThreshold(const KSimBaseInt & limit);
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

	static const KSimBaseInt DefaultFalseThreshold;
	static const KSimBaseInt DefaultTrueThreshold;

protected:

	ConnectorIntegerIn * m_input;
	ConnectorBoolOut * m_output;
	
	KSimBaseInt m_falseThreshold;
	KSimBaseInt m_trueThreshold;
	bool m_recursionLocked;
		
	class View;
	class PropertyGeneralWidget;
};

//###############################################################

/** View class of @ref ConvertIntegerBool.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertIntegerBool::View : public CompView
{
public:
	View(ConvertIntegerBool * comp, eViewType view, const char * name);
//	~View();

	virtual void draw(QPainter * p);
	
	ConvertIntegerBool* getConvertIntegerBool() { return (ConvertIntegerBool*) getComponent(); };

protected:
	
private:	
};

//###############################################################

/** The property widget for @ref ConvertIntegerBool components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class ConvertIntegerBool::PropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
public:
	PropertyGeneralWidget(ConvertIntegerBool * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	ConvertIntegerBool* getConvertIntegerBool() { return (ConvertIntegerBool*) getComponent(); };


protected:
	KSimBaseIntEdit * m_trueThreshold;
	QLabel * m_trueThresholdLabel;
	KSimBaseIntEdit * m_falseThreshold;
	QLabel * m_falseThresholdLabel;

};


//###############################################################


};  //namespace KSimLibInteger
#endif
