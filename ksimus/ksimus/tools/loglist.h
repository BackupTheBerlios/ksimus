/***************************************************************************
                          loglist.h  -  description
                             -------------------
    begin                : Mon Sep 11 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef LOGLIST_H
#define LOGLIST_H

// C-Includes

// QT-Includes
#include <qlistbox.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class LogListItem;
class KConfigBase;
class LogList;
class KSimusApp;



/**A message management class

  *@author Rasmus Diekenbrock
  */



#define LOG_DEBUG	0x01
#define LOG_INFO	0x02
#define LOG_WARNING	0x04
#define LOG_ERROR	0x08


class LogListProperty
{

public:
	LogListProperty();
	LogListProperty(const LogListProperty & llp);
	~LogListProperty();

    const LogListProperty & operator=(const LogListProperty & llp);
	
	void setPriorityMask(unsigned int newMask) { m_priorityMask = newMask & (LOG_DEBUG|LOG_INFO|LOG_WARNING|LOG_ERROR); };
	unsigned int getPriorityMask() const { return m_priorityMask; };
	
	void setPopupMask(unsigned int newMask) { m_popupMask = newMask & (LOG_DEBUG|LOG_INFO|LOG_WARNING|LOG_ERROR); };
	unsigned int getPopupMask() const { return m_popupMask; };
	
	void setDebugEnable(bool enable);
	bool isDebugEnabled() const { return m_priorityMask & LOG_DEBUG; };
	
	void setInfoEnable(bool enable);
	bool isInfoEnabled() const { return m_priorityMask & LOG_INFO; };
	
	void setWarningEnable(bool enable);
	bool isWarningEnabled() const { return m_priorityMask & LOG_WARNING; };
	
	void setErrorEnable(bool enable);
	bool isErrorEnabled() const { return m_popupMask & LOG_ERROR; };
	
	
	void setDebugPopup(bool popup);
	bool isDebugPopup() const { return m_popupMask & LOG_DEBUG; };
	
	void setInfoPopup(bool popup);
	bool isInfoPopup() const { return m_popupMask & LOG_INFO; };
	
	void setWarningPopup(bool popup);
	bool isWarningPopup() const { return m_popupMask & LOG_WARNING; };
	
	void setErrorPopup(bool popup);
	bool isErrorPopup() const { return m_popupMask & LOG_ERROR; };
	
	/** Save LogList property */
	void save(KConfigBase & file) const;
	/** Load LogList property */
	void load(KConfigBase & file);
	
protected:
	unsigned int m_priorityMask;
	unsigned int m_popupMask;
};




class LogList : public QListBox, public LogListProperty
{
   Q_OBJECT

public:
	
	#ifdef DEBUG
		#define logDebug(p0)								debug(__FILE__,__LINE__,p0)
		#define logDebug1(p0)								debug(__FILE__,__LINE__,p0)
		#define logDebug2(p0,p1)							debug(__FILE__,__LINE__,p0,p1)
		#define logDebug3(p0,p1,p2)							debug(__FILE__,__LINE__,p0,p1,p2)
		#define logDebug4(p0,p1,p2,p3)						debug(__FILE__,__LINE__,p0,p1,p2,p3)
		#define logDebug5(p0,p1,p2,p3,p4)					debug(__FILE__,__LINE__,p0,p1,p2,p3,p4)
		#define logDebug6(p0,p1,p2,p3,p4,p5)				debug(__FILE__,__LINE__,p0,p1,p2,p3,p4,p5)
		#define logDebug7(p0,p1,p2,p3,p4,p5,p6)				debug(__FILE__,__LINE__,p0,p1,p2,p3,p4,p5,p6)
		#define logDebug8(p0,p1,p2,p3,p4,p5,p6,p7)			debug(__FILE__,__LINE__,p0,p1,p2,p3,p4,p5,p6,p7)
		#define logDebug9(p0,p1,p2,p3,p4,p5,p6,p7,p8)		debug(__FILE__,__LINE__,p0,p1,p2,p3,p4,p5,p6,p7,p8)
		#define logDebug10(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9)	debug(__FILE__,__LINE__,p0,p1,p2,p3,p4,p5,p6,p7,p8,p9)
	#else
		#define logDebug(p0)								nop()
		#define logDebug1(p0)								nop()
		#define logDebug2(p0,p1)							nop()
		#define logDebug3(p0,p1,p2)							nop()
		#define logDebug4(p0,p1,p2,p3)						nop()
		#define logDebug5(p0,p1,p2,p3,p4)					nop()
		#define logDebug6(p0,p1,p2,p3,p4,p5)				nop()
		#define logDebug7(p0,p1,p2,p3,p4,p5,p6)				nop()
		#define logDebug8(p0,p1,p2,p3,p4,p5,p6,p7)			nop()
		#define logDebug9(p0,p1,p2,p3,p4,p5,p6,p7,p8)		nop()
		#define logDebug10(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9)	nop()
	#endif


	LogList(KSimusApp * app, QWidget *parent=0, const char *name=0);
	~LogList();
	
	void append(LogListItem * item);
	
	void debug(const char * file, int line, const char * format, ...);
	void info(const char * format, ...);
	void warning(const char * format, ...);
	void error(const char * format, ...);
	void nop() const { ; };
	
protected:

	virtual void mousePressEvent(QMouseEvent * ev);
	KSimusApp * m_app;

protected slots:
	void slotSelected(int index);
	void slotHighlighted(int index);
	void slotToggleDebug();
	void slotToggleInfo();
	void slotToggleWarning();
	void slotToggleError();
	void slotClear();

signals:
	void signalShow();

};

#endif
