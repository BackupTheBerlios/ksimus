/***************************************************************************
                          floatlineinput.h  -  description
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

#ifndef FLOATLINEINPUT_H
#define FLOATLINEINPUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/compviewhbox.h"
#include "floatstylerange1out.h"

// Forward declaration
class KSimDoubleEdit;
class QLabel;
class QSpinBox;
class QCheckBox;
class QComboBox;



namespace KSimLibFloatingPoint
{

const ComponentInfo * getFloatLineInputInfo();


/**A line input component for double values.
  *@author Rasmus Diekenbrock
  */

class FloatLineInput : public KSimLibFloatingPoint::FloatStyleRange1Out
{
  Q_OBJECT
	
public:
	FloatLineInput(CompContainer * container, const ComponentInfo * ci);
//	~FloatLineInput();


	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Reset all simulation variables */
	virtual void reset();
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Enables the value tracking during the input. If the current input is a valid value
	  * and is in the given range the signal @ref valueChanged is emitted. If tracking is
	  * disabled the signal is emitted only after retrun pressed or the input line was leaved.
	  * The default is enabled.
	  */
	void setTrackingEnabled(bool tracking);
	/** Returns true if tracking is enabled.
	  * @see setTrackingEnabled
	  */
	bool isTrackingEnabled() const;
	
	/** Returns the maximum number of digits after the decimal point. */
	int getDecimals() const;
	
	/** Sets the maximum number of digits after the decimal point. The default is 3. */
	void setDecimals(int decimals);

	/** Sets the convertion type. Allowed types are 'g', 'G', 'e', 'E', 'g' and 'G'. See the printf docu.
		*/
	void setConversionType(char type);
	/** Gets the convertion type. Allowed types are 'g', 'G', 'e', 'E', 'g' and 'G'. See the printf docu.
		*/
	char getConversionType() const;

public slots:
	void newValue(double i);
	void newValueTracking(double i);


private:
	int m_decimals;
	Q_UINT32 m_flags;

signals:
	void valueChanged(double i);
	void decimalsChanged(int);
	void trackingChanged(bool);
	void conversionTypeChanged(char);
};

//#######################################################################
//#######################################################################

class FloatLineInputView : public KSimLibFloatingPoint::FloatStyleRange1OutView
{

	Q_OBJECT

public:	
	FloatLineInputView(FloatLineInput * comp, eViewType viewType);
//	virtual ~FloatLineInputView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	FloatLineInput * getComponent() { return (FloatLineInput *)FloatStyleRange1OutView::getComponent(); };


protected:
	
private:

signals:
};

//#######################################################################
//#######################################################################

class FloatLineInputWidgetView : public CompViewHBox
{
	Q_OBJECT

friend class FloatLineInputView;

public:
	FloatLineInputWidgetView(FloatLineInputView * cv, QWidget *parent=0, const char *name=0);
//	~FloatLineInputWidgetView();

	FloatLineInput * getComponent() const { return (FloatLineInput *)getCompView()->getComponent(); };

public slots:
	void setMinValue(double d);
	void setMaxValue(double d);
	void setTrackingEnabled(bool tracking);
	void setDecimals(int decimals);
	void setConversionType(char);

signals:

protected:
	KSimDoubleEdit * m_edit;
};


//#######################################################################
//#######################################################################



/** The property widget for the @ref FloatLineInput.
  * This class adds to the general property dialog a item for selecting
  * the number of decimals and the value tracking.
  *
  * @author Rasmus Diekenbrock
  */


class FloatLineInputPropertyGeneralWidget : public KSimLibFloatingPoint::FloatStyleRange1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	FloatLineInputPropertyGeneralWidget(FloatLineInput * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this functionx^, if you add new properties.
	 */
	virtual void defaultPressed();

	FloatLineInput* getComponent() { return (FloatLineInput*) FloatStyleRange1OutPropertyGeneralWidget::getComponent(); };
	
protected:
	QLabel * m_convertTypeLabel;
	QComboBox * m_convertType;
	
	QLabel * m_decimalsLabel;
	QSpinBox * m_decimals;
	QCheckBox * m_tracking;
};

//#######################################################################
//#######################################################################

};  //namespace KSimLibFloatingPoint
#endif
