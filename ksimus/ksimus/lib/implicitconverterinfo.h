/***************************************************************************
                          implicitconverterinfo.h  -  description
                             -------------------
    begin                : Wed Aug 14 2002
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

#ifndef IMPLICITCONVERTERINFO_H
#define IMPLICITCONVERTERINFO_H

#include <qstring.h>
#include <qptrlist.h>

#include "baseinfo.h"

/**Info class for implicit converter
  *@author Rasmus Diekenbrock
  */
class ConnectorBase;
class ImplicitConverter;


class ImplicitConverterInfo : public BaseInfo
{
public:
	ImplicitConverterInfo(const QString & name,
	                      const QString & libName,
	                      const QString & inDataType,
	                      const QString & outDataType,
	                      ImplicitConverter * (*factory)(ConnectorBase*),
	                      const QString & shortDescr = QString::null,
	                      const QString & HTMLDescr = QString::null,
	                      const QString & oldLibNames = QString::null );
	
	const QString & getInDataType() const;
	const QString & getOutDataType() const;
	ImplicitConverter * create(ConnectorBase*) const;
	
private:

	const QString m_inDataType;
	const QString m_outDataType;
	ImplicitConverter * (*m_factory)(ConnectorBase*);
	
	
};

class ImplicitConverterInfoList : public QPtrList<const ImplicitConverterInfo>
{
};

#define FOR_EACH_IMPLICITCONVERTER_INFO(_it_,_ImplicitConverterInfoList_)	\
		for(QPtrListIterator<const ImplicitConverterInfo> _it_(_ImplicitConverterInfoList_);_it_.current();++_it_)




#endif
