/***************************************************************************
                          boolean.cpp  -  description
                             -------------------
    begin                : Sun May  6 22:47:09 CEST 2001
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
#include <kinstance.h>

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


#include "iodevicevirtualbool.h"


// Forward declaration

// Project-Includes




namespace KSimLibBoolean
{

static KInstance * packageInstance = (KInstance *)0;
static PackageInfo * packageInfo = (PackageInfo *)0;
static bool isPackageInitialized = false;


KInstance * getPackageInstance()
{
	return packageInstance;
}

const PackageInfo * getPackageInfo()
{
	return packageInfo;
}


/************************************************************************************
 ************************************************************************************
 **
 **  Comment out one or more of the following defines if you dont want to
 **  distribute this item type.
 **
 ************************************************************************************
 ************************************************************************************/
#define EXPORT_COMPONENT_INFO_LIST 1
//#define EXPORT_CONNECTOR_INFO_LIST 1
//#define EXPORT_WIREPROPERTY_INFO_LIST 1
#define EXPORT_IMPICITCONVERTER_INFO_LIST 1
#define EXPORT_IODEVICE_INFO_LIST 1
//#define EXPORT_IOJOIN_INFO_LIST 1



/************************************************************************************
 ************************************************************************************
 **
 **  Insert pointers to the ComponentInfo for each component you want to distribute.
 **
 ************************************************************************************
 ************************************************************************************/
#ifdef EXPORT_COMPONENT_INFO_LIST
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
#endif // EXPORT_COMPONENT_INFO_LIST



/************************************************************************************
 ************************************************************************************
 **
 **  Insert pointers to the ConnectorInfo for each connector you want to distribute.
 **
 ************************************************************************************
 ************************************************************************************/
#ifdef EXPORT_CONNECTOR_INFO_LIST
static const ConnectorInfoList & getDistConnector()
{
	static ConnectorInfoList * pDistConnector = (ConnectorInfoList *)0;
	
	if (pDistConnector == (ConnectorInfoList *)0)
	{
		// Initialize
		pDistConnector = new ConnectorInfoList;
		CHECK_PTR(pDistConnector);

		// Add your connector info here
	}

	return *pDistConnector;
}
#endif // EXPORT_CONNECTOR_INFO_LIST



/******************************************************************************************
 ******************************************************************************************
 **
 **  Insert pointers to the WirePropertyInfo for each wire property you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
#ifdef EXPORT_WIREPROPERTY_INFO_LIST
static const WirePropertyInfoList & getDistWireProperty()
{
	static WirePropertyInfoList * pDistWireProp = (WirePropertyInfoList *)0;

	if (pDistWireProp == (WirePropertyInfoList *)0)
	{
		// Initialize
		pDistWireProp = new WirePropertyInfoList;
		CHECK_PTR(pDistWireProp);

		// Add your wireproperty info here
	}

	return *pDistWireProp;
}
#endif // EXPORT_WIREPROPERTY_INFO_LIST



/******************************************************************************************
 ******************************************************************************************
 **
 **  Insert pointers to the ImplicitConverterInfo for each implicit converter you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
#ifdef EXPORT_IMPICITCONVERTER_INFO_LIST
static const ImplicitConverterInfoList & getDistImplicitConverter()
{
	static ImplicitConverterInfoList * pDistImplicitConverter = (ImplicitConverterInfoList *)0;

	if (pDistImplicitConverter == (ImplicitConverterInfoList *)0)
	{
		// Initialize
		pDistImplicitConverter = new ImplicitConverterInfoList;
		CHECK_PTR(pDistImplicitConverter);

		// Add your implicit converter info here
		pDistImplicitConverter->append(ImplicitConverterBoolTristate2Bool::getStaticInfo());
	}

	return *pDistImplicitConverter;
}
#endif // EXPORT_IMPICITCONVERTER_INFO_LIST




/******************************************************************************************
 ******************************************************************************************
 **
 **  Insert pointers to the KSimIoDeviceInfo for each io device type you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
#ifdef EXPORT_IODEVICE_INFO_LIST
static const KSimIoDeviceInfoList & getDistIoDevice()
{
	static KSimIoDeviceInfoList * pDistIoDevice = (KSimIoDeviceInfoList *)0;

	if (pDistIoDevice == (KSimIoDeviceInfoList *)0)
	{
		// Initialize
		pDistIoDevice = new KSimIoDeviceInfoList;
		CHECK_PTR(pDistIoDevice);

		// Add your io device info here
		pDistIoDevice->append(IoDeviceVirtualBool::getStaticInfo());
	}

	return *pDistIoDevice;
}
#endif // EXPORT_IODEVICE_INFO_LIST



/******************************************************************************************
 ******************************************************************************************
 **
 **  Insert pointers to the KSimIoJoinInfo for each io join type you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
#ifdef EXPORT_IOJOIN_INFO_LIST
static const KSimIoJoinInfoList & getDistIoJoin()
{
	static KSimIoJoinInfoList * pDistIoJoin = (KSimIoJoinInfoList *)0;

	if (pDistIoJoin == (KSimIoJoinInfoList *)0)
	{
		// Initialize
		pDistIoJoin = new KSimIoJoinInfoList;
		CHECK_PTR(pDistIoJoin);

		// Add your io join info here
	}

	return *pDistIoJoin;
}
#endif // EXPORT_IOJOIN_INFO_LIST



/******************************************************************************************
 ******************************************************************************************
 **
 **  No changes required inside the next function
 **
 ******************************************************************************************
 ******************************************************************************************/

static void initPackage(KLocale * ksimusLocale)
{

//	KSIMDEBUG("Init Package " KSIMUS_PACKAGE_NAME);
	ASSERT(packageInstance == (KInstance *)0);
	ASSERT(packageInfo == (const PackageInfo *)0);
	ASSERT(ksimusLocale != (KLocale *)0);

	packageInstance = new KInstance(KSIMUS_PACKAGE_LOWER_NAME);
	CHECK_PTR(packageInstance);
	// add translation
	ksimusLocale->insertCatalogue(packageInstance->instanceName());

	packageInfo = new PackageInfo( (const char *)KSIMUS_PACKAGE_NAME,
	                               packageInstance,
	                               (const char *)VERSION);      // version from config.h
	CHECK_PTR(packageInfo);

	// Add info lists
	#ifdef EXPORT_COMPONENT_INFO_LIST
		packageInfo->insert(getDistComponents());
	#endif

	#ifdef EXPORT_CONNECTOR_INFO_LIST
		packageInfo->insert(getDistConnector());
	#endif

	#ifdef EXPORT_WIREPROPERTY_INFO_LIST
		packageInfo->insert(getDistWireProperty());
	#endif

	#ifdef EXPORT_IMPICITCONVERTER_INFO_LIST
		packageInfo->insert(getDistImplicitConverter());
	#endif

	#ifdef EXPORT_IODEVICE_INFO_LIST
		packageInfo->insert(getDistIoDevice());
	#endif

	#ifdef EXPORT_IOJOIN_INFO_LIST
		packageInfo->insert(getDistIoJoin());
	#endif

}


// Remove defines
#ifdef EXPORT_COMPONENT_INFO_LIST
	#undef EXPORT_COMPONENT_INFO_LIST
#endif

#ifdef EXPORT_CONNECTOR_INFO_LIST
	#undef EXPORT_CONNECTOR_INFO_LIST
#endif

#ifdef EXPORT_WIREPROPERTY_INFO_LIST
	#undef EXPORT_WIREPROPERTY_INFO_LIST
#endif

#ifdef EXPORT_IMPICITCONVERTER_INFO_LIST
	#undef EXPORT_IMPICITCONVERTER_INFO_LIST
#endif

#ifdef EXPORT_IODEVICE_INFO_LIST
	#undef EXPORT_IODEVICE_INFO_LIST
#endif

#ifdef EXPORT_IOJOIN_INFO_LIST
	#undef EXPORT_IOJOIN_INFO_LIST
#endif

};  //namespace KSimLibBoolean


/******************************************************************************************
 ******************************************************************************************
 **
 **  Changes only the namespace below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

extern "C"
{
	const PackageInfo * KSIMUS_PACKAGE_INIT_FUNCTION(KLocale * ksimusLocale)
	{
		if (!KSimLibBoolean::isPackageInitialized)
		{
			KSimLibBoolean::initPackage(ksimusLocale);
			KSimLibBoolean::isPackageInitialized = true;
		}

		return KSimLibBoolean::packageInfo;
	}
}





