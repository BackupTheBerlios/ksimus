/***************************************************************************
                          integerimplicitconverter.h  -  description
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

#ifndef INTEGERIMPLICITCONVERTER_H
#define INTEGERIMPLICITCONVERTER_H

// C includes

// QT includes

// KDE includes

// KSimus-Includes
#include "ksimus/implicitconverter.h"

// Project includes

// Forward declaration

namespace KSimLibInteger
{

//#######################################################################
//#######################################################################

class ImplicitConverterInteger2Boolean : public ImplicitConverter
{
public:
	ImplicitConverterInteger2Boolean(ConnectorBase * connector)
	: ImplicitConverter(connector, getStaticInfo())
	{};
//	~ImplicitConverterInteger2Boolean();

	/** The converting function. Is called by the connector if converting is needed. */
	virtual const void * convert(const void * data);

	static const ImplicitConverterInfo * getStaticInfo();
	static ImplicitConverter * create(ConnectorBase * connector);

private:
	bool m_result;
};

//#######################################################################
//#######################################################################

class ImplicitConverterBoolean2Integer : public ImplicitConverter
{
public:
	ImplicitConverterBoolean2Integer(ConnectorBase * connector)
	: ImplicitConverter(connector, getStaticInfo())
	{};
//	~ImplicitConverterBoolean2Integer();

	/** The converting function. Is called by the connector if converting is needed. */
	virtual const void * convert(const void * data);

	static const ImplicitConverterInfo * getStaticInfo();
	static ImplicitConverter * create(ConnectorBase * connector);

private:
	int m_result;
};

//#######################################################################
//#######################################################################

class ImplicitConverterInteger2Float : public ImplicitConverter
{
public:
	ImplicitConverterInteger2Float(ConnectorBase * connector)
	: ImplicitConverter(connector, getStaticInfo())
	{};
//	~ImplicitConverterInteger2Float();

	/** The converting function. Is called by the connector if converting is needed. */
	virtual const void * convert(const void * data);

	static const ImplicitConverterInfo * getStaticInfo();
	static ImplicitConverter * create(ConnectorBase * connector);

private:
	double m_result;
};

//#######################################################################
//#######################################################################

class ImplicitConverterFloat2Integer : public ImplicitConverter
{
public:
	ImplicitConverterFloat2Integer(ConnectorBase * connector)
	: ImplicitConverter(connector, getStaticInfo())
	{};
//	~ImplicitConverterFloat2Integer();

	/** The converting function. Is called by the connector if converting is needed. */
	virtual const void * convert(const void * data);

	static const ImplicitConverterInfo * getStaticInfo();
	static ImplicitConverter * create(ConnectorBase * connector);

private:
	int m_result;
};

//#######################################################################
//#######################################################################

};  //namespace KSimLibInteger
#endif
