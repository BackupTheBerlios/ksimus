/***************************************************************************
                          floatstylerange1out.h  -  description
                             -------------------
    begin                : Sat Feb 2 2002
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

#ifndef FLOATSTYLERANGE1OUT_H
#define FLOATSTYLERANGE1OUT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/componentpropertygeneralwidget.h"
#include "floatstyle1out.h"

// Forward declaration
class KSimDoubleEdit;
class QLabel;


namespace KSimLibFloatingPoint
{

/**Base class for components with one double output with a range.
  *@author Rasmus Diekenbrock
  */

class FloatStyleRange1Out : public FloatStyle1Out
{
  Q_OBJECT
	
public:
//	~FloatStyleRange1Out();


	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Checks the component property. The functions is called after the
	*		property dialog.
	*   Eg. all connectors have unique names.
	*   The implementation checks the range and the reset value and calls the default implementation.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(Component * comp, QWidget *parent);

	/** Returns the maximum value. */
	double getInitMaxValue() const { return m_initMax; };
	/** Returns the minimum value. */
	double getInitMinValue() const { return m_initMin; };
	/** Returns the maximum value. */
	double getMaxValue() const { return m_max; };
	/** Returns the minimum value. */
	double getMinValue() const { return m_min; };
	
	/** Returns the maximum value. */
	void setMaxValue(double max, bool init = false);
	/** Returns the minimum value. */
	void setMinValue(double min, bool init = false);
	
	/** Allows swaping of the min and max value.
	  * If false the condition minValue <= resetValue <= maxValue has to be fulfilled.
	  * If true the condition minValue >= resetValue >= maxValue is also okay.
	  *
	  * The values are tested during a call of @ref checkProperty.
	  */
	void setMinMaxSwapAllowed(bool swap);
	/** Returns true if min max swaping is allowed.
	  * @see setMinMaxSwapAllowed
	  */
	bool isMinMaxSwapAllowed() const;


public slots:

protected:
	FloatStyleRange1Out(CompContainer * container, const ComponentInfo * ci);
	
private:	
	double m_initMax;
	double m_initMin;
	double m_max;
	double m_min;
	Q_UINT32 m_flags;

signals:
	void minChanged(double d);
	void maxChanged(double d);
};

//#######################################################################
//#######################################################################

/** The view widget for @ref FloatStyleRange1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */

class FloatStyleRange1OutView : public KSimLibFloatingPoint::FloatStyle1OutView
{
	Q_OBJECT

public:	
	FloatStyleRange1OutView(FloatStyleRange1Out * comp, eViewType viewType)
		:	FloatStyle1OutView(comp, viewType) {};
	
	FloatStyleRange1Out* getComponent() { return (FloatStyleRange1Out*) FloatStyle1OutView::getComponent(); };
};

//#######################################################################
//#######################################################################



/** The property widget for the @ref FloatInputSlider.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class FloatStyleRange1OutPropertyGeneralWidget : public KSimLibFloatingPoint::FloatStyle1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	FloatStyleRange1OutPropertyGeneralWidget(FloatStyleRange1Out * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	FloatStyleRange1Out* getComponent() { return (FloatStyleRange1Out*) FloatStyle1OutPropertyGeneralWidget::getComponent(); };
//	FloatInputSlider* getSlider() { return (FloatInputSlider*) getComponent(); };
	
	/** Returns the a pointer to the @ref KSimDoubleEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimDoubleEdit * getMaxValueEdit() const { return m_maxValue; };
	/** Returns the a pointer to the label of the @ref getMaxValueEdit. Use this to set a appropriate
	  * text, @ref QToolTip and @ref QWhatsThis. */
	QLabel * getMaxValueLabel() const { return m_maxValueLabel; };

	/** Returns the a pointer to the @ref KSimDoubleEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimDoubleEdit * getMinValueEdit() const { return m_minValue; };
	/** Returns the a pointer to the label of the @ref getMinValueEdit. Use this to set a appropriate
	  * text, @ref QToolTip and @ref QWhatsThis. */
	QLabel * getMinValueLabel() const { return m_minValueLabel; };

protected:
	KSimDoubleEdit * m_minValue;
	QLabel * m_minValueLabel;
	KSimDoubleEdit * m_maxValue;
	QLabel * m_maxValueLabel;
};


//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
