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
#include "multidlatch.h"
#include "multiplexer.h"
#include "demultiplexer.h"
#include "delay.h"

// Forward declaration

// Project-Includes

namespace KSimLibBoolean
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
		pDistComponents->append(getBooleanOrInfo());
		pDistComponents->append(getBooleanNorInfo());
		pDistComponents->append(getBooleanXorInfo());
		pDistComponents->append(getBooleanXnorInfo());
		pDistComponents->append(getRSFlipFlopInfo());
		pDistComponents->append(getJKFlipFlopInfo());
		pDistComponents->append(getJKMSFlipFlopInfo());
		pDistComponents->append(getDFlipFlopInfo());
		pDistComponents->append(getMonoFlopInfo());
		pDistComponents->append(getBooleanCounterInfo());
		pDistComponents->append(getBooleanConstantTrue());
		pDistComponents->append(getBooleanConstantFalse());
		pDistComponents->append(getBoolean7SegmentInfo());
		pDistComponents->append(getMultiDLatchInfo());
		pDistComponents->append(getMultiplexerInfo());
		pDistComponents->append(getDemultiplexerInfo());
		pDistComponents->append(getDelayInfo());
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
 **  Changes only the namespace below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

KInstance * instance = 0;
const PackageInfo * packageInfo = 0;

};  //namespace KSimLibBoolean



extern "C"
{
	const PackageInfo * PACKAGE_INIT_FUNCTION()
	{

		KSIMDEBUG("Init Package " PACKAGE_NAME);
			
		if (KSimLibBoolean::instance == 0)
		{
			KSimLibBoolean::instance = new KInstance(PACKAGE_LOWER_NAME);
		}
	
		if (KSimLibBoolean::packageInfo == 0)
		{
			KSimLibBoolean::packageInfo = new PackageInfo( PACKAGE_NAME,
			                                               KSimLibBoolean::instance,
			                                               VERSION,      // version from config.h
			                                               KSimLibBoolean::getDistComponents(),
			                                               KSimLibBoolean::getDistConnector(),
			                                               KSimLibBoolean::getDistWireProperty());
		}
	

		return KSimLibBoolean::packageInfo;
	}
}

