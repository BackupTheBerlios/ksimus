/***************************************************************************
                          random.h  -  description
                             -------------------
    begin                : Tue Sep 3 2002
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

#ifndef RANDOM_H
#define RANDOM_H

// C-Includes

// QT-Includes

// KDE-Includes
#include <krandomsequence.h>

// KSimus-Includes
#include "ksimus/component.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"
#include "ksimus/ksimtime.h"
#include "float1out.h"

// Project-Includes

// Forward declaration
class KSimBooleanBox;
class ConnectorBoolInEdge;
class ConnectorFloatOut;
class KRandomSequence;
class KSimDoubleEdit;
class KSimSpinBox;

namespace KSimLibFloatingPoint
{

/**A Random Generator.
  *@author Rasmus Diekenbrock
  */

class Random : public Component
{
	Q_OBJECT

public:
	Random(CompContainer * container, const ComponentInfo * ci);
//	~Random();

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

	/** Returns the output connector.*/
	ConnectorFloatOut * getOutputConnector() const { return m_out; };
	/** Returns the enable connector.*/
	ConnectorBoolInEdge * getEnableConnector() const { return m_enable; };

	void setUseSeed(bool useSeed) { m_useSeed = useSeed; };
	bool getUseSeed() const { return m_useSeed; };
	void setSeed(long seed) { m_seed = seed; };
	long getSeed() const { return m_seed; };
	void setMinimum(double min) { m_min = min; };
	double getMinimum() const { return m_min; };
	void setMaximum(double max) { m_max = max; };
	double getMaximum() const { return m_max; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	ConnectorFloatOut * m_out;
	ConnectorBoolInEdge * m_enable;
	KRandomSequence m_randomGen;
	long m_seed;
	bool m_useSeed;
	double m_gain;
	double m_offset;
	double m_min;
	double m_max;
	
	static const char * const sUseSeed;
	static const char * const sSeed;
	static const char * const sMin;
	static const char * const sMax;
};

//###############################################################
//###############################################################


class RandomView : public CompView
{
public:
	RandomView(Random * comp, eViewType viewType, const char * name);
//	~RandomView();
	virtual void draw(QPainter * p);
	
	Random * getRandom() const { return (Random*) getComponent(); };

};


//###############################################################
//###############################################################

class RandomPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	RandomPropertyGeneralWidget(Random * comp, QWidget *parent=0, const char *name=0);
//	~RandomPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

	Random * getRandom() const { return (Random*) getComponent(); };

private:
	KSimDoubleEdit * m_min;
	KSimDoubleEdit * m_max;
	KSimBooleanBox * m_useSeed;
	KSimSpinBox * m_seed;

};
//###############################################################



}; //namespace KSimLibBoolean

#endif
