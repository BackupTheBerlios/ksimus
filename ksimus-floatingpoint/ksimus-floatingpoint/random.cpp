/***************************************************************************
                          random.cpp  -  description
                             -------------------
    begin                : Tue Sep 3 2002
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
#include <limits.h>

// QT-Includes
#include <qpainter.h>
#include <qlabel.h>


// KDE-Includes
#include <kapp.h>
#include <klocale.h>

// KSimus-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/ksimbooleanbox.h"
#include "ksimus/ksimdoubleedit.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/optionalconnector.h"
#include "ksimus/ksimembfont.h"


// Project-Includes
#include "random.h"

// Forward declaration



namespace KSimLibFloatingPoint
{


Component * Random::create(CompContainer * container, const ComponentInfo * ci)
{
	return new Random(container, ci);
}

const ComponentInfo * Random::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Random"),
	                                QString::fromLatin1("Floating Point/Random/Random"),
	                                i18n("Component", "Floating Point/Random/Random"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-random-random"));
	return &Info;
}


//###############################################################
//###############################################################

#define DEFAULT_USE_SEED    false
#define DEFAULT_SEED        1
#define DEFAULT_MIN         0.0
#define DEFAULT_MAX         1.0
#define MIN_SEED            1
#define MAX_SEED            LONG_MAX

const char * const Random::sUseSeed   = "use seed";
const char * const Random::sSeed      = "seed";
const char * const Random::sMin       = "min";
const char * const Random::sMax       = "max";


Random::Random(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_randomGen(),
		m_seed(DEFAULT_SEED),
		m_useSeed(DEFAULT_USE_SEED),
		m_min(DEFAULT_MIN),
		m_max(DEFAULT_MAX)
{

	m_out = new ConnectorFloatOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("FloatingPoint-Connector", "Output"));
	CHECK_PTR(m_out);

	m_enable = new ConnectorBoolInEdge(this,
	                                   QString::fromLatin1("Enable"),
	                                   i18n("FloatingPoint-Connector", "Enable"),
	                                   QString::fromLatin1("E"));
	CHECK_PTR(m_enable);
	// make Enable optional
	new OptionalConnector(m_enable,
	                      QString::fromLatin1("Enable input"),
	                      i18n("FloatingPoint", "Enable input:"));

	// Initializes the sheet view
	if (getSheetMap())
	{
		new RandomView(this, SHEET_VIEW);
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
}
	
/*Random::~Random()
{
}*/

void Random::calculate()
{
	Component::calculate();
	
	bool ena = getEnableConnector()->isHidden() || getEnableConnector()->getInput();

	if (ena)
	{
		m_out->setOutput(m_randomGen.getDouble() * m_gain + m_offset);
		executeNext();
	}
}

/** Reset all simulation variables */
void Random::reset()
{
	Component::reset();
	
	if (m_useSeed)
	{
		m_randomGen.setSeed(m_seed);
	}
	else
	{
		m_randomGen.setSeed(KApplication::random());
	}

	m_gain = m_max - m_min;
	m_offset = m_min;
	
	m_out->setOutput(m_randomGen.getDouble() * m_gain + m_offset);
}

/** save component properties */
void Random::save(KSimData & file) const
{
	Component::save(file);
	
	file.writeEntry(sUseSeed, m_useSeed);
	file.writeEntry(sSeed, m_seed);
	file.writeEntry(sMin, m_min);
	file.writeEntry(sMax, m_max);
}

bool Random::load(KSimData & file, bool copyLoad)
{
	bool ok;
	
	ok = Component::load(file, copyLoad);
	
	m_useSeed = file.readBoolEntry(sUseSeed, DEFAULT_USE_SEED);
	m_seed = file.readLongNumEntry(sSeed, DEFAULT_SEED);
	m_min = file.readDoubleNumEntry(sMin, DEFAULT_MIN);
	m_max = file.readDoubleNumEntry(sMax, DEFAULT_MAX);
	
	return ok;
}

ComponentPropertyBaseWidget * Random::createGeneralProperty(QWidget *parent)
{
	RandomPropertyGeneralWidget * wid;
	wid = new RandomPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################

RandomView::RandomView(Random * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getRandom()->getEnableConnector()->setGridPos(0,2);
	getRandom()->getOutputConnector()->setGridPos(4,2);
}

/*RandomView::~RandomView()
{
}*/

void RandomView::draw(QPainter * p)
{
	drawFrame(p);
	
	if (getRandom()->getEnableConnector()->isHidden())
	{
		KSimEmbFont::getFont10()->drawText(p, getDrawingPlace(), AlignCenter, "Rnd");
	}
	else
	{
		QRect place = getDrawingPlace();
		place.rTop() += 4;
		KSimEmbFont::getFont10()->drawText(p, place, AlignTop | AlignHCenter, "Rnd");
	}
	
	CompView::draw(p);
}

//###############################################################
//###############################################################



RandomPropertyGeneralWidget::RandomPropertyGeneralWidget(Random * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;

	/********  Maximum  ********/
	lab = new QLabel(i18n("FloatingPoint", "Maximum:"), this);
	CHECK_PTR(lab);
	m_max = new KSimDoubleEdit(this, "RndF-Maximum");
	CHECK_PTR(m_max);
	lab->setBuddy(m_max);
	
	str = i18n("FloatingPoint", "Set here the maximum value (exclusive).");
	addToolTip(str, m_max, lab);
	addWhatsThis(str, m_max, lab);
	
	/********  Minimum  ********/
	lab = new QLabel(i18n("FloatingPoint", "Minimum:"), this);
	CHECK_PTR(lab);
	m_min = new KSimDoubleEdit(this, "RndF-Minimum");
	CHECK_PTR(m_min);
	lab->setBuddy(m_min);

	str = i18n("FloatingPoint", "Set here the minimum value (inclusive).");
	addToolTip(str, m_min, lab);
	addWhatsThis(str, m_min, lab);

	/********  Use individual seed  ********/
	lab = new QLabel(i18n("FloatingPoint", "Seed:"), this);
	CHECK_PTR(lab);
	m_useSeed = new KSimBooleanBox(this, "Seed-Use");
	CHECK_PTR(m_useSeed);
	lab->setBuddy(m_useSeed);
	m_useSeed->setFalseText(i18n("FloatingPoint - Random", "Random seed"));
	m_useSeed->setTrueText(i18n("FloatingPoint - Random", "Individual seed"));

	addEmptyCell();
	m_seed = new KSimSpinBox(this, "RndF-Seed");
	CHECK_PTR(m_seed);
	m_seed->setMinValue(MIN_SEED);
	m_seed->setMaxValue(MAX_SEED);
	
	str = i18n("FloatingPoint", "Set an individual or a random seed here.");
	addToolTip(str, m_useSeed, lab);
	addToolTip(str, m_seed);
	str = i18n("FloatingPoint",
	           "<p>Set an individual or a random seed here.</p>"
	           "<table><tr><td><b>Random seed</b></td><td> The random number generator is initialized with a random seed after each reset of the circuit.</td></tr>"
	           "<tr><td><b>Individual seed</b></td><td> The random number generator is initialized with an individual seed after each reset of the circuit (Range: 1 - %1).</td></tr></table>")
	           .arg(MAX_SEED);
	addWhatsThis(str, m_useSeed, lab);
	addWhatsThis(str, m_seed);

	//****** connect signals - slots ******
	connect(m_useSeed, SIGNAL(activated(bool)), m_seed, SLOT(setEnabled(bool)));


	//****** set values ******
	m_max->setValue(getRandom()->getMaximum());
	m_min->setValue(getRandom()->getMinimum());
	m_useSeed->setValue(getRandom()->getUseSeed());
	m_seed->setValue(getRandom()->getSeed());
	m_seed->setEnabled(getRandom()->getUseSeed());


}

/*RandomPropertyGeneralWidget::~RandomPropertyGeneralWidget()
{
} */

void RandomPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();

	if (getRandom()->getMaximum() != m_max->value())
	{
		changeData();
		getRandom()->setMaximum(m_max->value());
	}
	if (getRandom()->getMinimum() != m_min->value())
	{
		changeData();
		getRandom()->setMinimum(m_min->value());
	}
	if (getRandom()->getUseSeed() != m_useSeed->value())
	{
		changeData();
		getRandom()->setUseSeed(m_useSeed->value());
	}
	if (getRandom()->getSeed() != m_seed->value())
	{
		changeData();
		getRandom()->setSeed(m_seed->value());
	}
}

void RandomPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_max->setValue(DEFAULT_MAX);
	m_min->setValue(DEFAULT_MIN);
	m_useSeed->setValue(DEFAULT_USE_SEED);
	m_seed->setValue(DEFAULT_SEED);
}

//##########################################################################################
//##########################################################################################

#undef DEFAULT_USE_SEED
#undef DEFAULT_SEED
#undef DEFAULT_MIN
#undef DEFAULT_MAX
#undef MIN_SEED
#undef MAX_SEED

}; //namespace

#include "random.moc"
