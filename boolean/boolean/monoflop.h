/***************************************************************************
                          monoflop.h  -  description
                             -------------------
    begin                : Tue Dec 11 2001
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

#ifndef MONOFLOP_H
#define MONOFLOP_H

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus-Includes
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"
#include "ksimus/ksimtime.h"

// Project-Includes
#include "flipflopbase.h"

// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolInEdge;
class ComponentLayout;
class KSimTimeSpinBox;
class KSimBooleanBox;


namespace KSimLibBoolean
{

extern const ComponentInfo MonoFlopInfo;


//###############################################################
//###############################################################

/**A MonoFlop.
  *@author Rasmus Diekenbrock
  */

class MonoFlop : public KSimLibBoolean::FlipFlopBase
{
	Q_OBJECT

public:
	MonoFlop(CompContainer * container, const ComponentInfo * ci);
//	~MonoFlop();

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
	  * This function creeates a @ref MonoFlopPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(Component * comp, QWidget *parent);

  void setHighTime(const KSimTimeBase & time) { m_highTime = time; };
  const KSimTimeBase & getHighTime()const { return m_highTime; };

  void setRetrigger(bool retrig) {m_retrigger = retrig; };
  bool getRetrigger() const { return m_retrigger; };
	
  void setHighTimeUnitList(const QString & unitList) { m_highTimeUnitList = unitList; };
  QString getHighTimeUnitList() const { return m_highTimeUnitList; };


  		
private:
	bool m_retrigger;
	KSimTime m_highTime;
	KSimTime m_time;
	QString m_highTimeUnitList;

};

//###############################################################
//###############################################################


class MonoFlopView : public CompView
{
public:
	MonoFlopView(MonoFlop * comp, eViewType viewType);
//	~MonoFlopView();
	virtual void draw(QPainter * p);

private:
	ComponentLayout * m_layout;
};


//###############################################################
//###############################################################

class MonoFlopPropertyGeneralWidget : public KSimLibBoolean::FlipFlopBasePropertyGeneralWidget
{
	Q_OBJECT

public:
	MonoFlopPropertyGeneralWidget(MonoFlop * comp, QWidget *parent=0, const char *name=0);
//	~MonoFlopPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();


private:
	KSimBooleanBox * m_retrigger;
	KSimTimeSpinBox * m_highTime;
};
//###############################################################



}; //namespace KSimLibBoolean

#endif
