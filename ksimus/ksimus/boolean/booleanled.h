/***************************************************************************
                          booleanled.h  -  description
                             -------------------
    begin                : Sat Feb 17 2001
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

#ifndef BOOLEANLED_H
#define BOOLEANLED_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"
#include "compviewwidget.h"
#include "componentpropertybasewidget.h"

// Forward declaration
class QColor;
class QRadioButton;
class KLed;
class ConnectorBoolIn;


/**A LED
  *@author Rasmus Diekenbrock
  */

extern const ComponentInfo BooleanLedInfo;

class BooleanLed : public Component
{
friend class BooleanLedView;

	Q_OBJECT

public:
	BooleanLed(CompContainer * container, const ComponentInfo * ci);
	~BooleanLed();

	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();
	/** Init the property dialog */
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	void setColor(QColor & color);
	const QColor & getColor() const;

protected: // Protected attributes
	bool m_onState;
	QColor * m_color;
	ConnectorBoolIn * m_inConn;

signals:
	void signalColor(const QColor & color);
};

class BooleanLedView : public CompViewSize
{
friend class BooleanLed;

	Q_OBJECT

public:	
	BooleanLedView(Component * comp, eViewType viewType);
	virtual ~BooleanLedView();
	
	/** Draws the Button to a printer */
//	virtual void print(QPainter * paint);
	virtual QWidget * createCompViewWidget(QWidget * parent);

	/** Draw the component view */
	virtual void draw (QPainter *);

protected:
	virtual void resize();

signals:
	void signalState(bool on);
};

class BooleanLedWidgetView : public CompViewWidget
{
	Q_OBJECT

friend class BooleanLedView;
	
public:
	BooleanLedWidgetView(CompView * cv, QWidget *parent=0, const char *name=0);
	~BooleanLedWidgetView();
	virtual void paintEvent(QPaintEvent * pe);

public slots:
	void slotState(bool on);
	void slotColor(const QColor & color);

protected:
	KLed * m_led;
};

//###############################################################
//###############################################################

class BooleanLedPropertyWidget : public ComponentPropertyBaseWidget
{
	Q_OBJECT

public:
	BooleanLedPropertyWidget(Component * comp, QWidget *parent=0, const char *name=0);
	~BooleanLedPropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

protected slots:
	void slotRed();
	void slotGreen();
	void slotOrange();
	
protected:
	QRadioButton * m_red;
	QRadioButton * m_green;
	QRadioButton * m_orange;
	QColor * m_newColor;
};





#endif
