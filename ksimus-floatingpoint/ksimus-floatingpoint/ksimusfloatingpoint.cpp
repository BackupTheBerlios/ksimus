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
 **  Insert pointers to the ImplicitConverterInfo for each implicit converter you want to distribute.
 **
 ******************************************************************************************
 ******************************************************************************************/
static const ImplicitConverterInfoList & getImplicitConverterProperty()
{
	static ImplicitConverterInfoList * pImplicitConverterProp = 0;

	if (pImplicitConverterProp == 0)
	{
		// Initialize
		pImplicitConverterProp = new ImplicitConverterInfoList;
		CHECK_PTR(pImplicitConverterProp);

		// Add your implicit converter info here
	}

	return *pImplicitConverterProp;
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
	const PackageInfo * KSIMUS_PACKAGE_INIT_FUNCTION(KLocale * ksimusLocale)
	{

//		KSIMDEBUG("Init Package " KSIMUS_PACKAGE_NAME);
			
		if (KSimLibFloatingPoint::instance == 0)
		{
			KSimLibFloatingPoint::instance = new KInstance(KSIMUS_PACKAGE_LOWER_NAME);
			CHECK_PTR(KSimLibFloatingPoint::instance);
			CHECK_PTR(ksimusLocale);
			// add translation
			ksimusLocale->insertCatalogue(KSimLibFloatingPoint::instance->instanceName());
		}
	
		if (KSimLibFloatingPoint::packageInfo == 0)
		{
			KSimLibFloatingPoint::packageInfo = new PackageInfo( KSIMUS_PACKAGE_NAME,
			                                              KSimLibFloatingPoint::instance,
			                                              VERSION,      // version from config.h
			                                              KSimLibFloatingPoint::getDistComponents(),
			                                              KSimLibFloatingPoint::getDistConnector(),
			                                              KSimLibFloatingPoint::getDistWireProperty(),
			                                              KSimLibFloatingPoint::getImplicitConverterProperty());
	  }
	

		return KSimLibFloatingPoint::packageInfo;
	}
}

