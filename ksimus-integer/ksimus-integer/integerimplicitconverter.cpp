/***************************************************************************
                          integerimplicitconverter.cpp  -  description
                             -------------------
    begin                : Sat Aug 2 2003
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

// C includes

// QT includes

// KDE includes

// KSimus-Includes
#include "ksimus/implicitconverterinfo.h"
#include "ksimus/connectorbase.h"
#include "ksimus/ksimdebug.h"

// Project includes
#include "integerimplicitconverter.h"

// Forward declaration

namespace KSimLibInteger
{

//#######################################################################
//#######################################################################

ImplicitConverter * ImplicitConverterInteger2Boolean::create(ConnectorBase * connector)
{
	ImplicitConverter * ic;
	ic = new ImplicitConverterInteger2Boolean(connector);
	Q_CHECK_PTR(ic);
	return ic;
}

const ImplicitConverterInfo * ImplicitConverterInteger2Boolean::getStaticInfo()
{
	static const ImplicitConverterInfo Info(QString::fromLatin1("Implicit Converter Integer to Boolean"),
	                                        QString::fromLatin1("implicitconverter/Integer2Boolean"),
	                                        QString::fromLatin1("Integer"),
	                                        QString::fromLatin1("Boolean"),
	                                        create );
	return &Info;
}


const void * ImplicitConverterInteger2Boolean::convert(const void * data)
{
	if ((data == 0) || ((*(const int *) data) == 0))
	{
		m_result = false;
	}
	else
	{
		m_result = true;
	}

	return &m_result;
}

//#######################################################################
//#######################################################################

ImplicitConverter * ImplicitConverterBoolean2Integer::create(ConnectorBase * connector)
{
	ImplicitConverter * ic;
	ic = new ImplicitConverterBoolean2Integer(connector);
	Q_CHECK_PTR(ic);
	return ic;
}

const ImplicitConverterInfo * ImplicitConverterBoolean2Integer::getStaticInfo()
{
	static const ImplicitConverterInfo Info(QString::fromLatin1("Implicit Converter Boolean to Integer"),
	                                        QString::fromLatin1("implicitconverter/Boolean2Integer"),
	                                        QString::fromLatin1("Boolean"),
	                                        QString::fromLatin1("Integer"),
	                                        create );
	return &Info;
}


const void * ImplicitConverterBoolean2Integer::convert(const void * data)
{
	if ((data == 0) || ((*(const bool *) data) == false))
	{
		m_result = 0;
	}
	else
	{
		m_result = 1;
	}

	return &m_result;
}

//#######################################################################
//#######################################################################

ImplicitConverter * ImplicitConverterInteger2Float::create(ConnectorBase * connector)
{
	ImplicitConverter * ic;
	ic = new ImplicitConverterInteger2Float(connector);
	Q_CHECK_PTR(ic);
	return ic;
}

const ImplicitConverterInfo * ImplicitConverterInteger2Float::getStaticInfo()
{
	static const ImplicitConverterInfo Info(QString::fromLatin1("Implicit Converter Integer to Floating Point"),
	                                        QString::fromLatin1("implicitconverter/Integer2FloatingPoint"),
	                                        QString::fromLatin1("Integer"),
	                                        QString::fromLatin1("Floating Point"),
	                                        create );
	return &Info;
}


const void * ImplicitConverterInteger2Float::convert(const void * data)
{
	m_result = data
	           ? (double)(*(const int *) data)
	           : 0.0;

	return &m_result;
}

//#######################################################################
//#######################################################################

ImplicitConverter * ImplicitConverterFloat2Integer::create(ConnectorBase * connector)
{
	ImplicitConverter * ic;
	ic = new ImplicitConverterFloat2Integer(connector);
	Q_CHECK_PTR(ic);
	return ic;
}

const ImplicitConverterInfo * ImplicitConverterFloat2Integer::getStaticInfo()
{
	static const ImplicitConverterInfo Info(QString::fromLatin1("Implicit Converter Floating Point to Integer"),
	                                        QString::fromLatin1("implicitconverter/FloatingPoint2Integer"),
	                                        QString::fromLatin1("Floating Point"),
	                                        QString::fromLatin1("Integer"),
	                                        create );
	return &Info;
}


const void * ImplicitConverterFloat2Integer::convert(const void * data)
{
	m_result = data
	           ? qRound((*(const double *) data))
	           : 0;

	return &m_result;
}

//#######################################################################
//#######################################################################

};  //namespace KSimLibInteger
