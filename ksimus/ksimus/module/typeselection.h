/***************************************************************************
                          typeselection.h  -  description
                             -------------------
    begin                : Sun Apr 22 2001
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

#ifndef TYPESELECTION_H
#define TYPESELECTION_H

// C-Includes

// QT-Includes
#include <qvbuttongroup.h>

// KDE-Includes

// Project-Includes
#include "moduledata.h"

// Forward declaration
class QRadioButton;
class ModuleDialog;

/**Module view type selector
  *@author Rasmus Diekenbrock
  */

class TypeSelection : public QVButtonGroup
{
   Q_OBJECT

public:	
	TypeSelection(ModuleDialog * dialog, QWidget * parent, const char * name = 0);
	~TypeSelection();

public slots:	
	void setup();
	
private:
  ModuleDialog * m_dialog;
	QRadioButton * m_rbNoView;
	QRadioButton * m_rbGeneric;
	QRadioButton * m_rbUserView;
	QRadioButton * m_rbPixmap;

private slots:
	void slotClicked(int);

signals:
	void clicked(ModuleViewType);
};


#endif
