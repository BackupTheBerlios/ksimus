/***************************************************************************
                          ksimiopin.h  -  description
                             -------------------
    begin                : Sun Mar 30 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef KSIMIOPIN_H
#define KSIMIOPIN_H

// C/C++ includes

// QT includes
#include <qobject.h>
#include <qlist.h>

// KDE includes
#include "ksimiojoininfo.h"

// Project includes

// Forward declaration
class KSimIoDevice;
class KConfigBase;


/**Base class for io pins
  *@author Rasmus Diekenbrock
  */

class KSimIoPin : public QObject
{
   Q_OBJECT
public: 
	class List;
	class Pool;

	KSimIoPin(KSimIoDevice *parentDevice, int pinID, const QString & name, const QString & i18nName);
	~KSimIoPin();

	KSimIoDevice * getDevice() const { return m_parentDevice; };
	int getPinID() const { return m_pinID; };

	void addPinInfo(const KSimIoJoinInfo * joinInfo);

	const KSimIoJoinInfoList & getJoinInfoList() const { return m_joinInfoList; };
	const KSimIoJoinInfo * getSelectedJoinInfo() const { return m_selectedJoinInfo; };
	void setSelectedJoinInfo(const KSimIoJoinInfo * joinInfo);

	QString getDefaultName() const { return m_defaultName; };
	QString getDefaultI18nName() const { return m_defaultI18nName; };
	void setName(const QString & name);
	QString getName() const;

	void save(KConfigBase & config) const;
	bool load(KConfigBase & config);

private:
	KSimIoDevice * m_parentDevice;
	int m_pinID;
	const QString m_defaultName;
	const QString m_defaultI18nName;
	QString m_name;
	KSimIoJoinInfoList m_joinInfoList;
	const KSimIoJoinInfo * m_selectedJoinInfo;
	
};



//################################################################################
//################################################################################


class KSimIoPin::List : public QList<KSimIoPin>
{
public:
//	List();
//	~List();

	const KSimIoPin * find(const QString & deviceName, int pinID) const;
};


#define FOR_EACH_IO_PIN(_it_,_ioPinList_)  \
        for(QListIterator<KSimIoPin> _it_(_ioPinList_);_it_.current();++_it_)


//################################################################################
//################################################################################


class KSimIoPin::Pool : public KSimIoPin::List
{
public:
//	Pool();
//	~Pool();

	/** Returns the static pool of all io pins. */
	static Pool & get();


private:
};


#endif
