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
#include "implicitconverterinfo.h"
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


//###############################################################################################
//###############################################################################################


static ImplicitConverter * createImplicitConverterBoolean2Float(ConnectorBase * connector)
{
	ImplicitConverter * ic;
	ic = new ImplicitConverterBoolean2Float(connector);
	CHECK_PTR(ic);
	return ic;
}

const ImplicitConverterInfo * getImplicitConverterBoolean2FloatInfo()
{
	static const ImplicitConverterInfo Info(QString::fromLatin1("Implicit Converter Boolean to Floatingpoint"),
	                                        QString::fromLatin1("implicitconverter/Boolean2Float"),
	                                        QString::fromLatin1("Boolean"),
	                                        QString::fromLatin1("Floating Point"),
	                                        createImplicitConverterBoolean2Float );
	return &Info;
}


ImplicitConverterBoolean2Float::ImplicitConverterBoolean2Float(ConnectorBase * connector)
	: ImplicitConverter(connector, getImplicitConverterBoolean2FloatInfo())
{
}

ImplicitConverterBoolean2Float::~ImplicitConverterBoolean2Float()
{
}

const void * ImplicitConverterBoolean2Float::convert(const void * data)
{
	if (!data || ((*(const bool *) data) == false))
	{
		m_result = 0;
	}
	else
	{
		m_result = 1;
	}

	return &m_result;
}

