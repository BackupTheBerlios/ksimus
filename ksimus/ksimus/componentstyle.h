/***************************************************************************
                          componentstyle.h  -  description
                             -------------------
    begin                : Sat Dec 29 2001
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

#ifndef COMPONENTSTYLE_H
#define COMPONENTSTYLE_H

// Include QT Header
#include <qcolor.h>
#include <qfont.h>

// Include KDE Header

// Include KSimus Header
#include "component.h"

// Forward declaration


/** A component with additional style properties.
  * This compnent is useful if it uses also a @ref CompViewHBox or @ref CompViewVBox.
  *@author Rasmus Diekenbrock
  */

class ComponentStyle : public Component
{
	Q_OBJECT

public:
	
	/** Constructs a component with additional style properties.
	  * @param container  The container which is the parent.
	  * @param ci         Component info of this component
	  */
	ComponentStyle(CompContainer * container, const ComponentInfo * ci);
	/** The destuctor */
	~ComponentStyle();
	
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	
	
	/** Init the property dialog */
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);
	
	/** Add the style property page to the property dialog
		This function is called by @ref initPropertyDialog.*/
	void addStyleProperty(ComponentPropertyDialog * dialog);
	
	/** Creates the style property page for the property dialog.
	  * Overload this function if you want to use a modified Style Propery Page. Use as base class
	  * @ref ComponentPropertyStyleWidget. */
	virtual ComponentPropertyBaseWidget * createStyleProperty(ComponentStyle * comp, QWidget *parent);

	
	
	//##########   COLOR    ############

	/** Sets the adjustment of the color property. True allows the adjustment, false hides the
	  * adjustment. The default depends on the constructors parameter. */
	void setColorAdjustmentEnabled(bool enable);
	
	/** Returns true, if the adjustment of the color property is allowed. The default depends
	  * on the constructors parameter. */
	bool isColorAdjustmentEnabled() const;
	
	//##########   COLOR    ---   Default colors

	/** Returns true, if the component have to use the default colors. */
	bool isDefaultColorEnabled() const;
	
	
	//##########   COLORs

	/** Returns the foreground color. The default is a invalid color.*/
	const QColor & getForegroundColor() const;
	
	/** Returns the background color. The default is a invalid color. */
	const QColor & getBackgroundColor() const;
	
	
	
	/** Sets the default foreground color. The default is a invalid color
	  * Sets also the foreground color. */
	void setDefaultForegroundColor(const QColor & color);
	
	/** Returns the default foreground color. The default is a invalid color.*/
	const QColor & getDefaultForegroundColor() const { return m_defaultForeColor; };
	
	
	/** Sets the default background color. The default is a invalid color.
	  * Sets also the background color. */
	void setDefaultBackgroundColor(const QColor & color);
	
	/** Returns the default background color. The default is a invalid color. */
	const QColor & getDefaultBackgroundColor() const { return m_defaultBackColor; };
	
	/** Sets the default colors. The defaults are a invalid colors.
	  * Sets also the foreground and background colors. */
	void setDefaultColors(const QColor & fore, const QColor & back);

	
	
	//##########   FRAME    ############
	
	/** Sets the adjustment of the frame property. True allows the adjustment, false hides the
	  * adjustment. The default is off. */
	void setFrameAdjustmentEnabled(bool enable);
	
	/** Returns true, if the adjustment of the frame property is allowed. The default is off. */
	bool isFrameAdjustmentEnabled() const;
	
	/** Returns true, if the component have to be displayed a frame (user view only). The default is true. */
	bool isFrameEnabled() const;
	
	//##########   FONT    ############
	
	/** Sets the adjustment of the font property. True allows the adjustment, false hides the
	  * adjustment. The default is off. */
	void setFontAdjustmentEnabled(bool enable);
	
	/** Returns true, if the adjustment of the font property is allowed. The default is off. */
	bool isFontAdjustmentEnabled() const;
	
	/** Returns true, if the component have to use the default font. */
	bool isDefaultFontEnabled() const;
	
	/** Returns the selected font. */
	QFont getFont() const;
	
	
	
	
	
	
public slots:
	//##########   COLOR    ############
	
	/** The component uses the default colors if true or the special colors if false. */
	void setDefaultColorEnabled(bool enable);
	/** Sets the foreground color. */
	void setForegroundColor(const QColor & color);
	/** Sets the background color. */
	void setBackgroundColor(const QColor & color);

	
	//##########   FRAME    ############
	
	/** Enables the frame of the component view (user view only). The default is true. */
	void setFrameEnabled(bool enable);
	
	//##########   FONT    ############
	
	/** Sets the font. */
	void setFont(const QFont & font);
	
	/** Sets the font. */
	void setDefaultFontEnabled(bool defFont);
	

signals:
	/** Enables the frame of the component view (user view only). */
	void signalFrameEnabled(bool enable);
	/** Sets the foreground color. */
	void signalForegroundColor(const QColor & color);
	/** Sets the background color. */
	void signalBackgroundColor(const QColor & color);
	/** The component uses the default colors if true or the special colors if false. */
	void signalDefaultColorEnabled(bool enable);
	/** Font has changed. */
	void signalFont(const QFont & font);

private:
	Q_UINT32 m_flags;
	QColor m_foreColor;
	QColor m_backColor;
	QColor m_defaultForeColor;
	QColor m_defaultBackColor;
	QFont m_font;
	
};

#endif
