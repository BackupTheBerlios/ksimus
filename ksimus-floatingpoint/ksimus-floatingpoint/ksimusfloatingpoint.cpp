/***************************************************************************
                          ksimusfloatingpoint.cpp  -  description
                             -------------------
    begin                : Fri Feb  1 18:30:45 CET 2002
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
#include <klocale.h>

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
#include "delay.h"
#include "random.h"

// Forward declaration

// Project-Includes

namespace KSimLibFloatingPoint
{

static KInstance * packageInstance = (KInstance *)0;
static PackageInfo * packageInfo = (PackageInfo *)0;
static bool isPackageInitialized = false;


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
//#define EXPORT_IMPICITCONVERTER_INFO_LIST 1
//#define EXPORT_IODEVICE_INFO_LIST 1
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
	static ComponentInfoList * pDistComponents = 0;

	if (pDistComponents == 0)
	{
		// Initialize
		pDistComponents = new ComponentInfoList;
		CHECK_PTR(pDistComponents);

		// Add your component info here
		pDistComponents->append(FloatAdd::getStaticInfo());
		pDistComponents->append(FloatSubtractor::getStaticInfo());
		pDistComponents->append(FloatMultiplier::getStaticInfo());
		pDistComponents->append(FloatDivider::getStaticInfo());
		pDistComponents->append(FloatInputSlider::getStaticInfo());
		pDistComponents->append(FloatLineOutput::getStaticInfo());
		pDistComponents->append(FloatRemainder::getStaticInfo());
		pDistComponents->append(FloatLineInput::getStaticInfo());
		pDistComponents->append(FloatConstInput::getStaticInfo());
		pDistComponents->append(FloatAbs::getStaticInfo());
		pDistComponents->append(WaveformGenerator::getStaticInfo());
		pDistComponents->append(SimpleConditional::getStaticLesserInfo());
		pDistComponents->append(SimpleConditional::getStaticLesserEqualInfo());
		pDistComponents->append(SimpleConditional::getStaticEqualInfo());
		pDistComponents->append(SimpleConditional::getStaticLargerEqualInfo());
		pDistComponents->append(SimpleConditional::getStaticLargerInfo());
		pDistComponents->append(ExtConnFloatOut::getStaticInfo());
		pDistComponents->append(ExtConnFloatIn::getStaticInfo());
		pDistComponents->append(ConvertFloatBool::getStaticInfo());
		pDistComponents->append(ConvertBoolFloat::getStaticInfo());
		pDistComponents->append(FloatLatch::getStaticInfo());
		pDistComponents->append(DataSelector::getStaticInfo());
		pDistComponents->append(FloatExp::getStaticInfo());
		pDistComponents->append(FloatPow::getStaticInfo());
		pDistComponents->append(FloatSqrt::getStaticInfo());
		pDistComponents->append(FloatLog::getStaticInfo());
		pDistComponents->append(FloatLog10::getStaticInfo());
		pDistComponents->append(FloatSin::getStaticInfo());
		pDistComponents->append(FloatASin::getStaticInfo());
		pDistComponents->append(FloatSinh::getStaticInfo());
		pDistComponents->append(FloatASinh::getStaticInfo());
		pDistComponents->append(FloatCos::getStaticInfo());
		pDistComponents->append(FloatACos::getStaticInfo());
		pDistComponents->append(FloatCosh::getStaticInfo());
		pDistComponents->append(FloatACosh::getStaticInfo());
		pDistComponents->append(FloatTan::getStaticInfo());
		pDistComponents->append(FloatATan::getStaticInfo());
		pDistComponents->append(FloatATan2::getStaticInfo());
		pDistComponents->append(FloatTanh::getStaticInfo());
		pDistComponents->append(FloatATanh::getStaticInfo());
		pDistComponents->append(FloatMax::getStaticInfo());
		pDistComponents->append(FloatMin::getStaticInfo());
		pDistComponents->append(Delay::getStaticInfo());
		pDistComponents->append(Random::getStaticInfo());
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
	static ConnectorInfoList * pDistConnector = 0;

	if (pDistConnector == 0)
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
	static WirePropertyInfoList * pDistWireProp = 0;

	if (pDistWireProp == 0)
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
	static ImplicitConverterInfoList * pImplicitConverter = 0;

	if (pImplicitConverter == 0)
	{
		// Initialize
		pImplicitConverter = new ImplicitConverterInfoList;
		CHECK_PTR(pImplicitConverter);

		// Add your implicit converter info here
	}

	return *pImplicitConverter;
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

	packageInfo = new PackageInfo( KSIMUS_PACKAGE_NAME,
	                               packageInstance,
	                               VERSION);      // version from config.h
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

};  //namespace KSimLibFloatingPoint


/******************************************************************************************
 ******************************************************************************************
 **
 **  Changes only the namespace below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

extern "C"
{
	const PackageInfo * init_ksimus_package(KLocale * ksimusLocale)
	{
		if (!KSimLibFloatingPoint::isPackageInitialized)
		{
			KSimLibFloatingPoint::initPackage(ksimusLocale);
			KSimLibFloatingPoint::isPackageInitialized = true;
		}

		return KSimLibFloatingPoint::packageInfo;
	}
}


