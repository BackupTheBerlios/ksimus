/***************************************************************************
                          integerdelay.h  -  description
                             -------------------
    begin                : Sat Aug 2 2003
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

#ifndef INTEGERDELAY_H
#define INTEGERDELAY_H

// C-Includes

// QT-Includes
#include <qmemarray.h>

// KDE-Includes

// KSimus-Includes
#include "ksimus/ksimtime.h"
#include "integer1in1out.h"

// Project-Includes

// Forward declaration
class ConnectorIntegerIn;
class KSimTimeSpinBox;


namespace KSimLibInteger
{

/**An integer delay.
  *@author Rasmus Diekenbrock
  */

class IntegerDelay : public Integer1In1Out
{
public:
	IntegerDelay(CompContainer * container, const ComponentInfo * ci);
//	~IntegerDelay();

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
	  * This function creeates a @ref DelayPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	void setDelayTime(const KSimTimeBase & delay) { m_delayTime = delay; };
	const KSimTimeBase & getDelayTime()const { return m_delayTime; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

/*protected:*/
	class View;
	class PropertyGeneralWidget;

private:
	KSimTime m_delayTime;
	int m_lastValue;
	unsigned int m_counter;
	unsigned int m_index;
	QMemArray<int> m_list;
	
	static const unsigned long maxDepth;
	static const char * const strDelayTime;

};

//###############################################################
//###############################################################


class IntegerDelay::View : public CompView
{
public:
	View(IntegerDelay * comp, eViewType viewType, const char * name);
//	~View();
	virtual void draw(QPainter * p);
	
	IntegerDelay * getIntegerDelay() const { return (IntegerDelay*) getComponent(); };

};


//###############################################################
//###############################################################

class IntegerDelay::PropertyGeneralWidget : public Integer1In1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	PropertyGeneralWidget(IntegerDelay * comp, QWidget *parent=0, const char *name=0);
//	~PropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerDelay * getIntegerDelay() const { return (IntegerDelay*) getComponent(); };

private slots:
	void slotChanged(const KSimTime & time);


private:
	KSimTimeSpinBox * m_delayTime;
	QLabel * m_delayTimeHint;
};

//###############################################################

}; //namespace KSimLibInteger

#endif
