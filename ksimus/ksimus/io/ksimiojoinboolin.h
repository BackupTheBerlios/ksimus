/***************************************************************************
                          ksimiojoinboolin.h  -  description
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

#ifndef KSIMIOJOINBOOLIN_H
#define KSIMIOJOINBOOLIN_H

// C/C++ includes

// QT includes

// KDE includes

// Project includes
#include "ksimiojoin.h"

// Forward declaration
class KSimIoJoinInfo;

/**A boolean input join.
  *@author Rasmus Diekenbrock
  */

class KSimIoJoinBoolIn : public KSimIoJoin
{
   Q_OBJECT
public: 
	KSimIoJoinBoolIn(KSimIoComponent * comp, const KSimIoJoinInfo * info);
	~KSimIoJoinBoolIn();

	virtual ConnectorBase * createConnector();

	/** executes the join */
	virtual void calculate() const;

	static const KSimIoJoinInfo * getStaticInfo();
	static KSimIoJoin * create(KSimIoComponent * comp, const KSimIoJoinInfo * info);

private:
	
};

//###################################################################################
//###################################################################################
// TODO move to own file

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
