/***************************************************************************
                          integerconstinput.h  -  description
                             -------------------
    begin                : Sun Aug 11 2003
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

#ifndef INTEGERCONSTINPUT_H
#define INTEGERCONSTINPUT_H

// C-Includes

// QT-Includes
#include <qdialog.h>

// KDE-Includes

// Project-Includes
#include "ksimus/compviewhbox.h"

#include "integer1out.h"

// Forward declaration
class ComponentLayout;
class QLabel;



namespace KSimLibInteger
{

/**A constant component with integer output connector.
  *@author Rasmus Diekenbrock
  */

class IntegerConstInput : public Integer1Out
{
	Q_OBJECT
	
public:

	IntegerConstInput(CompContainer * container, const ComponentInfo * ci);
//	~IntegerConstInput();

	/** Reset all simulation variables */
	void reset();
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	

	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);

	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);


	/** Updates widget value. */
	void updateValue(const KSimBaseInt & val);
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	
private slots:

signals:
	void signalSetText(const QString & value);
};

//#######################################################################
//#######################################################################

class IntegerConstInputEdit : public QDialog
{
public:
	IntegerConstInputEdit(IntegerConstInput * comp, const char * name);

	void setValue(const KSimBaseInt & val);
	KSimBaseInt getValue() const;

private:
	KSimBaseIntEdit * m_edit;
};


//#######################################################################
//#######################################################################

class IntegerConstInputView : public CompViewSize
{
public:
	IntegerConstInputView(IntegerConstInput * comp, eViewType viewType, const char * name);
//	virtual ~IntegerConstInputView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	IntegerConstInput * getIntegerConstInput() const { return (IntegerConstInput *)CompViewSize::getComponent(); };


protected:
	virtual void resize();

	
private:


signals:
};

//#######################################################################
//#######################################################################

class IntegerConstInputWidgetView : public CompViewHBox
{
friend class IntegerConstInputView;
	
public:
	IntegerConstInputWidgetView(IntegerConstInputView * cv, QWidget *parent=0, const char *name=0);
//	~IntegerConstInputWidgetView();

	IntegerConstInput * getIntegerConstInput() const { return (IntegerConstInput *)getCompView()->getComponent(); };

protected:
	QLabel * m_label;
};

//#######################################################################
//#######################################################################

/** The property widget for @ref IntegerConstInput components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerConstInputPropertyGeneralWidget : public Integer1OutPropertyGeneralWidget
{
public:
	IntegerConstInputPropertyGeneralWidget(IntegerConstInput * comp, QWidget *parent=0, const char *name=0);

	IntegerConstInput* getIntegerConstInput() { return (IntegerConstInput*) getComponent(); };
	
	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
};


//#######################################################################
//#######################################################################


};  //namespace KSimLibInteger


#endif
