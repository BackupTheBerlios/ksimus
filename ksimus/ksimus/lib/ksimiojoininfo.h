/***************************************************************************
                          ksimiojoininfo.h  -  description
                             -------------------
    begin                : Fri Apr 18 2003
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

#ifndef KSIMIOJOININFO_H
#define KSIMIOJOININFO_H

#include <qstring.h>
#include <qptrlist.h>

#include "baseinfo.h"

/**Information class for io joins
  *@author Rasmus Diekenbrock
  */

class KSimIoJoin;
class KSimIoComponent;


class KSimIoJoinInfo : public BaseInfo
{
public: 
	KSimIoJoinInfo(const QString & name,
	               const QString & i18nLibName,
	               const QString & libName,
	               const QString & additionalI18nLibNames,
	               KSimIoJoin * (*factory)(KSimIoComponent *, const KSimIoJoinInfo *),
	               const QString & shortDescr = QString::null,
	               const QString & HTMLDescr = QString::null,
	               const QString & oldLibNames = QString::null );

	KSimIoJoin * create(KSimIoComponent * comp, const KSimIoJoinInfo * info) const;
	
	const QString & getI18nLibName() const { return m_i18nLibName; };
	const QString & getAdditionalI18nLibNames() const { return m_additionalI18nLibNames; };

private:
	const QString m_i18nLibName;
	const QString m_additionalI18nLibNames;
	KSimIoJoin * (*m_factory)(KSimIoComponent *, const KSimIoJoinInfo *);
};




//################################################################################
//################################################################################




class KSimIoJoinInfoList : public QPtrList<const KSimIoJoinInfo>
{
};



//################################################################################
//################################################################################



#define FOR_EACH_IO_JOIN_INFO(_it_,_ioJoinInfoList_)	\
		for(QPtrListIterator<const KSimIoJoinInfo> _it_(_ioJoinInfoList_);_it_.current();++_it_)

#endif

