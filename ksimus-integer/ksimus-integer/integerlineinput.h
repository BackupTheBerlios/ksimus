/***************************************************************************
                          integerlineinput.h  -  description
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

#ifndef INTEGERLINEINPUT_H
#define INTEGERLINEINPUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/compviewhbox.h"
#include "integerstylerange1out.h"

// Forward declaration
class QLabel;
class QCheckBox;
class QComboBox;
class KSimBaseIntEdit;



namespace KSimLibInteger
{

/**A line input component for integer values.
  *@author Rasmus Diekenbrock
  */

class IntegerLineInput : public IntegerStyleRange1Out
{
  Q_OBJECT
	
public:
	IntegerLineInput(CompContainer * container, const ComponentInfo * ci);
//	~IntegerLineInput();


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
	void setTrackingEnabled(bool tracking, bool init = false);
	/** Returns true if tracking is enabled.
	  * @see setTrackingEnabled
	  */
	bool isTrackingEnabled() const;
	bool isTrackingInitEnabled() const;
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

public slots:
	void newValue(const KSimBaseInt & i);
	void newValueTracking(const KSimBaseInt & i);


private:
	struct
	{
		unsigned int tracking     : 1;
		unsigned int trackingInit : 1;
	} m_flags;

private slots:

signals:
	void valueChanged(const KSimBaseInt & i);
	void trackingChanged(bool);
};

//#######################################################################
//#######################################################################

class IntegerLineInputView : public CompViewSize
{

	Q_OBJECT

public:	
	IntegerLineInputView(IntegerLineInput * comp, eViewType viewType);
//	virtual ~IntegerLineInputView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	IntegerLineInput * getIntegerLineInput() { return (IntegerLineInput *)CompViewSize::getComponent(); };


protected:
	
private:

signals:
};

//#######################################################################
//#######################################################################

class IntegerLineInputWidgetView : public CompViewHBox
{
	Q_OBJECT

friend class IntegerLineInputView;

public:
	IntegerLineInputWidgetView(IntegerLineInputView * cv, QWidget *parent=0, const char *name=0);
//	~IntegerLineInputWidgetView();

	IntegerLineInput * getIntegerLineInput() const { return (IntegerLineInput *)getCompView()->getComponent(); };

public slots:
	void setMinValue(int d);
	void setMaxValue(int d);
	void setTrackingEnabled(bool tracking);

signals:

protected:
	KSimBaseIntEdit * m_edit;
};


//#######################################################################
//#######################################################################



/** The property widget for the @ref IntegerLineInput.
  * This class adds to the general property dialog a item for selecting
  * the number of decimals and the value tracking.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerLineInputPropertyGeneralWidget : public IntegerStyleRange1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	IntegerLineInputPropertyGeneralWidget(IntegerLineInput * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this functionx^, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerLineInput* getIntegerLineInput() { return (IntegerLineInput*) IntegerStyleRange1OutPropertyGeneralWidget::getComponent(); };
	
protected:
	QCheckBox * m_tracking;
};

//#######################################################################
//#######################################################################

};  //namespace KSimLibInteger
#endif
