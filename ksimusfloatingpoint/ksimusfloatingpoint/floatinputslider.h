/***************************************************************************
                          floatinputslider.h  -  description
                             -------------------
    begin                : Sun Dec 30 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#ifndef FLOATINPUTSLIDER_H
#define FLOATINPUTSLIDER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/compviewvbox.h"
#include "ksimus/connectorfloatout.h"
#include "floatstyle1out.h"

// Forward declaration
class QSlider;
class KSimDoubleEdit;
class QLabel;


namespace KSimLibFloatingPoint
{

extern const ComponentInfo FloatInputSliderInfo;


/**A slider input for float values.
  *@author Rasmus Diekenbrock
  */

class FloatInputSlider : public KSimLibFloatingPoint::FloatStyle1Out
{
  Q_OBJECT
	
public:
	FloatInputSlider(CompContainer * container, const ComponentInfo * ci);
//	~FloatInputSlider();


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
	virtual ComponentPropertyBaseWidget * createGeneralProperty(Component * comp, QWidget *parent);


	/** Converts value from slider (uses int) to double. */
	double fromSlider(int) const;
	/** Converts value from double to slider (uses int). */
	int toSlider(double) const;


	/** Returns the maximum value. */
	double getMaxValue() const { return m_max; };
	/** Returns the minimum value. */
	double getMinValue() const { return m_min; };
	
	/** Returns the maximum value. */
	void setMaxValue(double max);
	/** Returns the minimum value. */
	void setMinValue(double min);
	
public slots:
	void newValue(int i);


protected: // Protected attributes
	double m_max;
	double m_min;

signals:
	void valueChanged(int i);
};

//#######################################################################
//#######################################################################

class FloatInputSliderView : public KSimLibFloatingPoint::FloatStyle1OutView
{

	Q_OBJECT

public:	
	FloatInputSliderView(FloatInputSlider * comp, eViewType viewType);
//	virtual ~FloatInputSliderView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	FloatInputSlider * getSlider() const { return (FloatInputSlider *)getComponent(); };


protected:
	virtual void resize();
	void makeOrientation();

	
private:


signals:
	void signalOrientation(Orientation orient);
};

//#######################################################################
//#######################################################################

class FloatInputSliderWidgetView : public CompViewVBox
{
	Q_OBJECT

friend class FloatInputSliderView;
	
public:
	FloatInputSliderWidgetView(FloatInputSliderView * cv, QWidget *parent=0, const char *name=0);
//	~FloatLineOutputWidgetView();

	FloatInputSlider * getSlider() const { return (FloatInputSlider *)getCompView()->getComponent(); };

public slots:
	void setValue(int i);
	void setOrientation(Orientation orient);
	
private slots:
	void silderValueChanged(int i);

signals:
	void valueChanged(int i);

protected:
	QSlider * m_slider;
};

//#######################################################################
//#######################################################################



/** The property widget for the @ref FloatInputSlider.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class FloatInputSliderPropertyGeneralWidget : public KSimLibFloatingPoint::FloatStyle1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	FloatInputSliderPropertyGeneralWidget(FloatInputSlider * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this functionx^, if you add new properties.
	 */
	virtual void defaultPressed();

	FloatInputSlider* getSlider() { return (FloatInputSlider*) getComponent(); };
	
protected:
	KSimDoubleEdit * m_minValue;
	QLabel * m_minValueLabel;
	KSimDoubleEdit * m_maxValue;
	QLabel * m_maxValueLabel;
};


//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
