/***************************************************************************
                          componentaddon.h  -  description
                             -------------------
    begin                : Thu May 17 2001
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

#ifndef COMPONENTADDON_H
#define COMPONENTADDON_H

// C-Includes

// QT-Includes
#include <qobject.h>
#include <qlist.h>

// KDE-Includes

// Project-Includes
#include "componentitem.h"
#include "ksimaction.h"

// Forward declaration
class Component;
class KSimData;
class QStringList;
class ComponentPropertyDialog;
class QPopupMenu;
class QPainter;


/**A virtual base class for component add on's
  *@author Rasmus Diekenbrock
  */

class ComponentAddOn : public QObject, public ComponentItem
{
   Q_OBJECT
public: 
	virtual ~ComponentAddOn();

	/** Save component add on's properties.
		* The default implementation does nothing.
		*/
	virtual void save(KSimData & file) const;
	/** Load component add on's properties.
		*	Returns true if successful.
		* The default implementation does nothing.
		*/
	virtual bool load(KSimData & file);
	/** Draw the sheet view.
		* The default implementation does nothing.
		*/
	virtual void drawSheetView (QPainter *p) const;
	/** Draw the user view.
		* The default implementation does nothing.
		*/
	virtual void drawUserView (QPainter *p) const;
	
	/** Checks the component add on before simulation.
		* Returns the number of errors.
		* The default implementation does nothing.
		*/
	virtual int checkCircuit();
	/** Checks the component property. The functions is called after the
		*	property dialog.
		* The default implementation does nothing.
		*
		*   Append the message to errorMsg.
		*/
	virtual void checkProperty(QStringList & errorMsg);
	/** Executes the simulation of this component.
		* The default implementation does nothing.
		*/
	virtual void calculate();
	/** Shift the result of calculation to output.
		* The default implementation does nothing.
		*/
	virtual void updateOutput();
	/** Reset all simulation variables.
		* The default implementation does nothing.
		*/
	virtual void reset();
	/** Init the property dialog.
		* The default implementation does nothing.
		*/
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);
	/** Initialize the component popup menu.
	  *	Return true, if items are added.
		* The default implementation does nothing.
		*/
	virtual bool initPopupMenu(QPopupMenu * popup);

  /** Returns the actions object. In this object describes all of this addon required actions. */
	KSimAction & getAction() { return m_myActions; };

  /** Returns the name of the add on. */
  const QString & getName() const { return m_addOnName; };
	
protected:
	/** Constructs a new add on.
	  *
	  * @param component The component to which the addon is added.
	  * @param addOnName The name of the add on. The name have to be unique within a component.
	  */
	ComponentAddOn(Component * component, const QString & addOnName);

private:
	KSimAction m_myActions;
	QString m_addOnName;
};


class ComponentAddOnList : public QList<ComponentAddOn>
{
public:
	/** Constructs a new Add On list.
		*
		* Enables the auto delete function.
		*/
	ComponentAddOnList();
	/** Destructor. Deletes all add on.
		*/
	~ComponentAddOnList();
	
	/** Save all component add on's properties.
		*/
	void save(KSimData & file) const;
	/** Load all component add on's properties.
		*	Returns true if successful.
		* This function takes care about the KSimAction information.
		*/
	bool load(KSimData & file);
	/** Draw the sheet view.
		* This function takes care about the KSimAction information.
		*/
	void drawSheetView (QPainter *p) const;
	/** Draw the user view.
		* This function takes care about the KSimAction information.
		*/
	void drawUserView (QPainter *p) const;
	/** Checks all component add on before simulation.
		* Returns the number of errors.
		* This function takes care about the KSimAction information.
		*/
	int checkCircuit();
	/** Checks  all component property. The functions is called after the
		*	property dialog.
		* This function takes care about the KSimAction information.
		*/
	void checkProperty(QStringList & errorMsg);
	/** Executes the simulation of this component.
		* This function takes care about the KSimAction information.
		*/
	void calculate();
	/** Shift the result of calculation to output.
		* This function takes care about the KSimAction information.
		*/
	void updateOutput();
	/** Reset all simulation variables.
		* This function takes care about the KSimAction information.
		*/
	void reset();
	/** Init the property dialog.
		* This function takes care about the KSimAction information.
		*/
	void initPropertyDialog(ComponentPropertyDialog * dialog);
	/** Initialize the component popup menu.
	  *	Return true, if items are added.
		* This function takes care about the KSimAction information.
		*/
	bool initPopupMenu(QPopupMenu * popup);

  /** Returns all actions of the addons. */
	KSimAction getAction() const;
};



#define FOR_EACH_COMPONENT_ADDON(_it_,_addOnList_)	\
		for(QListIterator<ComponentAddOn> _it_(_addOnList_);_it_.current();++_it_)


#endif
