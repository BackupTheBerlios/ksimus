/***************************************************************************
                          boolean7segment.h  -  description
                             -------------------
    begin                : Wed Dec 26 2001
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

#ifndef BOOLEAN7SEGMENT_H
#define BOOLEAN7SEGMENT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/compviewwidget.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/componentpropertybasewidget.h"

// Forward declaration
class ComponentLayout;
class QComboBox;
class KColorButton;
class QCheckBox;
class QLCDNumber;



namespace KSimLibBoolean
{


extern const ComponentInfo BooleanBoolean7SegmentInfo;


//#######################################################################
//#######################################################################


/**A 7 segment display.
  *@author Rasmus Diekenbrock
  */

class Boolean7Segment : public Component
{
	Q_OBJECT
	
//friend class Boolean7SegmentView;

public:
	Boolean7Segment(CompContainer * container, const ComponentInfo * ci);
//	~Boolean7Segment();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();
	/** Init the property dialog */
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);

	void setForeground(const QColor & color);
	void setBackground(const QColor & color);
	void setFrameEnabled(bool ena);
	const QColor & getForeground() const { return m_foreColor; };
	const QColor & getBackground() const { return m_backColor; };
	bool isFrameEnabed() const { return m_enaFrame; };

  ConnectorBoolIn * getInput1() const { return m_in1; };
  ConnectorBoolIn * getInput2() const { return m_in2; };
  ConnectorBoolIn * getInput4() const { return m_in4; };
  ConnectorBoolIn * getInput8() const { return m_in8; };
	

protected: // Protected attributes
	int m_number;
	QColor m_foreColor;
	QColor m_backColor;
	bool m_enaFrame;
	
	ConnectorBoolIn * m_in1;
	ConnectorBoolIn * m_in2;
	ConnectorBoolIn * m_in4;
	ConnectorBoolIn * m_in8;

signals:
	void signalSetNumber(int newNumber);
	void signalSetForeground(const QColor & color);
	void signalSetBackground(const QColor & color);
	void signalEnableFrame(bool);
};

//#######################################################################
//#######################################################################

class Boolean7SegmentView : public CompViewSize
{
//friend class Boolean7Segment;

	Q_OBJECT

public:	
	Boolean7SegmentView(Boolean7Segment * comp, eViewType viewType);
//	virtual ~Boolean7SegmentView();
	
	/** Draws the Button to a printer */
//	virtual void print(QPainter * paint);
	virtual QWidget * createCompViewWidget(QWidget * parent);

	Boolean7Segment * get7Seg() const { return (Boolean7Segment *)getComponent(); };


protected:
	virtual void resize();

	
private:


signals:
};

//#######################################################################
//#######################################################################

class Boolean7SegmentWidgetView : public CompViewWidget
{
	Q_OBJECT

friend class Boolean7SegmentView;
	
public:
	Boolean7SegmentWidgetView(Boolean7SegmentView * cv, QWidget *parent=0, const char *name=0);
//	~Boolean7SegmentWidgetView();

	Boolean7Segment * get7Seg() const { return (Boolean7Segment *)getCompView()->getComponent(); };

public slots:
	void slotForeground(const QColor & color);
	void slotBackground(const QColor & color);
	void slotEnableFrame(bool enaFrame);

protected:
	QLCDNumber * m_lcd;
};

//#######################################################################
//#######################################################################


class Boolean7SegmentPropertyWidget : public ComponentPropertyBaseWidget
{
	Q_OBJECT

public:
	Boolean7SegmentPropertyWidget(Boolean7Segment * comp, QWidget *parent=0, const char *name=0);
	~Boolean7SegmentPropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();


	Boolean7SegmentView * getView() { return (Boolean7SegmentView*) m_view; };
	Boolean7Segment * get7Seg() { return (Boolean7Segment*) m_view->getComponent(); };

protected:
	CompView * m_view;
	QLCDNumber * m_lcd;
	KColorButton * m_foreGround;
	KColorButton * m_backGround;
	QCheckBox * m_enaFrame;
	QColor m_foreColor;
	QColor m_backColor;
	
protected slots:
	void slotForeground(const QColor & color);
	void slotBackground(const QColor & color);
	void slotEnableFrame(bool enaFrame);

};

//###############################################################

}; //namespace KSimLibBoolean

#endif
