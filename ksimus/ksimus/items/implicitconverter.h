/***************************************************************************
                          implicitconverter.h  -  description
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

#ifndef IMPLICITCONVERTER_H
#define IMPLICITCONVERTER_H

// C includes

// QT includes
#include <qobject.h>

// KDE includes

// Project includes

// Forward declaration
class ConnectorBase;
class ImplicitConverterInfo;

/** Virtual base class for implizit data type converting.
  *@author Rasmus Diekenbrock
  */

class ImplicitConverter : public QObject
{
class Private;

   Q_OBJECT

public:
	~ImplicitConverter();

	/** The converting function. Is called by the connector if converting is needed. */
	virtual const void * convert(const void * data) = 0;

	/** Returns the associated ImplicitConverterInfo. */
	const ImplicitConverterInfo * getInfo() const;

protected:
	ImplicitConverter(ConnectorBase * connector, const ImplicitConverterInfo * implicitConverterInfo);

private:
	Private * m_p;
};


//###############################################################################################
//###############################################################################################


const ImplicitConverterInfo * getImplicitConverterBoolean2FloatInfo();

class ImplicitConverterBoolean2Float : public ImplicitConverter
{
   Q_OBJECT

public:
	ImplicitConverterBoolean2Float(ConnectorBase * connector);
	~ImplicitConverterBoolean2Float();

	/** The converting function. Is called by the connector if converting is needed. */
	virtual const void * convert(const void * data);

private:
	double m_result;
};



#endif
