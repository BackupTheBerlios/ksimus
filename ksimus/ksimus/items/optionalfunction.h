/***************************************************************************
                          optionalfunction.h  -  description
                             -------------------
    begin                : Sun Jun 2 2002
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

#ifndef OPTIONALFUNCTION_H
#define OPTIONALFUNCTION_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "componentaddon.h"

// Forward declaration
class QLabel;
class QComboBox;
class PropertyWidget;


/**Base class for optinal functions.
  *@author Rasmus Diekenbrock
  */

class OptionalFunction : public ComponentAddOn
{
class Private;

   Q_OBJECT
public:
	/** Constructs a new optional function.
	  *
	  * @param component The component to which the optional function is added.
	  * @param addOnName The name of the optional function.
	  * @param unique    True if the name have to be unique within a component.
	  */
	OptionalFunction(Component * component, const QString & addOnName, bool unique = true);
	
	~OptionalFunction();
	
	/** Save component add on's properties.
		* The default implementation does nothing.
		*/
	virtual void save(KSimData & file) const;
	/** Load component add on's properties.
		* Returns true if successful.
		* The implementation saves the enable state.
		*/
	virtual bool load(KSimData & file);
	/** Init the property dialog.
		* The implementation loads the enable state.
		*/
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);
	
	/** Enables the optional function. */
	virtual void setEnabled(bool ena);
	/** Returns true, if the optional function is enabled. */
	bool isEnabled() const;

	/** Set the default value the optional function. Uses in property widget.
	  * Set also the enable state. */
	void setDefaultEnabled(bool ena);
	/** Returns true, if the optional function is default enabled. */
	bool isDefaultEnabled() const;
	
	/** Sets the optional function in changeable state. This means only the QComboBox is
	    enabled. The default is changeable. */
	void setChangeable(bool changeable);
	/** Returns true, if the optional function is changeable. */
	bool isChangeable() const;

	
	/** Adds an entry to a property widget.
	  * @param widget     The @ref PropertyWidget to which the entry is added.
	  * */
	virtual void addToPropertyWidget(PropertyWidget * widget);

	/** Returns the @ref QLabel of the current @ref PropertyWidget.
	  * The function returns a null pointer if no PropertyWidget exists. */
	QLabel * getLabel() const;
	/** Returns the @ref QCheckBox of the current @ref PropertyWidget.
	  * The function returns a null pointer if no PropertyWidget exists. */
	QComboBox * getComboBox() const;
	/** Returns a pointer to the current @ref PropertyWidget.
	  * The function returns a null pointer if no PropertyWidget exists. */
	PropertyWidget * getPropertyWidget() const;

protected:

private slots:
	void slotAccept();
	void slotDefault();
	void slotPropertyWidgetDestroyed();

private:
	Private * m_p;
};

#endif
