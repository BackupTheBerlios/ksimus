/***************************************************************************
                          integerstylerange1out.h  -  description
                             -------------------
    begin                : Mon Jul 14 2003
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

#ifndef INTEGERSTYLERANGE1OUT_H
#define INTEGERSTYLERANGE1OUT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integerstyle1out.h"

// Forward declaration
class QLabel;
class KSimBaseIntEdit;


namespace KSimLibInteger
{

/**Base class for components with one integer output with a range.
  *@author Rasmus Diekenbrock
  */

class IntegerStyleRange1Out : public IntegerStyle1Out
{
  Q_OBJECT
	
public:
	~IntegerStyleRange1Out();


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
	  * This function creeates a @ref IntegerStyleRange1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Sets the maximum value. */
	void setMaxValue(const KSimBaseInt &  max);
	/** Returns the maximum value. */
	const KSimBaseInt & getMaxValue() const { return m_max; };

	/** Sets the minimum value. */
	void setMinValue(const KSimBaseInt &  min);
	/** Returns the minimum value. */
	const KSimBaseInt & getMinValue() const { return m_min; };
	
	/** Sets the initial value of the maximum value. */
	void setInitMaxValue(const KSimBaseInt & max);
	/** Returns the initial value of the maximum value. */
	const KSimBaseInt & getInitMaxValue() const { return m_initMax; };

	/** Sets the initial value of the minimum value. */
	void setInitMinValue(const KSimBaseInt &  min);
	/** Returns the initial value of the minimum value. */
	const KSimBaseInt & getInitMinValue() const { return m_initMin; };

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
	IntegerStyleRange1Out(CompContainer * container, const ComponentInfo * ci);
	
private:
	KSimBaseInt m_min;
	KSimBaseInt m_max;
	KSimBaseInt m_initMin;
	KSimBaseInt m_initMax;
	struct
	{
		unsigned int swapRangeAllowed : 1;
	} m_flags;

signals:
	void minValueChanged(int i);
	void maxValueChanged(int i);
};

//#######################################################################
//#######################################################################

/** The view widget for @ref IntegerStyleRange1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */

class IntegerStyleRange1OutView : public IntegerStyle1OutView
{
	Q_OBJECT

public:	
	IntegerStyleRange1OutView(IntegerStyleRange1Out * comp, eViewType viewType, const char * name)
	:	IntegerStyle1OutView(comp, viewType, name) {};
	
	IntegerStyleRange1Out* getComponent() { return (IntegerStyleRange1Out*) IntegerStyle1OutView::getComponent(); };
};

//#######################################################################
//#######################################################################



/** The property widget for the @ref IntegerStyleRange1Out.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerStyleRange1OutPropertyGeneralWidget : public IntegerStyle1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	IntegerStyleRange1OutPropertyGeneralWidget(IntegerStyleRange1Out * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerStyleRange1Out* getIntegerStyleRange1Out() { return (IntegerStyleRange1Out*) getIntegerStyle1Out(); };
	
	/** Returns the a pointer to the @ref KSimBaseIntEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBaseIntEdit * getMaxValueEdit() const { return m_maxValue; };
	/** Returns the a pointer to the label of the @ref getMaxValueEdit. Use this to set a appropriate
	  * text, @ref QToolTip and @ref QWhatsThis. */
	QLabel * getMaxValueLabel() const { return m_maxValueLabel; };

	/** Returns the a pointer to the @ref KSimBaseIntEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBaseIntEdit * getMinValueEdit() const { return m_minValue; };
	/** Returns the a pointer to the label of the @ref getMinValueEdit. Use this to set a appropriate
	  * text, @ref QToolTip and @ref QWhatsThis. */
	QLabel * getMinValueLabel() const { return m_minValueLabel; };

protected:
	KSimBaseIntEdit * m_minValue;
	QLabel * m_minValueLabel;
	KSimBaseIntEdit * m_maxValue;
	QLabel * m_maxValueLabel;
};


//###############################################################


};  //namespace KSimLibInteger
#endif
