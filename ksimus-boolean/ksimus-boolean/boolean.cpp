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
#include <klocale.h>

// KSimus-Includes
#include "ksimus/packageinfo.h"
#include "ksimus/ksimdebug.h"

// Project-Includes
#include "config.h"
#include "boolean.h"

// Component-Includes
#include "inverter.h"
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
#include "convertbool2booltristate.h"
#include "convertbooltristate2bool.h"
#include "extconnbooltristate.h"
#include "implicitconverterbooltristate2bool.h"
#include "booltristateand.h"
#include "booltristateor.h"
#include "booltristatexor.h"

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
	static ComponentInfoList * pDistComponents = (ComponentInfoList *)0;

	if (pDistComponents == (ComponentInfoList *)0)
	{
		// Initialize
		pDistComponents = new ComponentInfoList;
		CHECK_PTR(pDistComponents);

		// Add your component info here
		pDistComponents->append(Inverter::getStaticIdentityInfo());
		pDistComponents->append(Inverter::getStaticInverterInfo());
		pDistComponents->append(BooleanOr::getStaticOrInfo());
		pDistComponents->append(BooleanOr::getStaticNorInfo());
		pDistComponents->append(BooleanXor::getStaticXorInfo());
		pDistComponents->append(BooleanXor::getStaticXnorInfo());
		pDistComponents->append(RSFlipFlop::getStaticInfo());
		pDistComponents->append(JKFlipFlop::getStaticJKInfo());
		pDistComponents->append(JKFlipFlop::getStaticJKMSInfo());
		pDistComponents->append(DFlipFlop::getStaticInfo());
		pDistComponents->append(MonoFlop::getStaticInfo());
		pDistComponents->append(BooleanCounter::getStaticInfo());
		pDistComponents->append(BooleanConstant::getStaticTrueInfo());
		pDistComponents->append(BooleanConstant::getStaticFalseInfo());
		pDistComponents->append(Boolean7Segment::getStaticInfo());
		pDistComponents->append(MultiDLatch::getStaticInfo());
		pDistComponents->append(Multiplexer::getStaticInfo());
		pDistComponents->append(Demultiplexer::getStaticInfo());
		pDistComponents->append(Delay::getStaticInfo());

		// Tristate
		pDistComponents->append(ConvertBool2BoolTristate::getStaticInfo());
		pDistComponents->append(ConvertBoolTristate2Bool::getStaticInfo());
		pDistComponents->append(ExtConnBoolTristateIn::getStaticInfo());
		pDistComponents->append(ExtConnBoolTristateOut::getStaticInfo());
		pDistComponents->append(BoolTristateAnd::getStaticAndInfo());
		pDistComponents->append(BoolTristateAnd::getStaticNandInfo());
		pDistComponents->append(BoolTristateOr::getStaticOrInfo());
		pDistComponents->append(BoolTristateOr::getStaticNorInfo());
		pDistComponents->append(BoolTristateXor::getStaticXorInfo());
		pDistComponents->append(BoolTristateXor::getStaticXnorInfo());
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
	static ConnectorInfoList * pDistConnector = (ConnectorInfoList *)0;
	
	if (pDistConnector == (ConnectorInfoList *)0)
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
	static WirePropertyInfoList * pDistWireProp = (WirePropertyInfoList *)0;

	if (pDistWireProp == (WirePropertyInfoList *)0)
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
 **  Insert pointers to the ImplicitConverterInfo for each implicit converter you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
static const ImplicitConverterInfoList & getDistImplicitConverterProperty()
{
	static ImplicitConverterInfoList * pDistImplicitConverterProp = (ImplicitConverterInfoList *)0;

	if (pDistImplicitConverterProp == (ImplicitConverterInfoList *)0)
	{
		// Initialize
		pDistImplicitConverterProp = new ImplicitConverterInfoList;
		CHECK_PTR(pDistImplicitConverterProp);

		// Add your implicit converter info here
		pDistImplicitConverterProp->append(ImplicitConverterBoolTristate2Bool::getStaticInfo());
	}

	return *pDistImplicitConverterProp;
}



/******************************************************************************************
 ******************************************************************************************
 **
 **  Changes only the namespace below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

KInstance * instance = (KInstance *)0;
const PackageInfo * packageInfo = (const PackageInfo *)0;

};  //namespace KSimLibBoolean



extern "C"
{
	const PackageInfo * KSIMUS_PACKAGE_INIT_FUNCTION(KLocale * ksimusLocale)
	{

//		KSIMDEBUG("Init Package " KSIMUS_PACKAGE_NAME);
			
		if (KSimLibBoolean::instance == (KInstance *)0)
		{
			KSimLibBoolean::instance = new KInstance(KSIMUS_PACKAGE_LOWER_NAME);
			CHECK_PTR(KSimLibBoolean::instance);
			CHECK_PTR(ksimusLocale);
			// add translation
			ksimusLocale->insertCatalogue(KSimLibBoolean::instance->instanceName());
		}
	
		if (KSimLibBoolean::packageInfo == (const PackageInfo *)0)
		{
			KSimLibBoolean::packageInfo = new PackageInfo( KSIMUS_PACKAGE_NAME,
			                                               KSimLibBoolean::instance,
			                                               VERSION,      // version from config.h
			                                               KSimLibBoolean::getDistComponents(),
			                                               KSimLibBoolean::getDistConnector(),
			                                               KSimLibBoolean::getDistWireProperty(),
			                                               KSimLibBoolean::getDistImplicitConverterProperty());
		}
	

		return KSimLibBoolean::packageInfo;
	}
}

