/***************************************************************************
                          ksimtype.cpp  -  description
                             -------------------
    begin                : Sun Dec 23 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimtype.h"
#include "ksimdata.h"

// Forward declaration



//#######################################################################################
//#######################################################################################




void KSimTypeBase::save(KSimData & config, const char * pKey) const
{
	config.writeEntry(pKey, text());
}

bool KSimTypeBase::load(KSimData & config, const char * pKey, const QString& aDefault)
{
	QString str = config.readEntry(pKey, aDefault);
	
	bool ok;
	
	setText(str, &ok);
	
	return ok;
}



//#######################################################################################
//#######################################################################################


// Create function QString KSimInt::text() const
MAKE_KSimTypeBase_text( KSimInt , QString::number )
// Create function void KSimInt::setText(const QString & text, bool * ok)
MAKE_KSimTypeBase_setText( KSimInt , text.toInt )

// Create function QString KSimUInt::text() const
MAKE_KSimTypeBase_text( KSimUInt , QString::number )
// Create function void KSimUInt::setText(const QString & text, bool * ok)
MAKE_KSimTypeBase_setText( KSimUInt , text.toUInt )


//#######################################################################################
//#######################################################################################






