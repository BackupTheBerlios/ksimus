/***************************************************************************
                          flipflopbase.h  -  description
                             -------------------
    begin                : Fri Dec 7 2001
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

#ifndef FLIPFLOPBASE_H
#define FLIPFLOPBASE_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/boolean1out.h"
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"

// Forward declaration
class ConnectorBoolOut;
class ConnectorBoolInEdge;



namespace KSimLibBoolean
{


//###############################################################
//###############################################################

/**Base class for flip flops.
  *@author Rasmus Diekenbrock
  */

class FlipFlopBase : public Boolean1Out
{
	Q_OBJECT

public:
//	~FlipFlopBase();

	/** Shifts the current component state (@ref setState) to output connecotor. */
	virtual void updateOutput();
	
	/** Resets the component state. */
	virtual void reset();
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref FlipFlopBasePropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);
	
	/** Returns the "not" output connector.
	  */
	ConnectorBoolOut * getNotOutputConnector() const { return m_notOut; };
	/** Returns the "reset" input connector.
	  */
	ConnectorBoolInEdge * getResetInputConnector() const { return m_inReset; };
	/** Returns the "set" input connector.
	  */
	ConnectorBoolInEdge * getSetInputConnector() const { return m_inSet; };

	/** Sets the set input dominant, if true. */
	void setDominant(bool setDominant) { m_setDominant = setDominant; };
	
	/** Returns true, if the set input is dominant. */
	bool getDominant() const { return m_setDominant; };

protected:
	FlipFlopBase(CompContainer * container, const ComponentInfo * ci);

private:
	bool m_setDominant;
	ConnectorBoolOut * m_notOut;
	ConnectorBoolInEdge * m_inSet;
	ConnectorBoolInEdge * m_inReset;

};



//###############################################################
//###############################################################

class FlipFlopBasePropertyGeneralWidget : public Boolean1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	FlipFlopBasePropertyGeneralWidget(FlipFlopBase * comp, QWidget *parent=0, const char *name=0);
//	~FlipFlopBasePropertyGeneralWidget();


	/** Returns the dominant input selector. */
	KSimBooleanBox * getDominantBox() const { return m_dominant; };


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
	KSimBooleanBox * m_dominant;
};

//###############################################################

}; //namespace KSimLibBoolean
#endif
