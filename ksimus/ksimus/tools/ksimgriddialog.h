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
	

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();
	/** The function cancelPressed() is called, if user wants to cancel the dialog.
	 */
	virtual void cancelPressed();

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
