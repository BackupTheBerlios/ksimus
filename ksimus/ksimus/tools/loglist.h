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
#include <qcolor.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class LogListItem;
class KConfigBase;
class LogList;
class KSimusApp;
class QColor;



/**A message management class

  *@author Rasmus Diekenbrock
  */



#define LOG_DEBUG   0x01
#define LOG_INFO    0x02
#define LOG_WARNING 0x04
#define LOG_ERROR   0x08


class LogListProperty
{

public:
	LogListProperty();
	~LogListProperty();

	void setPriorityMask(unsigned int newMask) { m_priorityMask = newMask & (LOG_DEBUG|LOG_INFO|LOG_WARNING|LOG_ERROR); };
	unsigned int getPriorityMask() const { return m_priorityMask; };
	static unsigned int getDefaultPriorityMask();
	
	void setPopupMask(unsigned int newMask) { m_popupMask = newMask & (LOG_DEBUG|LOG_INFO|LOG_WARNING|LOG_ERROR); };
	unsigned int getPopupMask() const { return m_popupMask; };
	static unsigned int getDefaultPopupMask();
	
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


	void setDebugColor(const QColor & color);
	const QColor & getDebugColor() const { return m_debugColor; };

	void setInfoColor(const QColor & color);
	const QColor & getInfoColor() const { return m_infoColor; };

	void setWarningColor(const QColor & color);
	const QColor & getWarningColor() const { return m_warningColor; };

	void setErrorColor(const QColor & color);
	const QColor & getErrorColor() const { return m_errorColor; };
	

	static const QColor & getDefaultDebugColor();
	static const QColor & getDefaultInfoColor();
	static const QColor & getDefaultWarningColor();
	static const QColor & getDefaultErrorColor();


	/** Save LogList property */
	void save(KConfigBase & file) const;
	/** Load LogList property */
	void load(KConfigBase & file);
	
protected:
	unsigned int m_priorityMask;
	unsigned int m_popupMask;
	QColor m_debugColor;
	QColor m_infoColor;
	QColor m_warningColor;
	QColor m_errorColor;
};




class LogList : public QListBox, public LogListProperty
{
	class Private;

	Q_OBJECT

public:
	
	#ifdef DEBUG
		#define logDebug(p)      debug(__FILE__,__LINE__,p)
	#else
		#define logDebug(p)      nop()
	#endif


	LogList(KSimusApp * app, QWidget *parent=0, const char *name=0);
	~LogList();
	
	void append(LogListItem * item);
	
	void debug(const char * file, int line, const char * txt);
	void debug(const char * file, int line, const QString & s);
	void info(const QString & s);
	void warning(const QString & s);
	void error(const QString & s);
	void nop() const { ; };
	
protected:

	virtual void mousePressEvent(QMouseEvent * ev);

protected slots:
	void slotSelected(int index);
	void slotHighlighted(int index);
	void slotToggleDebug();
	void slotToggleInfo();
	void slotToggleWarning();
	void slotToggleError();
	void slotClear();

private:
	Private * m_p;

signals:
	void signalShow();

};

#endif
