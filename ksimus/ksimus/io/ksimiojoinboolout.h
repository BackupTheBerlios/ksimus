/***************************************************************************
                          ksimiojoinboolout.h  -  description
                             -------------------
    begin                : Sun May 18 2003
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

#ifndef KSIMIOJOINBOOLOUT_H
#define KSIMIOJOINBOOLOUT_H

// C/C++ includes

// QT includes

// KDE includes

// Project includes
#include "ksimiojoin.h"

// Forward declaration
class KSimIoJoinInfo;


/**A boolean output join.
  *
  * Remember: This is an output for KSimus and an input for the io device!!!
  *
  *@author Rasmus Diekenbrock
  */

class KSimIoJoinBoolOut : public KSimIoJoin
{
   Q_OBJECT
public:
	KSimIoJoinBoolOut(KSimIoComponent * comp, const KSimIoJoinInfo * info);
	~KSimIoJoinBoolOut();

	virtual ConnectorBase * createConnector();

	/** executes the join */
	virtual void calculate() const;

	static const KSimIoJoinInfo * getStaticInfo();
	static KSimIoJoin * create(KSimIoComponent * comp, const KSimIoJoinInfo * info);

private:

};
#endif
