/***************************************************************************
                          integercounter.h  -  description
                             -------------------
    begin                : Sun Aug 17 2003
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

#ifndef INTEGERCOUNTER_H
#define INTEGERCOUNTER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertybasewidget.h"
#include "ksimus/ksimbaseint.h"
#include "ksimus/componentlayout.h"



// Forward declaration
class KSimSpinBox;
class QCheckBox;
class ConnectorBoolIn;
class ConnectorBoolInEdge;
class ConnectorBoolOut;
class ConnectorIntegerIn;
class ConnectorIntegerOut;
class ConnectorPack;
class ConnectorLabel;
class KSimBaseIntEdit;
//class KSimBaseUIntEdit;

//###############################################################
//###############################################################
namespace KSimLibInteger
{

/**Base class of integer counter.
  *@author Rasmus Diekenbrock
  */

class IntegerCounterBase : public Component
{
public:
	~IntegerCounterBase();

	/** save component properties */
	virtual void save(KSimData & file) const;

	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

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
	
	ConnectorBoolInEdge * getInputClear() const { return m_inClear; };
	ConnectorIntegerOut * getOutput() const { return m_output; };
	ConnectorBoolOut * getOutputMin() const { return m_outputMin; };
	ConnectorBoolOut * getOutputMax() const { return m_outputMax; };

	KSimBaseInt & getDefaultMinCount() { return m_defaultMinCount; };
	const KSimBaseInt & getDefaultMinCount() const { return m_defaultMinCount; };
	KSimBaseInt & getDefaultMaxCount() { return m_defaultMaxCount; };
	const KSimBaseInt & getDefaultMaxCount() const { return m_defaultMaxCount; };
	KSimBaseInt & getDefaultResetCount() { return m_defaultResetCount; };
	const KSimBaseInt & getDefaultResetCount() const { return m_defaultResetCount; };

	KSimBaseInt & getMinCount() { return m_minCount; };
	const KSimBaseInt & getMinCount() const { return m_minCount; };
	KSimBaseInt & getMaxCount() { return m_maxCount; };
	const KSimBaseInt & getMaxCount() const { return m_maxCount; };
	KSimBaseInt & getResetCount() { return m_resetCount; };
	const KSimBaseInt & getResetCount() const { return m_resetCount; };

	void setWrappingEnabled(bool ena) { m_wrapping = ena; };
	bool isWrappingEnabled() const { return m_wrapping; };


protected:
	IntegerCounterBase(CompContainer * container, const ComponentInfo * ci);
	ConnectorBoolInEdge * m_inClear;
	ConnectorIntegerOut * m_output;
	ConnectorBoolOut * m_outputMin;
	ConnectorBoolOut * m_outputMax;
	int m_cnt;
	bool m_wrapping;

	KSimBaseInt m_defaultMaxCount;
	KSimBaseInt m_defaultMinCount;
	KSimBaseInt m_defaultResetCount;

	KSimBaseInt m_maxCount;
	KSimBaseInt m_minCount;
	KSimBaseInt m_resetCount;

};

//###############################################################
//###############################################################

class IntegerCounterBaseView : public CompView
{
public:
	IntegerCounterBaseView(IntegerCounterBase * comp, eViewType viewType, const char * name);
//	~IntegerCounterView();
	
	IntegerCounterBase * getCounterBase() const { return (IntegerCounterBase*) getComponent(); };
	
	ComponentLayoutVerticalCtrl * getComponentLayout() const { return m_layout; };


private:
	ComponentLayoutVerticalCtrl * m_layout;
};

//###############################################################
//###############################################################

class IntegerCounterBasePropertyWidget : public ComponentPropertyBaseWidget
{
public:
	IntegerCounterBasePropertyWidget(IntegerCounterBase * comp, QWidget *parent=0, const char *name=0);
	~IntegerCounterBasePropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerCounterBase * getCounterBase() { return (IntegerCounterBase *)getComponent();	};


	KSimBaseIntEdit * getMaxValueWidget() { return m_maxValue; };
	KSimBaseIntEdit * getMinValueWidget() { return m_minValue; };
	KSimBaseIntEdit * getResetValueWidget() { return m_resetValue; };
	QCheckBox * getWrappingWidget() { return m_wrapping; };


protected:
	KSimBaseIntEdit * m_maxValue;
	KSimBaseIntEdit * m_minValue;
	KSimBaseIntEdit * m_resetValue;
	QCheckBox * m_wrapping;
};

//###############################################################
//###############################################################


/**Integer counter with clock up and clock down inputs.
  *@author Rasmus Diekenbrock
  */

class IntegerCounterClkUpClkDown : public IntegerCounterBase
{
public:
	IntegerCounterClkUpClkDown(CompContainer * container, const ComponentInfo * ci);
	~IntegerCounterClkUpClkDown();

	/** Executes the simulation of this component */
	virtual void calculate();

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

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();


protected:
	ConnectorBoolInEdge * m_inClkUp;
	ConnectorBoolInEdge * m_inClkDown;
};


//###############################################################
//###############################################################

class IntegerCounterClkUpClkDownView : public IntegerCounterBaseView
{
public:
	IntegerCounterClkUpClkDownView(IntegerCounterClkUpClkDown * comp, eViewType viewType, const char * name);
//	~IntegerCounterClkUpClkDownView();

	IntegerCounterClkUpClkDown * getCounterClkUpClkDown() const { return (IntegerCounterClkUpClkDown*) getComponent(); };

private:
};


//###############################################################
//###############################################################


/**Integer counter with clock up, clock down and load inputs.
  *@author Rasmus Diekenbrock
  */

class IntegerCounterClkUpClkDownLoad : public IntegerCounterClkUpClkDown
{
public:
	IntegerCounterClkUpClkDownLoad(CompContainer * container, const ComponentInfo * ci);
	~IntegerCounterClkUpClkDownLoad();

	/** Executes the simulation of this component */
	virtual void calculate();

	ConnectorBoolInEdge * getInputLoadEnable() const { return m_inLoadEnable; };
	ConnectorIntegerIn * getInputCounter() const { return m_inCounter; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();


protected:
	ConnectorBoolInEdge * m_inLoadEnable;
	ConnectorIntegerIn * m_inCounter;
};


//###############################################################
//###############################################################

class IntegerCounterClkUpClkDownLoadView : public IntegerCounterClkUpClkDownView
{
public:
	IntegerCounterClkUpClkDownLoadView(IntegerCounterClkUpClkDownLoad * comp, eViewType viewType, const char * name);
//	~IntegerCounterClkUpClkDownLoadView();

	IntegerCounterClkUpClkDownLoad * getCounterClkUpClkDownLoad() const { return (IntegerCounterClkUpClkDownLoad*) getComponent(); };

private:
};


//###############################################################
//###############################################################


/**Integer counter with clock and direction inputs.
  *@author Rasmus Diekenbrock
  */

class IntegerCounterClkDir : public IntegerCounterBase
{
public:
	IntegerCounterClkDir(CompContainer * container, const ComponentInfo * ci);
	~IntegerCounterClkDir();

	/** Executes the simulation of this component */
	virtual void calculate();

	ConnectorBoolIn * getInputDir() const { return m_inDir; };
	ConnectorBoolInEdge * getInputClock() const { return m_inClk; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();


protected:
	ConnectorBoolIn * m_inDir;
	ConnectorBoolInEdge * m_inClk;
};


//###############################################################
//###############################################################

class IntegerCounterClkDirView : public IntegerCounterBaseView
{
public:
	IntegerCounterClkDirView(IntegerCounterClkDir * comp, eViewType viewType, const char * name);
//	~IntegerCounterClkDirView();

	IntegerCounterClkDir * getCounterClkDir() const { return (IntegerCounterClkDir*) getComponent(); };

private:
};

//###############################################################
//###############################################################


/**Integer counter with clock up, direction and load inputs.
  *@author Rasmus Diekenbrock
  */

class IntegerCounterClkDirLoad : public IntegerCounterClkDir
{
public:
	IntegerCounterClkDirLoad(CompContainer * container, const ComponentInfo * ci);
	~IntegerCounterClkDirLoad();

	/** Executes the simulation of this component */
	virtual void calculate();

	ConnectorBoolInEdge * getInputLoadEnable() const { return m_inLoadEnable; };
	ConnectorIntegerIn * getInputCounter() const { return m_inCounter; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();


protected:
	ConnectorBoolInEdge * m_inLoadEnable;
	ConnectorIntegerIn * m_inCounter;
};


//###############################################################
//###############################################################

class IntegerCounterClkDirLoadView : public IntegerCounterClkDirView
{
public:
	IntegerCounterClkDirLoadView(IntegerCounterClkDirLoad * comp, eViewType viewType, const char * name);
//	~IntegerCounterClkDirLoadView();

	IntegerCounterClkDirLoad * getCounterClkDirLoad() const { return (IntegerCounterClkDirLoad*) getComponent(); };

private:
};


}; //namespace KSimLibInteger

#endif
