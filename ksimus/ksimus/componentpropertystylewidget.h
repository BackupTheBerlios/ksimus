/***************************************************************************
                          componentpropertystylewidget.h  -  description
                             -------------------
    begin                : Sun Dec 30 2001
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

#ifndef COMPONENTPROPERTYSTYLEWIDGET_H
#define COMPONENTPROPERTYSTYLEWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "componentpropertybasewidget.h"
#include "componentstyle.h"

// Forward declaration
class QVBox;
class QVGroupBox;
class QCheckBox;
class KColorButton;
class QLabel;

/**Style property dialog for ComponentStyle class
  *@author Rasmus Diekenbrock
  */

class ComponentPropertyStyleWidget : public ComponentPropertyBaseWidget
{
   Q_OBJECT
public: 
	ComponentPropertyStyleWidget(ComponentStyle * comp, QWidget *parent=0, const char *name=0);
	~ComponentPropertyStyleWidget();
	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

	/** Returns the widget which is used as layout for this dialog widget.
	  * Add additional items to this widget.
	  */
	QVBox * getVBox() const { return m_vBox; };

	/** Returns the widget which is used as layout for the color properties.
	  * Add additional color items to this widget.
	  */
	QVGroupBox * getColorBox() const { return m_colorGroup; };

	/** Returns the widget which is used as layout for the frame properties.
	  * Add additional frame items to this widget.
	  */
	QVGroupBox * getFrameBox() const { return m_frameGroup; };

	/** Returns the widget which is used as layout for the frame properties.
	  * Add additional frame items to this widget.
	  */
	QVGroupBox * getFontBox() const { return m_fontGroup; };

	ComponentStyle * getCompStyle() { return (ComponentStyle *)getComponent(); };

	
protected slots:
	void slotFontDialog();


protected:
	QVBox * m_vBox;
	
	QVGroupBox * m_colorGroup;
	QCheckBox * m_defaultColors;
	KColorButton * m_foreGround;
	KColorButton * m_backGround;
	QColor m_foreColor;
	QColor m_backColor;
	
	QVGroupBox * m_frameGroup;
	QCheckBox * m_enaFrame;

	QVGroupBox * m_fontGroup;
	QCheckBox * m_defaultFont;
	QPushButton * m_fontButton;
	QFont m_font;
	QLabel * m_exampleFont;
};

#endif
