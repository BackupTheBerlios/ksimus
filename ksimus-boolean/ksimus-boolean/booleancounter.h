/***************************************************************************
                          booleancounter.h  -  description
                             -------------------
    begin                : Wed Dec 19 2001
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

#ifndef BOOLEANCOUNTER_H
#define BOOLEANCOUNTER_H

// C-Includes

// QT-Includes
#include <qspinbox.h>

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentpropertybasewidget.h"
#include "ksimus/ksimbaseuint.h"



// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolInEdge;
class ConnectorBoolOut;
class ConnectorPack;
class ComponentControlBlock;
class ComponentLayout;
class ConnectorLabel;
class KSimBaseUIntEdit;

//###############################################################
//###############################################################
namespace KSimLibBoolean
{

/**a simple boolean counter.
  *@author Rasmus Diekenbrock
  */

class BooleanCounter : public Component
{

	Q_OBJECT

public:
	BooleanCounter(CompContainer * container, const ComponentInfo * ci);
	~BooleanCounter();

	/** save component properties */
	virtual void save(KSimData & file) const;

	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Executes the simulation of this component */
	virtual void calculate();
	/** Shift the result of calculation to output */
	void setOutput() const;
	/** Reset all simulation variables */
	virtual void reset();

	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);
	
	/** Checks the component property. The functions is called after the
	*		property dialog.
	*   Eg. all connectors have unique names.
	*   The implementation checks if up clock and down clock is hidden.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);
	
	ConnectorBoolInEdge * getInputClockUp() const { return m_inClkUp; };
	ConnectorBoolInEdge * getInputClockDown() const { return m_inClkDown; };
	ConnectorBoolInEdge * getInputClear() const { return m_inClear; };
	ConnectorPack * getOutputCounter() const { return m_outCnt; };
	ConnectorBoolOut * getOutputBorrow() const { return m_outBorrow; };
	ConnectorBoolOut * getOutputCarry() const { return m_outCarry; };

	KSimBaseUInt & getMinCount() { return m_minCount; };
	const KSimBaseUInt & getMinCount() const { return m_minCount; };
	KSimBaseUInt & getMaxCount() { return m_maxCount; };
	const KSimBaseUInt & getMaxCount() const { return m_maxCount; };
	KSimBaseUInt & getResetCount() { return m_resetCount; };
	const KSimBaseUInt & getResetCount() const { return m_resetCount; };
	unsigned int getBits() const;
	void setBits(unsigned int bits);
	unsigned int getMinBits() const;

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();


protected:
	ConnectorBoolInEdge * m_inClear;
	ConnectorBoolInEdge * m_inClkUp;
	ConnectorBoolInEdge * m_inClkDown;
	ConnectorPack * m_outCnt;
	ConnectorBoolOut * m_outBorrow;
	ConnectorBoolOut * m_outCarry;
	unsigned int m_cnt;
	bool m_borrow;
	bool m_carry;

	KSimBaseUInt m_maxCount;
	KSimBaseUInt m_minCount;
	KSimBaseUInt m_resetCount;
		
};

//###############################################################
//###############################################################

class BooleanCounterView : public CompView
{

	Q_OBJECT

public:
	BooleanCounterView(BooleanCounter * comp, eViewType viewType);
//	~BooleanCounterView();
	virtual void draw(QPainter * p);
	
	BooleanCounter * getCounter() const { return (BooleanCounter*) getComponent(); };
	
	ComponentControlBlock * getControlBlock() const { return m_ctrlBlock; };
	ComponentLayout * getComponentLayout() const { return m_layout; };


private:
	ComponentControlBlock * m_ctrlBlock;
	ComponentLayout * m_layout; 	

private slots:
	void addConn(ConnectorBase * conn);

};

//###############################################################
//###############################################################

class BooleanCounterPropertyWidget : public ComponentPropertyBaseWidget
{
	Q_OBJECT

public:
	BooleanCounterPropertyWidget(BooleanCounter * comp, QWidget *parent=0, const char *name=0);
	~BooleanCounterPropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

	BooleanCounter * getCounter() { return (BooleanCounter *)getComponent();	};


	QSpinBox * getBitsWidget() { return m_bits; };
	KSimBaseUIntEdit * getMaxValueWidget() { return m_maxValue; };
	KSimBaseUIntEdit * getMinValueWidget() { return m_minValue; };
	KSimBaseUIntEdit * getResetValueWidget() { return m_resetValue; };


protected:
	KSimBaseUIntEdit * m_maxValue;
	KSimBaseUIntEdit * m_minValue;
	KSimBaseUIntEdit * m_resetValue;
	QSpinBox * m_bits;
	

private:
	void limit();
	bool m_noBitChanges;

private slots:
	void slotMinValueChanged();
	void slotMaxValueChanged();
	void slotBitsChanged();
	void slotResetValueChanged();

};

//###############################################################

}; //namespace KSimLibBoolean

#endif
