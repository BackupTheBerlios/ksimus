/***************************************************************************
                          ksimgriddialog.h  -  description
                             -------------------
    begin                : Mon Oct 9 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef KSIMGRIDDIALOG_H
#define KSIMGRIDDIALOG_H

#include <kdialogbase.h>


#include "propertywidget.h"

/**Dialog class for KSimGrid
  *@author Rasmus Diekenbrock
  */

class KSimGrid;
class QCheckBox;
class QComboBox;
class KColorButton;
//class KIntegerLine;
class KIntNumInput;


class KSimGridDialogWidget : public PropertyWidget
{
   Q_OBJECT
public: 
	KSimGridDialogWidget(KSimGrid * grid, QWidget *parent=0, const char * name=0);
	virtual ~KSimGridDialogWidget();
	

public slots:
	/** Called if accept button is pressed */
	virtual void slotAccept();
	/** Called if cancel button is pressed */
	virtual void slotCancel();
	/** Called if default button is pressed */
	virtual void slotDefault();

protected:
	KSimGrid * m_gridWork;
	KSimGrid * m_gridStart;
	QCheckBox * enaGrid;
	QCheckBox * enaGlobalGrid;
	QComboBox * styleSel;
	KColorButton * colorButton;
	KIntNumInput * gridXEdit;
	KIntNumInput * gridYEdit;
	void initView();
	void initData();
	

protected slots:
	void slotEnabled(bool on);
	void slotStyleSel(int index);
	void slotColorChanged(const QColor &newColor);
	void slotGlobalEnabled(bool on);
	void slotGridXChanged(int value);
	void slotGridYChanged(int value);
	

};

class KSimGridDialog : public KDialogBase
{
   Q_OBJECT
public:
	KSimGridDialog(KSimGrid * grid, QWidget *parent=0, const char * name=0);
//	virtual ~KSimGridDialog();

};


#endif
