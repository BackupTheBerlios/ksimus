/***************************************************************************
                          watchwidget.cpp  -  description
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

// C-Includes
#include <stdio.h>

// QT-Includes
#include <qtimer.h>
#include <qhbox.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qvaluelist.h>
#include <qscrollbar.h>
#include <qslider.h>

// KDE-Includes
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "watchlistview.h"
#include "ksimdebug.h"
#include "watchwidget.h"
#include "watchviewitem.h"
#include "ksimus.h"
#include "ksimusdoc.h"
#include "ksimeditor.h"
#include "wireproperty.h"
#include "wire.h"
#include "connectorbase.h"
#include "ksimtimeserver.h"
#include "ksimtime.h"
#include "ksimicon.h"

// Forward declaration


#define UNKNOWN_TIME        QString::fromLatin1(" - - - ")
#define DEFAULT_TRACE_SIZE  50
#define DEFAULT_RISE_BY_ADD 1  // == true !!!

//###################################################################
//###################################################################

class WatchWidget::Private
{
public:
	QValueList<QString> timeList;
//	QScrollBar * scrollBar;
	QSlider * scrollBar;
	bool riseByAdd;
	
	void resize(unsigned int size);
};

void WatchWidget::Private::resize(unsigned int size)
{
	while(timeList.count() < size)
	{
		timeList.append(QString::null);
	};
	while(timeList.count() > size)
	{
		timeList.remove(timeList.fromLast());
	};
}

//###################################################################
//###################################################################

WatchWidget::WatchWidget(KSimusApp * app, QWidget *parent, const char *name )
	:	QVBox(parent,name),
		m_app(app),
		m_index(0),
		m_viewIndex(0),
		m_traceSize(DEFAULT_TRACE_SIZE),
		m_stepCnt(0)
{
	m_p = new Private;
	CHECK_PTR(m_p);

	setSpacing(KDialog::spacingHint());

	m_watchListView = new WatchListView(this, this, "WatchListView");
	CHECK_PTR(m_watchListView);

	// Button row
	QHBox * buttonBox = new QHBox(this, "buttonBox");
	CHECK_PTR(buttonBox);

	m_stepBegin = new QPushButton(QString::null, buttonBox, "button begin");
	CHECK_PTR(m_stepBegin);
	m_stepBegin->setPixmap(KSimIcon::load("start", KIcon::Small));
	m_stepBegin->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	connect(m_stepBegin, SIGNAL(clicked()), SLOT(slotStepBegin()));

	m_stepBack = new QPushButton(QString::null, buttonBox, "button back");
	CHECK_PTR(m_stepBack);
	m_stepBack->setPixmap(KSimIcon::load("back", KIcon::Small));
	m_stepBack->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	connect(m_stepBack, SIGNAL(clicked()), SLOT(slotStepBack()));

	QWidget * spacer = new QWidget(buttonBox);
	CHECK_PTR(m_stepBack);
	buttonBox->setStretchFactor(spacer,10);

	m_stepForward = new QPushButton(buttonBox, "button forward");
	CHECK_PTR(m_stepForward);
	m_stepForward->setPixmap(KSimIcon::load("forward", KIcon::Small));
	m_stepForward->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	connect(m_stepForward, SIGNAL(clicked()), SLOT(slotStepForward()));

	m_stepEnd = new QPushButton(QString::null, buttonBox, "button end");
	CHECK_PTR(m_stepEnd);
	m_stepEnd->setPixmap(KSimIcon::load("finish", KIcon::Small));
	m_stepEnd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	connect(m_stepEnd, SIGNAL(clicked()), SLOT(slotStepEnd()));

	m_p->scrollBar = new QSlider(Qt::Horizontal, this, "Watch scrollbar");
	CHECK_PTR(m_p->scrollBar);


	// Time and Tick
	QHBox * timeWid = new QHBox(this, "timeBox");
	CHECK_PTR(timeWid);
	timeWid->setSpacing(KDialog::spacingHint());
//	timeWid->setMargin(KDialog::marginHint());
	
	QLabel * lab = new QLabel(i18n("Watch List", "Time:"), timeWid);
	CHECK_PTR(lab);
	lab->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
	m_timeLabel = new QLabel(UNKNOWN_TIME, timeWid, "m_timeLabel");
	
	CHECK_PTR(m_timeLabel);
	
	m_watchItemList.setAutoDelete(false);
	connect(getDoc(), SIGNAL(signalPaused(bool)), SLOT(slotSimPaused(bool)));
	connect(getDoc(), SIGNAL(signalPreReset()), SLOT(slotSimReset()));
	connect(getDoc(), SIGNAL(signalStart()), SLOT(slotSimStart()));
	connect(getDoc(), SIGNAL(signalStop()), SLOT(slotSimStop()));
	connect(getDoc(), SIGNAL(signalNewDoc()), SLOT(slotNewDoc()));
	connect(m_p->scrollBar, SIGNAL(valueChanged(int)), SLOT(slotScrollBarValue(int)));

	connect(m_watchListView, SIGNAL(currentChanged(QListViewItem *)), SLOT(slotCurrentChanged(QListViewItem *)));
	

	m_p->resize(getTraceSize());
	setRiseByAdd(DEFAULT_RISE_BY_ADD);
	
	setupUI();
}

WatchWidget::~WatchWidget()
{
//	KSIMDEBUG("WatchWidget::~WatchWidget()");
	while(m_watchItemList.count())
	{
		// The item removes itself from the list !!!!
		delete m_watchItemList.first();
	}
	delete m_p;
}

KSimusApp * WatchWidget::getApp()
{
	return m_app;
}

KSimusDoc * WatchWidget::getDoc()
{
	return getApp()->getDocument();
}

KSimEditor * WatchWidget::getEditor()
{
	return getDoc()->getActiveEditor();
}


void WatchWidget::reset()
{
	m_index = 0;
	m_viewIndex = 0;
	m_stepCnt = 0;

	QListIterator<WatchItemBase> it(m_watchItemList);
	for(;it.current();++it)
	{
		it.current()->reset();
		it.current()->showData(m_viewIndex);
	}
	m_timeLabel->setText(UNKNOWN_TIME);
	m_p->scrollBar->setValue(0);
	setupUI();
}

void WatchWidget::execute()
{
	static const QString timeFormat(i18n("Watch - Time format","%1 / %2 ticks"));
	// Do nothing if no watches are selected
	if (m_watchItemList.count() == 0) return;
	
	bool commonBrk = false;
	if (m_stepCnt < getTraceSize()) m_stepCnt++;
	QListIterator<WatchItemBase> it(m_watchItemList);
	for(;it.current();++it)
	{
		bool brk = it.current()->execute(m_index, m_stepCnt);
		if (brk)
		{
			commonBrk = true;
			if (it.current()->getConnector())
			{
				it.current()->getConnector()->logWarning(i18n("causes a break"));
			}
			else if (it.current()->getWireProperty())
			{
				it.current()->getWireProperty()->logWarning(i18n("causes a break"));
			}
		}
		it.current()->showData(m_index);
	}
	if (commonBrk)
	{
		QTimer::singleShot(0, m_app, SLOT(slotExecutePause()));
	}

	char tmp[40];
	sprintf(tmp,"%lli",getDoc()->getTimeServer().raw() / getDoc()->getTimeServer().getTick().raw());
	QString val = timeFormat.arg(getDoc()->getTimeServer().getAdjustValueString(4))
	                        .arg(QString::fromLatin1(tmp));
	m_p->timeList[m_index] = val;
	m_timeLabel->setText(val);

			
	m_viewIndex = m_index;		//current view index
	m_index = getNextIndex(m_index);	// Next index
}


void WatchWidget::addWatchItem(WatchItemBase * watchItem)
{
	if(watchItem)
	{
		m_watchItemList.append(watchItem);
		WatchViewItem * wvi = new WatchViewItem(watchItem, getWatchListView());
		CHECK_PTR(wvi);
		watchItem->setWatchViewItem(wvi);
		watchItem->reset();
		watchItem->showData(m_index);
		if (isRiseByAdd())
		{
			emit signalShowMe();
		}
	}
}

void WatchWidget::delWatchItem(WatchItemBase * watchItem)
{
	m_watchItemList.removeRef(watchItem);
	if (m_watchItemList.count() == 0)
	{
		reset();
	}
}

void WatchWidget::setTraceSize(unsigned int size)
{
	m_traceSize = size;

	QListIterator<WatchItemBase> it(m_watchItemList);
	for(;it.current();++it)
	{
		it.current()->setTraceSize(size);
		it.current()->reset();
	}
	m_p->resize(size);
	reset();
}

void WatchWidget::slotSimReset()
{
	reset();
}

void WatchWidget::slotSimStart()
{
	setupUI();
}

void WatchWidget::slotSimStop()
{
	setupUI();
	m_p->scrollBar->setValue(getStepCount()-1);
}

void WatchWidget::slotNewDoc()
{
	reset();
}

void WatchWidget::slotSimPaused(bool pause)
{
	setupUI();
	if(pause)
	{
		m_p->scrollBar->setValue(getStepCount()-1);
	}
	else
	{
		QListIterator<WatchItemBase> it(m_watchItemList);
		for(;it.current();++it)
		{
			if (it.current()->isBreakEnabled() && (it.current()->getBreaksActual() == 0))
			{
				it.current()->setBreaksActual(it.current()->getBreaksRequired());
			}
		}
	}
}

void WatchWidget::slotScrollBarValue(int value)
{
//	KSIMDEBUG_VAR("slotScrollBarValue", value);
	unsigned int i = value + getMinViewIndex();
	if (i >= getTraceSize()) i -= getTraceSize();
//	KSIMDEBUG_VAR("slotScrollBarValue", i);
	if (m_viewIndex != i) setViewIndex(i);
}


void WatchWidget::setupUI()
{
	bool simActive = getDoc()->isSimulationRunning() && !getDoc()->isSimulationPaused();
	bool backEna = (m_viewIndex != getMinViewIndex()) && !simActive;
	bool forwardEna = (m_viewIndex != getMaxViewIndex()) && !simActive;

	m_stepBegin->setEnabled(backEna);
	m_stepBack->setEnabled(backEna);
	m_stepForward->setEnabled(forwardEna);
	m_stepEnd->setEnabled(forwardEna);
	m_p->scrollBar->setEnabled(!simActive);
	m_p->scrollBar->setMinValue(0);
	int i = getStepCount()-1;
	if (i < 0) i = 0;
	m_p->scrollBar->setMaxValue(i);
	m_p->scrollBar->setLineStep(1);
//	m_p->scrollBar->setPageStep(getStepCount()/20);
}

unsigned int WatchWidget::getMinViewIndex() const
{
	if (m_stepCnt < getTraceSize())
	{
		return 0;
	}
	else
	{
		return m_index;
	}
}

unsigned int WatchWidget::getMaxViewIndex() const
{
	if (m_stepCnt == 0)
	{
		return 0;
	}
	else
	{
		return getPreviousIndex(m_index);
	}
}

unsigned int WatchWidget::getPreviousIndex(unsigned int index) const
{
	return ((index == 0) ? getTraceSize() : index) -1;
}

unsigned int WatchWidget::getNextIndex(unsigned int index) const
{
	if (++index >= getTraceSize())
	{
		return 0;
	}
	return index;
}

void WatchWidget::slotStepBegin()
{
	setViewIndex(getMinViewIndex());
}
void WatchWidget::slotStepBack()
{
	setViewIndex(getPreviousIndex(m_viewIndex));
}
void WatchWidget::slotStepForward()
{
	setViewIndex(getNextIndex(m_viewIndex));
}
void WatchWidget::slotStepEnd()
{
	setViewIndex(getMaxViewIndex());
}

void WatchWidget::showHistory(unsigned int index) const
{
	m_timeLabel->setText(m_p->timeList[index]);
	QListIterator<WatchItemBase> it(m_watchItemList);
	for(;it.current();++it)
	{
		it.current()->showData(index);
		it.current()->showBreakCountHistory(index);
	}
}

void WatchWidget::setViewIndex(unsigned int index)
{
	m_viewIndex = index;
	showHistory(m_viewIndex);
	setupUI();
	int i = (int)index - (int)getMinViewIndex();
	if (i < 0) i += getTraceSize();
//	KSIMDEBUG(QString::fromLatin1("i=%1 index=%2 getMinViewIndex=%3 getStepCount=%4").arg(i).arg(index).arg(getMinViewIndex()).arg(getStepCount()));
	m_p->scrollBar->setValue(i);
}


void WatchWidget::slotCurrentChanged(QListViewItem * lvi)
{
	if (!lvi) return;
	WatchItemBase * wib = ((WatchViewItem*)lvi)->getWatchItem();
	getEditor()->unselectAll();
	if (wib->getConnector())
	{
		getEditor()->select(wib->getConnector()->getComponent(), true);
	}
	else if (wib->getWireProperty())
	{
		FOR_EACH_CONNECTOR(it, *wib->getWireProperty()->getWire()->getConnList())
		{
			getEditor()->select(it.current()->getComponent(), true);
		}
	}
}

void WatchWidget::setRiseByAdd(bool enable)
{
	m_p->riseByAdd = enable;
}

bool WatchWidget::isRiseByAdd() const
{
	return m_p->riseByAdd;
}

//############################################################################
//############################################################################


#include "ksimspinbox.h"
#include <qcheckbox.h>

class WatchWidgetPropertyWidget::Private
{
public:
	KSimSpinBox * traceSize;
	QCheckBox * riseByAdd;
};

//############################################################################
//############################################################################

WatchWidgetPropertyWidget::WatchWidgetPropertyWidget(WatchWidget * watchWidget, QWidget *parent, const char *name, WFlags f)
	:	PropertyWidget(2, parent, name, f),
		m_watchWidget(watchWidget)
{
	m_p = new Private();
	CHECK_PTR(m_p);

	setCenterRowStretch(0);
	setBottomRowStretch(1);
	
	QLabel * label;

	label = new QLabel(i18n("Watch settings", "Trace buffer size:"), this, "history depth label");
	CHECK_PTR(label);

	m_p->traceSize = new KSimSpinBox(1, 10000, 10, this, "history depth spinbox");
	CHECK_PTR(m_p->traceSize);
	addToolTip(i18n("Set the size of the trace buffer."),
	           m_p->traceSize, label);
	addWhatsThis(i18n("Set the size of the trace buffer.\n\n"
	                  "The values of each watchpoint are traced during execution. "
	                  "You can examin these traced values if the execution is stopped or paused. "
	                  "Enter here the maximum number of traced values.\n\n"
	                  "The default is %1 ticks.").arg(DEFAULT_TRACE_SIZE),
	             m_p->traceSize, label);
	m_p->traceSize->setValue(getWatchWidget()->getTraceSize());
	m_p->traceSize->setSuffix(i18n("Suffix unit ticks (leading space)"," ticks"));


	QWidget * rowWid = newRowHBox("rowWid riseByAdd");
	CHECK_PTR(rowWid);
	m_p->riseByAdd = new QCheckBox(i18n("Watch settings", "Rise watch view if watchpoint is added"), rowWid, "riseByAdd");
	CHECK_PTR(m_p->riseByAdd);
	addToolTip(i18n("Rises the watch view if a new watchpoint is added."),
	           m_p->riseByAdd);
	addWhatsThis(i18n("Rises the watch view if a new watchpoint is added.\n\n"
	                  "If checked the watch view is rised each time the user will add a new watchpoint to the watch list. "
	                  "Disable it if you don't like this.\n\nThe default is enabled."),
	             m_p->riseByAdd);
#if DEFAULT_RISE_BY_ADD != 1
#error Fix addWhatsThis: DEFAULT_RISE_BY_ADD != 1
#endif
	m_p->riseByAdd->setChecked(getWatchWidget()->isRiseByAdd());
	
}


WatchWidgetPropertyWidget::~WatchWidgetPropertyWidget()
{
	delete m_p;
}

void WatchWidgetPropertyWidget::acceptPressed()
{
	PropertyWidget::acceptPressed();

	if (getWatchWidget()->getTraceSize() != (unsigned int)m_p->traceSize->value())
	{
		changeData();
		getWatchWidget()->setTraceSize(m_p->traceSize->value());
	}
	
	if (getWatchWidget()->isRiseByAdd() != m_p->riseByAdd->isChecked())
	{
		changeData();
		getWatchWidget()->setRiseByAdd(m_p->riseByAdd->isChecked());
	}

}

void WatchWidgetPropertyWidget::defaultPressed()
{
	PropertyWidget::defaultPressed();

	m_p->traceSize->setValue(DEFAULT_TRACE_SIZE);
	m_p->riseByAdd->setChecked(DEFAULT_RISE_BY_ADD);
	
}


#include "watchwidget.moc"


