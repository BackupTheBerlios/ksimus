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

// Include KSimus Header
#include "componentstyle.h"
#include "componentaddon.h"
#include "componentpropertystylewidget.h"
#include "ksimdata.h"
#include "ksimdebug.h"
#include "componentpropertydialog.h"

// Forward declaration

#define FLAG_ENA_FRAME          0x0002
#define FLAG_ENA_FRAME_ADJ      0x0004
#define FLAG_ENA_COLOR          0x0010
#define FLAG_ENA_COLOR_DEF      0x0020
#define FLAG_ENA_COLOR_ADJ      0x0040

#define INIT_FRAME(x)       ((x) ? (FLAG_ENA_FRAME | FLAG_ENA_FRAME_ADJ) : 0)
#define INIT_COLOR(x)       ((x) ? (FLAG_ENA_COLOR | FLAG_ENA_COLOR_ADJ) : FLAG_ENA_COLOR_DEF)

const char * sForegroundColor = "Foreground Color";
const char * sBackgroundColor = "Foreground Color";
const char * sFrameEnabled    = "Frame Enabled";



ComponentStyle::ComponentStyle(bool colorEna, bool frameEna, CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_flags(INIT_COLOR(colorEna) | INIT_FRAME(frameEna)),
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
	
	if(isColorAdjustmentEnabled())
	{
		if (getDefaultForegroundColor() != getForegroundColor())
		{
			file.writeEntry(sForegroundColor, getForegroundColor());
		}

		if (getDefaultBackgroundColor() != getBackgroundColor())
		{
			file.writeEntry(sBackgroundColor, getBackgroundColor());
		}
	}

	if (isFrameAdjustmentEnabled() && !isFrameEnabled())
	{
		file.writeEntry(sFrameEnabled, false);
	}
}

bool ComponentStyle::load(KSimData & file, bool copyLoad)
{
	if(isColorAdjustmentEnabled())
	{
		setForegroundColor(file.readColorEntry(sForegroundColor, &getDefaultForegroundColor()));
		setBackgroundColor(file.readColorEntry(sBackgroundColor, &getDefaultBackgroundColor()));
	}

	if (isFrameAdjustmentEnabled())
	{
		setFrameEnabled(file.readBoolEntry(sFrameEnabled, true));
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
	
bool ComponentStyle::isColorEnabled() const
{
	return m_flags & FLAG_ENA_COLOR;
}

bool ComponentStyle::isDefaultColorEnabled() const
{
	return m_flags & FLAG_ENA_COLOR_DEF;
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

void ComponentStyle::setDefaultColor(const QColor & fore, const QColor & back)
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

void ComponentStyle::setDefaultColorEnabled(bool enable)
{
	if (enable)
	{
		m_flags |= FLAG_ENA_COLOR_DEF;
	}
	else
	{
		m_flags &= ~FLAG_ENA_COLOR_DEF;
	}
	emit signalDefaultColorEnabled(enable);
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
