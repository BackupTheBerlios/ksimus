/***************************************************************************
                          delay.h  -  description
                             -------------------
    begin                : Tue Jun 18 2002
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

#ifndef DELAY_H
#define DELAY_H

// C-Includes

// QT-Includes
#include <qbitarray.h>

// KDE-Includes

// KSimus-Includes
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"
#include "ksimus/ksimtime.h"
#include "ksimus/boolean1out.h"

// Project-Includes

// Forward declaration
class ConnectorBoolIn;
class KSimTimeSpinBox;


namespace KSimLibBoolean
{

//###############################################################
//###############################################################

/**A Delay.
  *@author Rasmus Diekenbrock
  */

class Delay : public Boolean1Out
{
	Q_OBJECT

public:
	Delay(CompContainer * container, const ComponentInfo * ci);
//	~Delay();

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

	ConnectorBoolIn * getInputConnector() const { return m_input; };

	void setDelayTime(const KSimTimeBase & delay) { m_delayTime = delay; };
	const KSimTimeBase & getDelayTime()const { return m_delayTime; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	ConnectorBoolIn * m_input;
	KSimTime m_delayTime;
	bool m_lastState;
	unsigned int m_counter;
	unsigned int m_index;
	QBitArray m_list;
};

//###############################################################
//###############################################################


class DelayView : public CompView
{
public:
	DelayView(Delay * comp, eViewType viewType);
//	~DelayView();
	virtual void draw(QPainter * p);
	
	Delay * getDelay() const { return (Delay*) getComponent(); };

};


//###############################################################
//###############################################################

class DelayPropertyGeneralWidget : public Boolean1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	DelayPropertyGeneralWidget(Delay * comp, QWidget *parent=0, const char *name=0);
//	~DelayPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

	Delay * getDelay() const { return (Delay*) getComponent(); };

private slots:
	void slotChanged(const KSimTime & time);


private:
	KSimTimeSpinBox * m_delayTime;
	QLabel * m_delayTimeHint;
};
//###############################################################



}; //namespace KSimLibBoolean

#endif
