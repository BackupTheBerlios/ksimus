/***************************************************************************
                          integerlineoutput.h  -  description
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

#ifndef INTEGERLINEOUTPUT_H
#define INTEGERLINEOUTPUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/componentstyle.h"
#include "ksimus/compview.h"
#include "ksimus/compviewvbox.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/ksimbaseint.h"

// Forward declaration
class QLabel;
class ConnectorIntegerIn;
class KSimBaseIntBaseSelection;



namespace KSimLibInteger
{

/**A simple integer output.
  *@author Rasmus Diekenbrock
  */

class IntegerLineOutput : public ComponentStyle
{
	Q_OBJECT
	
public:

	IntegerLineOutput(CompContainer * container, const ComponentInfo * ci);
//	~IntegerLineOutput();

	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref IntegerLineOutputPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	void setBase(KSimBaseInt::eBaseType base);
	KSimBaseInt::eBaseType getBase() const { return m_base; };

	const KSimBaseInt & getNumber() const { return m_number; };
	

	ConnectorIntegerIn * getInput() const { return m_input; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
	static const KSimBaseInt::eBaseType defaultBase;

protected: // Protected attributes
	KSimBaseInt m_number;
	KSimBaseInt::eBaseType m_base;
	
	ConnectorIntegerIn * m_input;

signals:
	void signalSetText(const QString & newNumber);
};

//#######################################################################
//#######################################################################

class IntegerLineOutputView : public CompViewSize
{

	Q_OBJECT

public:	
	IntegerLineOutputView(IntegerLineOutput * comp, eViewType viewType, const char * name);
//	virtual ~IntegerLineOutputView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	IntegerLineOutput * getLineOutput() const { return (IntegerLineOutput *)getComponent(); };


protected:
	virtual void resize();

	
private:

signals:
};

//#######################################################################
//#######################################################################

class IntegerLineOutputWidgetView : public CompViewVBox
{
	Q_OBJECT

friend class IntegerLineOutputView;
	
public:
	IntegerLineOutputWidgetView(IntegerLineOutputView * cv, QWidget *parent=0, const char *name=0);
//	~IntegerLineOutputWidgetView();

	IntegerLineOutput * getLineOutput() const { return (IntegerLineOutput *)getCompView()->getComponent(); };

protected:
	QLabel * m_label;
};

//#######################################################################
//#######################################################################



/** The property widget for the @ref IntegerLineOutput.
  * This class adds to the general property dialog a item for selecting
  * the number of decimals and the value tracking.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerLineOutputPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	IntegerLineOutputPropertyGeneralWidget(IntegerLineOutput * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerLineOutput* getIntegerLineOutput() { return (IntegerLineOutput*) ComponentPropertyGeneralWidget::getComponent(); };

protected:
	KSimBaseIntBaseSelection * m_baseSel;
};

//#######################################################################
//#######################################################################

};  //namespace KSimLibInteger
#endif
