/***************************************************************************
              implicitconverterbooltristate2bool.h  -  description
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

#ifndef IMPLICITCONVERTERBOOLTRISTATE2BOOL_H
#define IMPLICITCONVERTERBOOLTRISTATE2BOOL_H

// C includes

// QT includes

// KDE includes

// Project includes
#include "ksimus/implicitconverter.h"
#include "ksimus/implicitconverterinfo.h"

// Forward declaration


namespace KSimLibBoolean
{

const ImplicitConverterInfo * getImplicitConverterBoolTriState2BoolInfo();

class ImplicitConverterBoolTriState2Bool : public ImplicitConverter
{
   Q_OBJECT

public:
	ImplicitConverterBoolTriState2Bool(ConnectorBase * connector);
//	~ImplicitConverterBoolTriState2Bool();

	/** The converting function. Is called by the connector if converting is needed. */
	virtual const void * convert(const void * data);

private:
	bool m_result;
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
