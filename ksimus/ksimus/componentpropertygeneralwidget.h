/***************************************************************************
                          componentpropertygeneralwidget.h  -  description
                             -------------------
    begin                : Sat Feb 10 2001
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

#ifndef COMPONENTPROPERTYGENERALWIDGET_H
#define COMPONENTPROPERTYGENERALWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "componentpropertybasewidget.h"

// Forward declaration
class QString;
class QStringList;

/**General properties
  *@author Rasmus Diekenbrock
  */


class ComponentPropertyGeneralWidget : public ComponentPropertyBaseWidget
{
	Q_OBJECT

public:
	ComponentPropertyGeneralWidget(Component * comp, QWidget *parent=0, const char *name=0);
	~ComponentPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	void addNameSuggestion(const QString & suggName);
	void addNameSuggestion(const QStringList & suggNameList);

protected:

private:
	class Private;
	Private * m_p;

private slots:
	void slotOpenSuggestionName();
	void slotSuggestionName(int id);
	void slotClearName();
	
};

#endif
