/***************************************************************************
                          ksimiojoinboolin.cpp  -  description
                             -------------------
    begin                : Sat Apr 5 2003
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

// C/C++ includes

// QT includes

// KDE includes
#include <klocale.h>

// Project includes
#include "ksimiojoinboolin.h"
#include "ksimiopin.h"
#include "ksimiojoininfo.h"
#include "connectorboolin.h"
#include "connectorboolout.h"

// Forward declaration


const KSimIoJoinInfo * KSimIoJoinBoolIn::getStaticInfo()
{
/* The required parameter of KSimIoJoinInfo
	KSimIoJoinInfo(const QString & name,
	               const QString & i18nLibName,
	               const QString & libName,
	               const QString & additionalI18nLibNames,
	               KSimIoJoin * (*factory)(const KSimIoJoinInfo *),
	               const QString & shortDescr = QString::null,
	               const QString & HTMLDescr = QString::null,
	               const QString & oldLibNames = QString::null );*/

	static KSimIoJoinInfo info(QString::fromLatin1("Pin Boolean In"),
	                           i18n("IO Join", "Boolean Input"),
	                           QString::fromLatin1("Pin Boolean In"),
	                           QString::null,
	                           create);
	return &info;
}

KSimIoJoin * KSimIoJoinBoolIn::create(KSimIoComponent * comp, const KSimIoJoinInfo * info)
{
	KSimIoJoin * join = new KSimIoJoinBoolIn(comp, info);
	CHECK_PTR(join);
	return join;
}


KSimIoJoinBoolIn::KSimIoJoinBoolIn(KSimIoComponent * comp, const KSimIoJoinInfo * info)
	:	KSimIoJoin(comp, info)
{
}

KSimIoJoinBoolIn::~KSimIoJoinBoolIn()
{
}

ConnectorBase * KSimIoJoinBoolIn::createConnector()
{
	ConnectorBoolIn * conn = new ConnectorBoolIn(getComponent(), QString::null, QString::null);
	CHECK_PTR(conn);
	conn->setErasable(true);
	setConnector(conn);
	return conn;
}

void KSimIoJoinBoolIn::calculate() const
{
	
}


//###################################################################################
//###################################################################################
// TODO move to own file


const KSimIoJoinInfo * KSimIoJoinBoolOut::getStaticInfo()
{
/* The required parameter of KSimIoJoinInfo
	KSimIoJoinInfo(const QString & name,
	               const QString & i18nLibName,
	               const QString & libName,
	               const QString & additionalI18nLibNames,
	               KSimIoJoin * (*factory)(const KSimIoJoinInfo *),
	               const QString & shortDescr = QString::null,
	               const QString & HTMLDescr = QString::null,
	               const QString & oldLibNames = QString::null );*/

	static KSimIoJoinInfo info(QString::fromLatin1("Pin Boolean Out"),
	                           i18n("IO Join", "Boolean Output"),
	                           QString::fromLatin1("Pin Boolean Out"),
	                           QString::null,
	                           create);
	return &info;
}

KSimIoJoin * KSimIoJoinBoolOut::create(KSimIoComponent * comp, const KSimIoJoinInfo * info)
{
	KSimIoJoin * join = new KSimIoJoinBoolOut(comp, info);
	CHECK_PTR(join);
	return join;
}


KSimIoJoinBoolOut::KSimIoJoinBoolOut(KSimIoComponent * comp, const KSimIoJoinInfo * info)
	:	KSimIoJoin(comp, info)
{
}

KSimIoJoinBoolOut::~KSimIoJoinBoolOut()
{
}


ConnectorBase * KSimIoJoinBoolOut::createConnector()
{
	ConnectorBoolOut * conn = new ConnectorBoolOut(getComponent(), QString::null, QString::null);
	CHECK_PTR(conn);
	conn->setErasable(true);
	setConnector(conn);
	return conn;
}

void KSimIoJoinBoolOut::calculate() const
{

}
