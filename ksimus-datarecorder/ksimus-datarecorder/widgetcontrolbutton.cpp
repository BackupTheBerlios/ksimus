/***************************************************************************
                          widgetcontrolbutton.cpp  -  description
                             -------------------
    begin                : Wed Aug 29 2001
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

// C-Includes

// QT-Includes
#include <qpushbutton.h>


// KDE-Includes

// KSimus-Includes
#include "ksimus/ksimdebug.h"

// Project-Includes
#include "widgetcontrolbutton.h"

// Forward declaration

namespace KSimLibDataRecorder
{

WidgetControlButton::WidgetControlButton(QWidget * controlledWidget, QWidget *parent, const char *name )
	: QHBox(parent,name),
		m_controlledWidget(controlledWidget),
		m_autoDelete(false),
		m_widIsHidden(controlledWidget->isHidden())
{
	
	setSpacing(3);
	m_color = new KSimColorLabel(this);
	Q_CHECK_PTR(m_color);
	m_color->setFixedSize(8,20);
	m_button = new QPushButton(this);
	Q_CHECK_PTR(m_button);
	
	m_button->setToggleButton(true);
	connect(m_button, SIGNAL(toggled(bool)), this, SLOT(slotChanged(bool)));
	setAutoDelete(true);
	m_controlledWidget->installEventFilter(this);
}

WidgetControlButton::~WidgetControlButton()
{
}

/** Deletes the button, if the widget will be delete. */
void WidgetControlButton::setAutoDelete(bool enable)
{
	m_autoDelete = enable;
	if (m_autoDelete)
	{
		connect(getControlledWidget(), SIGNAL(destroyed()), this, SLOT(slotDestroy()));
	}
	else
	{
		disconnect(getControlledWidget(), SIGNAL(destroyed()), this, SLOT(slotDestroy()));
	}
}

/** Returns true, if the button will be automatically deleted. */
bool WidgetControlButton::getAutoDelete() const
{
	return m_autoDelete;
}
	
/** Returns the controlled widget. */
QWidget * WidgetControlButton::getControlledWidget() const
{
	return m_controlledWidget;
}

void WidgetControlButton::slotChanged(bool state)
{
	if (state)
	{
		getControlledWidget()->show();
		getControlledWidget()->raise();
	}
	else
	{
		getControlledWidget()->hide();
	}
}

void WidgetControlButton::slotDestroy()
{
	delete this;
}

void WidgetControlButton::setText(const QString & text)
{
	m_button->setText(text);
}

void WidgetControlButton::setColor(const QColor & color)
{
	m_color->setColor(color);
}


bool WidgetControlButton::eventFilter(QObject * obj, QEvent * ev)
{
	if (obj == (QObject*)getControlledWidget())
	{
		switch(ev->type())
		{
			case QEvent::Show:
				if(m_widIsHidden)
				{
					m_widIsHidden =false;
					m_button->setOn(true);
					if (m_widSize.isValid())
					{
						getControlledWidget()->resize(m_widSize);
						getControlledWidget()->move(m_widPos);
					}
				}
				else
				{
					KSIMDEBUG("Show event dont match m_hidden");
				}
				break;
				
			case QEvent::Hide:
				if(!m_widIsHidden)
				{
					m_widSize = getControlledWidget()->size();
					m_widPos = getControlledWidget()->pos();
					m_widIsHidden = true;
					m_button->setOn(false);
				}
				else
				{
					KSIMDEBUG("Show event dont match m_hidden");
				}
				break;
				
    	default:
    		break;
    }
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Object", obj);
	}
	return false;
}

void WidgetControlButton::hideEvent(QHideEvent *)
{
	// Hide also the conrotlling widget
	getControlledWidget()->hide();
}

//#####################################################################################

KSimColorLabel::KSimColorLabel(QWidget * parent, const char * name)
	:	QFrame(parent,name)
{
	setFrameStyle(QFrame::Panel | QFrame::Sunken);
	setBackgroundColor(green);
}

void KSimColorLabel::setColor(const QColor & color)
{
	setBackgroundColor(color);
}

const QColor & KSimColorLabel::getColor() const
{
	return backgroundColor();
}


};  //namespace KSimLibDataRecorder

#include "widgetcontrolbutton.moc"
