/***************************************************************************
                          packageinteger.cpp  -  description
                             -------------------
    begin                : Sat Jul 12 2003
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

// C-Includes

// QT-Includes

// KDE-Includes
#include <klocale.h>

// KSimus-Includes
#include "ksimus/packageinfo.h"
#include "ksimus/ksimdebug.h"

// Project-Includes
#include "config.h"
#include "packageinteger.h"

// Component-Includes
#include "integerlineinput.h"
#include "integerconstinput.h"
#include "integerlineoutput.h"
#include "integerspinbox.h"
#include "integeradd.h"
#include "integersubtractor.h"
#include "integermultiplier.h"
#include "integerdivider.h"
#include "integerremainder.h"
#include "integerabs.h"
#include "integerinverterarithmetic.h"
#include "integerdataselector.h"
#include "integerdelay.h"
#include "integerinverter.h"
#include "integerand.h"
#include "integeror.h"
#include "integerxor.h"
#include "extconnintegerin.h"
#include "extconnintegerout.h"
#include "integerconditional.h"
#include "integermin.h"
#include "integermax.h"
#include "integercounter.h"
#include "integerlatch.h"
#include "convertboolinteger.h"
#include "convertintegerbool.h"
#include "convertfloatinteger.h"
#include "convertintegerfloat.h"
#include "multibool2integer.h"
#include "integer2multibool.h"




#include "integerimplicitconverter.h"

// Forward declaration

// Project-Includes

namespace KSimLibInteger
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
	static ComponentInfoList * pDistComponents = (ComponentInfoList *)0;

	if (pDistComponents == (ComponentInfoList *)0)
	{
		// Initialize
		pDistComponents = new ComponentInfoList;
		CHECK_PTR(pDistComponents);

		// Add your component info here
		pDistComponents->append(IntegerLineInput::getStaticInfo());
		pDistComponents->append(IntegerConstInput::getStaticInfo());
		pDistComponents->append(IntegerLineOutput::getStaticInfo());
		pDistComponents->append(IntegerSpinbox::getStaticInfo());
		pDistComponents->append(IntegerAdd::getStaticInfo());
		pDistComponents->append(IntegerSubtractor::getStaticInfo());
		pDistComponents->append(IntegerMultiplier::getStaticInfo());
		pDistComponents->append(IntegerDivider::getStaticInfo());
		pDistComponents->append(IntegerRemainder::getStaticInfo());
		pDistComponents->append(IntegerAbs::getStaticInfo());
		pDistComponents->append(IntegerArithmeticInverter::getStaticInfo());
		pDistComponents->append(IntegerDataSelector::getStaticInfo());
		pDistComponents->append(IntegerDelay::getStaticInfo());

		pDistComponents->append(IntegerInverter::getStaticInfo());
		pDistComponents->append(IntegerAnd::getStaticInfo());
		pDistComponents->append(IntegerNand::getStaticInfo());
		pDistComponents->append(IntegerOr::getStaticInfo());
		pDistComponents->append(IntegerNor::getStaticInfo());
		pDistComponents->append(IntegerXor::getStaticInfo());
		pDistComponents->append(IntegerXnor::getStaticInfo());

		pDistComponents->append(IntegerCounterClkUpClkDown::getStaticInfo());
		pDistComponents->append(IntegerCounterClkUpClkDownLoad::getStaticInfo());
		pDistComponents->append(IntegerCounterClkDir::getStaticInfo());
		pDistComponents->append(IntegerCounterClkDirLoad::getStaticInfo());

		pDistComponents->append(IntegerConditional::getStaticLesserInfo());
		pDistComponents->append(IntegerConditional::getStaticLesserEqualInfo());
		pDistComponents->append(IntegerConditional::getStaticEqualInfo());
		pDistComponents->append(IntegerConditional::getStaticLargerEqualInfo());
		pDistComponents->append(IntegerConditional::getStaticLargerInfo());
		pDistComponents->append(IntegerConditional::getStaticNotEqualInfo());
		pDistComponents->append(IntegerMin::getStaticInfo());
		pDistComponents->append(IntegerMax::getStaticInfo());
		pDistComponents->append(IntegerLatch::getStaticInfo());

		pDistComponents->append(ExtConnIntegerIn::getStaticInfo());
		pDistComponents->append(ExtConnIntegerOut::getStaticInfo());


		pDistComponents->append(ConvertBoolInteger::getStaticInfo());
		pDistComponents->append(ConvertIntegerBool::getStaticInfo());
		pDistComponents->append(ConvertFloatInteger::getStaticInfo());
		pDistComponents->append(ConvertIntegerFloat::getStaticInfo());
		pDistComponents->append(MultiBool2Integer::getStaticInfo());
		pDistComponents->append(Integer2MultiBool::getStaticInfo());
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
		pDistImplicitConverter->append(ImplicitConverterInteger2Boolean::getStaticInfo());
		pDistImplicitConverter->append(ImplicitConverterBoolean2Integer::getStaticInfo());
		pDistImplicitConverter->append(ImplicitConverterInteger2Float::getStaticInfo());
		pDistImplicitConverter->append(ImplicitConverterFloat2Integer::getStaticInfo());
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

};  //namespace KSimLibInteger


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
		if (!KSimLibInteger::isPackageInitialized)
		{
			KSimLibInteger::initPackage(ksimusLocale);
			KSimLibInteger::isPackageInitialized = true;
		}

		return KSimLibInteger::packageInfo;
	}
}

