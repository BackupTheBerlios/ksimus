/***************************************************************************
                          integerspinbox.h  -  description
                             -------------------
    begin                : Wen Jul 30 2003
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

#ifndef INTEGERSPINBOX_H
#define INTEGERSPINBOX_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/compviewhbox.h"
#include "integerstylerange1out.h"

// Forward declaration
class QLabel;
class QLineEdit;
class QMultiLineEdit;
class QCheckBox;
class KSimBaseIntSpinBox;



namespace KSimLibInteger
{

/**A spinbox component for integer values.
  *@author Rasmus Diekenbrock
  */

class IntegerSpinbox : public IntegerStyleRange1Out
{
  Q_OBJECT
	
public:
	IntegerSpinbox(CompContainer * container, const ComponentInfo * ci);
//	~IntegerSpinbox();


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

	/** Checks the component property. The functions is called after the
	*   property dialog.
	*   Eg. all connectors have unique names.
	*   The default implementation checks the connector names and calls the checks functions of the connectors.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);

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
	
	/** Enables the warp around property.
	  * The default is disabled.
	  */
	void setWraparoundEnabled(bool warparound, bool init = false);
	/** Returns true if warp around is enabled.
	  * @see setWraparoundEnabled
	  */
	bool isWraparoundEnabled() const;
	bool isWraparoundInitEnabled() const;

	/** Sets the line step.
	  * The default is 1.
	  */
	void setLineStep(const KSimBaseInt & lines, bool init = false);
	/** Returns true if warp around is enabled.
	  * @see setWraparoundEnabled
	  */
	const KSimBaseInt & getLineStep() const { return m_lineStep; };
	const KSimBaseInt & getLineStepInit() const { return m_lineStepInit; };

	void setSpecialValueText(const QString & text);
	QString getSpecialValueText() const { return m_specialValueText; };

	void setPrefixText(const QString & text);
	QString getPrefixText() const { return m_prefixText; };

	void setSuffixText(const QString & text);
	QString getSuffixText() const { return m_suffixText; };

	void setConstantList(const QString & text);
	QString getConstantList() const { return m_constantList; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

public slots:
	void newValue(const KSimBaseInt & i);
	void newValueTracking(const KSimBaseInt & i);


private:
	KSimBaseInt m_currentValue;
	KSimBaseInt m_currentValueTracking;
	KSimBaseInt m_lineStep;
	KSimBaseInt m_lineStepInit;
	QString m_specialValueText;
	QString m_prefixText;
	QString m_suffixText;
	QString m_constantList;
	struct
	{
		unsigned int tracking       : 1;
		unsigned int trackingInit   : 1;
		unsigned int warparound     : 1;
		unsigned int warparoundInit : 1;
	} m_flags;

signals:
	void valueChanged(const KSimBaseInt & i);
	void trackingChanged(bool);
	void wraparoundChanged(bool);
	void lineStepChanged(int);
	void specialValueTextChanged(const QString &);
	void prefixTextChanged(const QString &);
	void suffixTextChanged(const QString &);
	void constantListChanged(const QString &);
};

//#######################################################################
//#######################################################################

class IntegerSpinboxView : public IntegerStyleRange1OutView
{

	Q_OBJECT

public:
	IntegerSpinboxView(IntegerSpinbox * comp, eViewType viewType);
//	virtual ~IntegerSpinboxView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	IntegerSpinbox * getIntegerSpinbox() { return (IntegerSpinbox *)IntegerStyleRange1OutView::getComponent(); };


protected:
	
private:

signals:
};

//#######################################################################
//#######################################################################

class IntegerSpinboxWidgetView : public CompViewHBox
{
	Q_OBJECT

friend class IntegerSpinboxView;

public:
	IntegerSpinboxWidgetView(IntegerSpinboxView * cv, QWidget *parent=0, const char *name=0);
//	~IntegerSpinboxWidgetView();

	IntegerSpinbox * getIntegerSpinbox() const { return (IntegerSpinbox *)getCompView()->getComponent(); };

public slots:
	void setMinValue(int d);
	void setMaxValue(int d);
	void setTrackingEnabled(bool tracking);
	void setWraparoundEnabled(bool wraparound);
	void setLineStep(int lines);
	void setSpecialValueText(const QString &);
	void setPrefixText(const QString &);
	void setSuffixText(const QString &);
	void setConstantList(const QString & text);


signals:

protected:
	KSimBaseIntSpinBox * m_spinbox;
};


//#######################################################################
//#######################################################################



/** The property widget for the @ref IntegerSpinbox.
  * This class adds to the general property dialog a item for selecting
  * the number of decimals and the value tracking.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerSpinboxPropertyGeneralWidget : public IntegerStyleRange1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	IntegerSpinboxPropertyGeneralWidget(IntegerSpinbox * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this functionx^, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerSpinbox* getIntegerSpinbox() { return (IntegerSpinbox*) IntegerStyleRange1OutPropertyGeneralWidget::getComponent(); };
	
protected:
	KSimBaseIntSpinBox * m_lineStep;
	QCheckBox * m_tracking;
	QCheckBox * m_wraparound;
	QLineEdit * m_specialValueText;
	QLineEdit * m_prefixText;
	QLineEdit * m_suffixText;
	QMultiLineEdit * m_constantList;
};

//#######################################################################
//#######################################################################

};  //namespace KSimLibInteger
#endif
