/***************************************************************************
                          widgetcontrolbutton.h  -  description
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

#ifndef WIDGETCONTROLBUTTON_H
#define WIDGETCONTROLBUTTON_H

// C-Includes

// QT-Includes
#include <qhbox.h>

// KDE-Includes

// KSimus-Includes

// Project-Includes

// Forward declaration
class QPushButton;

namespace KSimLibDataRecorder
{

// Forward declaration (namespace)
class KSimColorLabel;

/**A button which raises or hides widgets
  *@author Rasmus Diekenbrock
  */

class WidgetControlButton : public QHBox
{
   Q_OBJECT
public: 
	WidgetControlButton(QWidget * controlledWidget, QWidget *parent=0, const char *name=0);
	~WidgetControlButton();
	
	
	/** Deletes the button, if the widget will be delete. */
	void setAutoDelete(bool enable);
	/** Returns true, if the button will be automatically deleted. */
	bool getAutoDelete() const;
	
	/** Returns the controlled widget. */
	QWidget * getControlledWidget() const;

	/** Detect show/hide events. */
	virtual bool eventFilter(QObject *, QEvent *);
	
public slots:
	void setText(const QString & text);
	void setColor(const QColor & color);

protected:
	virtual void hideEvent(QHideEvent *);

private:
	QWidget * m_controlledWidget;
	QPushButton * m_button;
	KSimColorLabel * m_color;
	bool m_autoDelete;
	/** True, if controlled widget is hidden. */
	bool m_widIsHidden;
	QSize m_widSize;
	QPoint m_widPos;

private slots:
	void slotDestroy(void);
	void slotChanged(bool state);
};


class KSimColorLabel : public QFrame
{
Q_OBJECT

public:
	KSimColorLabel(QWidget * parent = 0, const char * name = 0);
	
	const QColor & getColor() const;

public slots:
	void setColor(const QColor & color);
};


};  //namespace KSimLibDataRecorder

#endif
