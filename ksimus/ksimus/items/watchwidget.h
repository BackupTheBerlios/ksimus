/***************************************************************************
                          watchwidget.h  -  description
                             -------------------
    begin                : Wed Jul 17 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

#ifndef WATCHWIDGET_H
#define WATCHWIDGET_H

// C-Includes

// QT-Includes
#include <qvbox.h>
#include <qlist.h>

// KDE-Includes

// Project-Includes
#include "watchitembase.h"
#include "propertywidget.h"

// Forward declaration
class WatchListView;
class KSimusApp;
class KSimusDoc;
class KSimEditor;
class QPushButton;
class QListViewItem;
class QLabel;

/**The widget for the watch window.
  *@author Rasmus Diekenbrock
  */

class WatchWidget : public QVBox
{
class Private;

	Q_OBJECT

public:
	WatchWidget(KSimusApp * app, QWidget *parent=0, const char *name=0);
	~WatchWidget();

	void reset();
	void execute();
	
	WatchListView * getWatchListView() { return m_watchListView;};
	unsigned int getIndex() const { return m_index; };
	unsigned int getStepCount() const { return m_stepCnt; };
	
	void addWatchItem(WatchItemBase * watchItem);
	void delWatchItem(WatchItemBase * watchItem);
	
	/** Sets the size of the trace buffers.*/
	void setTraceSize(unsigned int size);
	/** Returns the size of the trace buffer. */
	unsigned int getTraceSize() const { return m_traceSize; };

	/** If true the watch widget will rise if a new item is added to the list. */
	void setRiseByAdd(bool enable);
	/** Returns true if the watch widget will rise if a new item is added to the list. */
	bool isRiseByAdd() const;

	KSimusApp * getApp();
	KSimusDoc * getDoc();
	KSimEditor * getEditor();
	

private:
	unsigned int getMinViewIndex() const;
	unsigned int getMaxViewIndex() const;
	unsigned int getPreviousIndex(unsigned int index) const;
	unsigned int getNextIndex(unsigned int index) const;
	void showHistory(unsigned int index) const;
	void setViewIndex(unsigned int index);
	void setupUI();
	
	
	Private * m_p;
	KSimusApp * m_app;
	WatchListView * m_watchListView;
	unsigned int m_index;
	unsigned int m_viewIndex;
	QList<WatchItemBase> m_watchItemList;
	unsigned int m_traceSize;
	unsigned int m_stepCnt;
	QPushButton * m_stepBegin;
	QPushButton * m_stepForward;
	QPushButton * m_stepBack;
	QPushButton * m_stepEnd;
	QLabel * m_timeLabel;
	

private slots:
	void slotSimReset();
	void slotSimStart();
	void slotSimStop();
	void slotNewDoc();
	void slotSimPaused(bool pause);
	void slotScrollBarValue(int value);
	void slotStepBegin();
	void slotStepBack();
	void slotStepForward();
	void slotStepEnd();
	void slotCurrentChanged(QListViewItem * lvi);

signals:
	void signalShowMe();
	
};


//############################################################################
//############################################################################

class WatchWidgetPropertyWidget : public PropertyWidget
{
class Private;

	Q_OBJECT

public:
	WatchWidgetPropertyWidget(WatchWidget * watchWidget, QWidget *parent=0, const char *name=0, WFlags f=0);
	~WatchWidgetPropertyWidget();

	/** The function acceptPressed() is called, if changes are accepted.
	    You have to reimplement this function, if you add new properties.
	    If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
	    You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	WatchWidget * getWatchWidget() const { return m_watchWidget; };

private:
	WatchWidget * m_watchWidget;
	Private * m_p;
};


#endif
