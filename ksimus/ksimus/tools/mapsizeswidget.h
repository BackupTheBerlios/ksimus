/***************************************************************************
                          mapsizeswidget.h  -  description
                             -------------------
    begin                : Sun Jan 6 2002
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

#ifndef MAPSIZESWIDGET_H
#define MAPSIZESWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"

// Forward declaration
class KSimusDoc;
class QSpinBox;

/**Adjusts the map sizes.
  *@author Rasmus Diekenbrock
  */

class MapSizesWidget : public PropertyWidget
{

   Q_OBJECT

public:
	MapSizesWidget(KSimusDoc * doc, QWidget *parent=0, const char *name=0);
//	~MapSizesWidget();

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();


protected:

private:
	KSimusDoc * m_doc;
	
	QSpinBox * m_schematicWidth;
	QSpinBox * m_schematicHeight;
	QSpinBox * m_userWidth;
	QSpinBox * m_userHeight;
	
	
private slots:
	/** Stores data for undo mechanism */
	void slotUndo();
	
};

#endif
