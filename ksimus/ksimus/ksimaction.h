/***************************************************************************
                          ksimaction.h  -  description
                             -------------------
    begin                : Sat Sep 1 2001
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

#ifndef KSIMACTION_H
#define KSIMACTION_H


/**Storage class for simualtion actions
  *@author Rasmus Diekenbrock
  */

class KSimAction {
public: 
	
enum eAction  {	NO 									= 0x0000,		// No actions to do
								CALCULATE 					= 0x0001,		// execute calculation step
								UPDATEVIEW					= 0x0002,		// update view during simulation
								UPDATEOUTPUT	 			= 0x0004,		// execute update output step
								DRAWSHEETVIEW				= 0x0008,		// draws user view
								DRAWUSERVIEW				= 0x0010,		// draws sheet view
								RESET  							= 0x0020,		// execute reset step
								LOAD								= 0x0040,		// execute load step
								SAVE								= 0x0080,		// execute save step
								CHECKCIRCUIT				= 0x0100,		// execute checkCircuit step
								CHECKPROPERTY				= 0x0200,		// execute checkProperty step
								INITPOPUPMENU				= 0x0400,		// inits the popup menu
								INITPROPERTYDIALOG	= 0x0800,		// inits the property dialog
								
								ALL									= ~0L,			// enables all Actions

								// Some useful combinations
								SIMULATION				= CALCULATE | UPDATEOUTPUT | RESET,
								STORAGE						= LOAD | SAVE
							};
	
	
	
	KSimAction();
	KSimAction(eAction action);
	KSimAction(const KSimAction & action);
	~KSimAction();

	/** Set actions. */
	void setAction(eAction newAction)  { m_action = newAction; };
	
	/** Returns the current enabled actions. */
	eAction getAction() const  { return m_action; };

	/** Enables one or more actions. Other actions are not  influenced. */
	void enable(eAction newAction)  { m_action = (eAction)(m_action | newAction); };
	/** Disables one or more actions. Other actions are not  influenced. */
	void disable(eAction newAction) { m_action = (eAction)(m_action & ~newAction); };
	/** Returns true, if the selected action is enabled. If you select more than one action,
	  * the funtion returns true, if one or more of these actions are enabled. */
	bool isEnabled(eAction action) const { return (bool)(m_action & action); };
	
	
	
	/** Returns true, if the calculation step is enabled. */
	bool isCalculateEnabled() const { return isEnabled(CALCULATE); };
	/** Returns true, if the view update during simulation is enabled. */
	bool isUpdateViewEnabled() const { return isEnabled(UPDATEVIEW); };
	/** Returns true, if the update output step is enabled. */
	bool isUpdateOutputEnabled() const { return isEnabled(UPDATEOUTPUT); };
	/** Returns true, if the sheet view drawing is enabled. */
	bool isDrawSheetViewEnabled() const { return isEnabled(DRAWSHEETVIEW); };
	/** Returns true, if the user view drawing is enabled. */
	bool isDrawUserViewEnabled() const { return isEnabled(DRAWUSERVIEW); };
	/** Returns true, if the reset step is enabled. */
	bool isResetEnabled() const { return isEnabled(RESET); };
	/** Returns true, if the load step is enabled. */
	bool isLoadEnabled() const { return isEnabled(LOAD); };
	/** Returns true, if the save step is enabled. */
	bool isSaveEnabled() const { return isEnabled(SAVE); };
	/** Returns true, if the checkCircuit step is enabled. */
	bool isCheckCircuitEnabled() const { return isEnabled(CHECKCIRCUIT); };
	/** Returns true, if the checkProperty step is enabled. */
	bool isCheckPropertyEnabled() const { return isEnabled(CHECKPROPERTY); };
	/** Returns true, if init popup menu is enabled. */
	bool isInitPopupMenuEnabled() const { return isEnabled(INITPOPUPMENU); };
	/** Returns true, if init property dialog is enabled. */
	bool isInitPropertyDialogEnabled() const { return isEnabled(INITPROPERTYDIALOG); };

  /** Filters by component addons unused actions. */
  static KSimAction componentAddOnFilter(KSimAction action);
  /** Filters by component unused actions. */
  static KSimAction componentFilter(KSimAction action);
  /** Filters by component unused actions. */
  static KSimAction connectorFilter(KSimAction action);
	

private:
	eAction m_action;

};

#endif
