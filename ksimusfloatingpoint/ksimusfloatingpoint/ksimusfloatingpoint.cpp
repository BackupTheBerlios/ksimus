/***************************************************************************
                          ksimusfloatingpoint.cpp  -  description
                             -------------------
    begin                : Fre Feb  1 18:30:45 CET 2002
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

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus-Includes
#include "ksimus/packageinfo.h"
#include "ksimus/ksimdebug.h"

// Project-Includes
#include "config.h"
#include "ksimusfloatingpoint.h"

// Component-Includes
#include "floatadd.h"
#include "floatsubtractor.h"
#include "floatmultiplier.h"
#include "floatdivider.h"
#include "floatremainder.h"
#include "floatinputslider.h"
#include "floatlineoutput.h"
#include "floatlineinput.h"

// Forward declaration

// Project-Includes

namespace KSimLibFloatingPoint
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
	&FloatAddInfo,
	&FloatSubtractorInfo,
	&FloatMultiplierInfo,
	&FloatDividerInfo,
	&FloatInputSliderInfo,
	&FloatLineOutputInfo,
	&FloatRemainderInfo,
	&FloatLineInputInfo,
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

};  //namespace KSimLibFloatingPoint



extern "C"
{
	const PackageInfo * PACKAGE_INIT_FUNCTION()
	{

		KSIMDEBUG("Init Package " PACKAGE_NAME);
			
		if (KSimLibFloatingPoint::instance == 0)
		{
			KSimLibFloatingPoint::instance = new KInstance(PACKAGE_LOWER_NAME);
		}
	
		if (KSimLibFloatingPoint::packageInfo == 0)
		{
			KSimLibFloatingPoint::packageInfo = new PackageInfo( PACKAGE_NAME,
			                                              KSimLibFloatingPoint::instance,
	  		                                            VERSION,      // version from config.h
	    		                                          KSimLibFloatingPoint::distributeComponent,
	      		                                        KSimLibFloatingPoint::distributeConnector,
	        		                                      KSimLibFloatingPoint::distributeWireProperty);
	  }
	

		return KSimLibFloatingPoint::packageInfo;
	}
}

