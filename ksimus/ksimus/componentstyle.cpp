/***************************************************************************
                          componentstyle.cpp  -  description
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

// Include QT Header
#include <qvbox.h>

// Include KDE Header
#include <klocale.h>
#include <kapp.h>

// Include KSimus Header
#include "componentstyle.h"
#include "componentaddon.h"
#include "componentpropertystylewidget.h"
#include "ksimdata.h"
#include "ksimdebug.h"
#include "componentpropertydialog.h"

// Forward declaration

#define FLAG_ENA_FRAME          0x0001
#define FLAG_ENA_FRAME_ADJ      0x0002
#define FLAG_ENA_COLOR_ADJ      0x0020
#define FLAG_ENA_FONT_DEF       0x0100
#define FLAG_ENA_FONT_ADJ       0x0200

const char * sForegroundColor = "Foreground Color";
const char * sBackgroundColor = "Background Color";
const char * sForegroundColorDefault = "Foreground Color Default";
const char * sBackgroundColorDefault = "Background Color Default";
const char * sFrameEnabled    = "Frame Enabled";
const char * sFont            = "Font";



ComponentStyle::ComponentStyle(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_flags(FLAG_ENA_FRAME | FLAG_ENA_FONT_DEF),
		m_foreColor(QColor()),
		m_backColor(QColor()),
		m_defaultForeColor(QColor()),
		m_defaultBackColor(QColor())
{
}

ComponentStyle::~ComponentStyle()
{
}
	
	
void ComponentStyle::save(KSimData & file) const
{
	Component::save(file);
	
	if (getDefaultForegroundColor() != getForegroundColor())
	{
		if(getForegroundColor().isValid())
		{
			file.writeEntry(sForegroundColor, getForegroundColor());
		}
		else
		{
			file.writeEntry(sForegroundColorDefault, true);
		}
	}
	if (getDefaultBackgroundColor() != getBackgroundColor())
	{
		if(getBackgroundColor().isValid())
		{
			file.writeEntry(sBackgroundColor, getBackgroundColor());
		}
		else
		{
			file.writeEntry(sBackgroundColorDefault, true);
		}
	}

	if (!isFrameEnabled())
	{
		file.writeEntry(sFrameEnabled, false);
	}

	if (!isDefaultFontEnabled())
	{
		file.writeEntry(sFont, getFont());
	}
	
}

bool ComponentStyle::load(KSimData & file, bool copyLoad)
{
	if(isColorAdjustmentEnabled())
	{
		if(file.hasKey(sForegroundColorDefault))
		{
			setForegroundColor(QColor());
		}
		else
		{
			setForegroundColor(file.readColorEntry(sForegroundColor, &getDefaultForegroundColor()));
		}
		if(file.hasKey(sBackgroundColorDefault))
		{
			setBackgroundColor(QColor());
		}
		else
		{
			setBackgroundColor(file.readColorEntry(sBackgroundColor, &getDefaultBackgroundColor()));
		}
	}

	if(isFrameAdjustmentEnabled())
	{
		setFrameEnabled(file.readBoolEntry(sFrameEnabled, true));
	}

	QFont defFont(KApplication::font());
	if(file.hasKey(sFont))
	{
		setFont(file.readFontEntry(sFont, &defFont));
		setDefaultFontEnabled(false);
	}
	else
	{
		setFont(defFont);
		setDefaultFontEnabled(true);
	}
	
	return Component::load(file, copyLoad);
}
	
	
bool ComponentStyle::isFrameEnabled() const
{
	return m_flags & FLAG_ENA_FRAME;
}

bool ComponentStyle::isFrameAdjustmentEnabled() const
{
	return m_flags & FLAG_ENA_FRAME_ADJ;
}

void ComponentStyle::setFrameAdjustmentEnabled(bool enable)
{
	if (enable)
	{
		m_flags |= FLAG_ENA_FRAME_ADJ;
	}
	else
	{
		m_flags &= ~FLAG_ENA_FRAME_ADJ;
	}
}
	
bool ComponentStyle::isColorAdjustmentEnabled() const
{
	return m_flags & FLAG_ENA_COLOR_ADJ;
}

void ComponentStyle::setColorAdjustmentEnabled(bool enable)
{
	if (enable)
	{
		m_flags |= FLAG_ENA_COLOR_ADJ;
	}
	else
	{
		m_flags &= ~FLAG_ENA_COLOR_ADJ;
	}
}
	
void ComponentStyle::setDefaultForegroundColor(const QColor & color)
{
	m_defaultForeColor = color;
	setForegroundColor(color);
}

void ComponentStyle::setDefaultBackgroundColor(const QColor & color)
{
	m_defaultBackColor = color;
	setBackgroundColor(color);
}

void ComponentStyle::setDefaultColors(const QColor & fore, const QColor & back)
{
	setDefaultForegroundColor(fore);
	setDefaultBackgroundColor(back);
}


void ComponentStyle::setFrameEnabled(bool enable)
{
	if (enable)
	{
		m_flags |= FLAG_ENA_FRAME;
	}
	else
	{
		m_flags &= ~FLAG_ENA_FRAME;
	}
	emit signalFrameEnabled(enable);
}

void ComponentStyle::setForegroundColor(const QColor & color)
{
	m_foreColor = color;
	signalForegroundColor(color);	
}

void ComponentStyle::setBackgroundColor(const QColor & color)
{
	m_backColor = color;
	signalBackgroundColor(color);	
}

const QColor & ComponentStyle::getForegroundColor() const
{
	return m_foreColor;
}
	
const QColor & ComponentStyle::getBackgroundColor() const
{
	return m_backColor;
}


void ComponentStyle::setFontAdjustmentEnabled(bool enable)
{
	if (enable)
	{
		m_flags |= FLAG_ENA_FONT_ADJ;
	}
	else
	{
		m_flags &= ~FLAG_ENA_FONT_ADJ;
	}
}

bool ComponentStyle::isFontAdjustmentEnabled() const
{
	return m_flags & FLAG_ENA_FONT_ADJ;
}

bool ComponentStyle::isDefaultFontEnabled() const
{
	return m_flags & FLAG_ENA_FONT_DEF;
}

QFont ComponentStyle::getFont() const
{
	if (isDefaultFontEnabled())
	{
		return KApplication::font();
	}
	else
	{	
		return m_font;
	}
}


void ComponentStyle::setFont(const QFont & font)
{
	if (m_font != font)
	{
		m_font = font;
	}
	emit signalFont(getFont());
}
	
void ComponentStyle::setDefaultFontEnabled(bool defFont)
{
	if (defFont)
	{
		m_flags |= FLAG_ENA_FONT_DEF;
	}
	else
	{
		m_flags &= ~FLAG_ENA_FONT_DEF;
	}
	emit signalFont(getFont());
}


void ComponentStyle::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	addGeneralProperty(dialog);
	addConnectorProperty(dialog);
	addInfoProperty(dialog);
	addStyleProperty(dialog);

	if(getAddOnList())
	{
		getAddOnList()->initPropertyDialog(dialog);
	}

}
	
void ComponentStyle::addStyleProperty(ComponentPropertyDialog * dialog)
{
	QVBox * page;
	ComponentPropertyBaseWidget * wid;
	page = dialog->addVBoxPage(i18n("Style"));
	wid = createStyleProperty(this, page);
	dialog->connectSlots(wid);
}

ComponentPropertyBaseWidget * ComponentStyle::createStyleProperty(ComponentStyle * comp, QWidget *parent)
{
	ComponentPropertyBaseWidget * wid = new ComponentPropertyStyleWidget(comp, parent, "Style Settings");
	CHECK_PTR(wid);
	return wid;
}

#include "componentstyle.moc"
