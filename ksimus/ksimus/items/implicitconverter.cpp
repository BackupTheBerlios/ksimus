/***************************************************************************
                          implicitconverter.cpp  -  description
                             -------------------
    begin                : Mit Aug 14 2002
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

// C includes

// QT includes

// KDE includes

// Project includes
#include "implicitconverter.h"
#include "connectorbase.h"

// Forward declaration


//###############################################################################################
//###############################################################################################

class ImplicitConverter::Private
{
public:

	Private(const ImplicitConverterInfo * ici)
		: implicitConverterInfo(ici)
	{
	};

	const ImplicitConverterInfo * implicitConverterInfo;
};


//###############################################################################################
//###############################################################################################



ImplicitConverter::ImplicitConverter(ConnectorBase * connector, const ImplicitConverterInfo * implicitConverterInfo)
	:	QObject(connector)
{
	m_p = new Private(implicitConverterInfo);
	CHECK_PTR(m_p);

}

ImplicitConverter::~ImplicitConverter()
{
	delete m_p;
}

const ImplicitConverterInfo * ImplicitConverter::getInfo() const
{
	return m_p->implicitConverterInfo;
}
