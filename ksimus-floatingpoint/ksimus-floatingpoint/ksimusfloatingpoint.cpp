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
#include "floatconstinput.h"
#include "floatabs.h"
#include "waveformgenerator.h"
#include "simpleconditional.h"
#include "extconnfloatin.h"
#include "extconnfloatout.h"
#include "convertfloatbool.h"
#include "convertboolfloat.h"
#include "floatlatch.h"
#include "dataselector.h"
#include "floatexp.h"
#include "floatlog.h"
#include "floatsin.h"
#include "floatcos.h"
#include "floattan.h"
#include "floatmax.h"
#include "floatmin.h"

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
static const ComponentInfoList & getDistComponents()
{
	static ComponentInfoList * pDistComponents = 0;

	if (pDistComponents == 0)
	{
		// Initialize
		pDistComponents = new ComponentInfoList;
		CHECK_PTR(pDistComponents);

		// Add your component info here
		pDistComponents->append(getFloatAddInfo());
		pDistComponents->append(getFloatSubtractorInfo());
		pDistComponents->append(getFloatMultiplierInfo());
		pDistComponents->append(getFloatDividerInfo());
		pDistComponents->append(getFloatInputSliderInfo());
		pDistComponents->append(getFloatLineOutputInfo());
		pDistComponents->append(getFloatRemainderInfo());
		pDistComponents->append(getFloatLineInputInfo());
		pDistComponents->append(getFloatConstInputInfo());
		pDistComponents->append(getFloatAbsInfo());
		pDistComponents->append(getWaveformGeneratorInfo());
		pDistComponents->append(getConditionalLesserInfo());
		pDistComponents->append(getConditionalLesserEqualInfo());
		pDistComponents->append(getConditionalEqualInfo());
		pDistComponents->append(getConditionalLargerEqualInfo());
		pDistComponents->append(getConditionalLargerInfo());
		pDistComponents->append(getExtConnFloatOutInfo());
		pDistComponents->append(getExtConnFloatInInfo());
		pDistComponents->append(getConvertFloatBoolInfo());
		pDistComponents->append(getConvertBoolFloatInfo());
		pDistComponents->append(getFloatLatchInfo());
		pDistComponents->append(getDataSelectorInfo());
		pDistComponents->append(getFloatExpInfo());
		pDistComponents->append(getFloatLogInfo());
		pDistComponents->append(getFloatSinInfo());
		pDistComponents->append(getFloatASinInfo());
		pDistComponents->append(getFloatCosInfo());
		pDistComponents->append(getFloatACosInfo());
		pDistComponents->append(getFloatTanInfo());
		pDistComponents->append(getFloatATanInfo());
		pDistComponents->append(getFloatMaxInfo());
		pDistComponents->append(getFloatMinInfo());
	}

	return *pDistComponents;
}




/************************************************************************************
 ************************************************************************************
 **
 **  Insert pointers to the ConnectorInfo for each connector you want to distribute.
 **
 ************************************************************************************
 ************************************************************************************/
static const ConnectorInfoList & getDistConnector()
{
	static ConnectorInfoList * pDistConnector = 0;

	if (pDistConnector == 0)
	{
		// Initialize
		pDistConnector = new ConnectorInfoList;
		CHECK_PTR(pDistConnector);

		// Add your connector info here
//		pDistConnector->append(getConnectorBoolInInfo());
	}
	return *pDistConnector;
}



/******************************************************************************************
 ******************************************************************************************
 **
 **  Insert pointers to the WirePropertyInfo for each wire property you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
static const WirePropertyInfoList & getDistWireProperty()
{
	static WirePropertyInfoList * pDistWireProp = 0;

	if (pDistWireProp == 0)
	{
		// Initialize
		pDistWireProp = new WirePropertyInfoList;
		CHECK_PTR(pDistWireProp);

		// Add your wireproperty info here
//		pDistWireProp->append(getWirePropertyBooleanInfo());
	}

	return *pDistWireProp;
}



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
			                                              KSimLibFloatingPoint::getDistComponents(),
			                                              KSimLibFloatingPoint::getDistConnector(),
			                                              KSimLibFloatingPoint::getDistWireProperty());
	  }
	

		return KSimLibFloatingPoint::packageInfo;
	}
}
