/***************************************************************************
                          boolean.cpp  -  description
                             -------------------
    begin                : Son Mai  6 22:47:09 CEST 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
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

// KSimus-Includes
#include "ksimus/packageinfo.h"
#include "ksimus/ksimdebug.h"

// Project-Includes
#include "config.h"
#include "boolean.h"

// Component-Includes
#include "booleanor.h"
#include "booleanxor.h"
#include "rsflipflop.h"
#include "jkflipflop.h"
#include "dflipflop.h"
#include "monoflop.h"
#include "booleancounter.h"
#include "booleanconstant.h"
#include "boolean7segment.h"

// Forward declaration

// Project-Includes

namespace LIB_NAMESPACE
{



/************************************************************************************
 ************************************************************************************
 **
 **  Insert pointers to the ComponentInfo for each component you want to distribute.
 **
 ************************************************************************************
 ************************************************************************************/
static const ComponentInfoPtr distributeComponent[] =
{
	&BooleanOrInfo,
	&BooleanNorInfo,
	&BooleanXorInfo,
	&BooleanXnorInfo,
	&RSFlipFlopInfo,
	&JKFlipFlopInfo,
	&JKMSFlipFlopInfo,
	&DFlipFlopInfo,
	&MonoFlopInfo,
	&BooleanCounterInfo,
	&BooleanConstantTrue,
	&BooleanConstantFalse,
	&BooleanBoolean7SegmentInfo,
	(ComponentInfoPtr) 0          // Do not remove. Must be the last item.
};




/************************************************************************************
 ************************************************************************************
 **
 **  Insert pointers to the ConnectorInfo for each connector you want to distribute.
 **
 ************************************************************************************
 ************************************************************************************/
static const ConnectorInfoPtr distributeConnector[] =
{
	(ConnectorInfoPtr) 0          // Do not remove. Must be the last item.
};



/******************************************************************************************
 ******************************************************************************************
 **
 **  Insert pointers to the WirePropertyInfo for each wire property you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
static const WirePropertyInfoPtr distributeWireProperty[] =
{
	(WirePropertyInfoPtr) 0       // Do not remove. Must be the last item.
};



/******************************************************************************************
 ******************************************************************************************
 **
 **  No changes required below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

KInstance * instance = 0;
const PackageInfo * packageInfo = 0;

};  //namespace LIB_NAMESPACE



extern "C"
{
	const PackageInfo * PACKAGE_INIT_FUNCTION()
	{

		KSIMDEBUG("Init Package " PACKAGE_NAME);
			
		if (LIB_NAMESPACE::instance == 0)
		{
			LIB_NAMESPACE::instance = new KInstance(PACKAGE_LOWER_NAME);
		}
	
		if (LIB_NAMESPACE::packageInfo == 0)
		{
			LIB_NAMESPACE::packageInfo = new PackageInfo( PACKAGE_NAME,
			                                              LIB_NAMESPACE::instance,
	  		                                            VERSION,      // version from config.h
	    		                                          LIB_NAMESPACE::distributeComponent,
	      		                                        LIB_NAMESPACE::distributeConnector,
	        		                                      LIB_NAMESPACE::distributeWireProperty);
	  }
	

		return LIB_NAMESPACE::packageInfo;
	}
}

