/***************************************************************************
            implicitconverterbooltristate2bool.cpp  -  description
                             -------------------
    begin                : Mit Nov 13 2002
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
#include "implicitconverterbooltristate2bool.h"
#include "ksimus/connectorbase.h"
#include "ksimus/wirepropertybooltristate.h"

// Forward declaration

namespace KSimLibBoolean
{


//###############################################################################################
//###############################################################################################


ImplicitConverter * ImplicitConverterBoolTristate2Bool::create(ConnectorBase * connector)
{
	ImplicitConverter * ic;
	ic = new ImplicitConverterBoolTristate2Bool(connector);
	CHECK_PTR(ic);
	return ic;
}

const ImplicitConverterInfo * ImplicitConverterBoolTristate2Bool::getStaticInfo()
{
	static const ImplicitConverterInfo Info(QString::fromLatin1("Implicit Converter Boolean Tristate to Boolean"),
	                                        QString::fromLatin1("implicitconverter/BooleanTristate2Boolean"),
	                                        QString::fromLatin1("Boolean Tristate"),
	                                        QString::fromLatin1("Boolean"),
	                                        create );
	return &Info;
}

ImplicitConverterBoolTristate2Bool::ImplicitConverterBoolTristate2Bool(ConnectorBase * connector)
	: ImplicitConverter(connector, getStaticInfo())
{
}

/*ImplicitConverterBoolTristate2Bool::~ImplicitConverterBoolTristate2Bool()
{
} */

const void * ImplicitConverterBoolTristate2Bool::convert(const void * data)
{
	if (data && !((const WireStateBoolTristate *) data)->isFalse())
	{
		m_result = true;
	}
	else
	{
		m_result = false;
	}
	return &m_result;
}

//###############################################################

}; //namespace KSimLibBoolean

