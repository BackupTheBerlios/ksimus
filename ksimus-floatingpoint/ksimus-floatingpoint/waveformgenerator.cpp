/***************************************************************************
                          waveformgenerator.cpp  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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
#include <math.h>

// QT-Includes
#include <qpainter.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpixmap.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimtimeserver.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "ksimus/enumdict.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdoubleedit.h"
#include "ksimus/ksimtimespinbox.h"

#include "waveformgenerator.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * WaveformGenerator::create(CompContainer * container, const ComponentInfo * ci)
{
	return new WaveformGenerator(container, ci);
}

const ComponentInfo * WaveformGenerator::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Waveform Generator"),
	                                QString::fromLatin1("Floating Point/Control/Waveform Generator"),
	                                i18n("Component", "Floating Point/Control/Waveform Generator"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-waveform-gen"));
	return &Info;
}


//###############################################################
//###############################################################

#define WAVEFORMGENERATOR_DEFAULT_WAVEFORM  WaveformGenerator::eSinusoidal
#define WAVEFORMGENERATOR_DEFAULT_PHASE     0.0
#define WAVEFORMGENERATOR_DEFAULT_AMPLITUDE 1.0
#define WAVEFORMGENERATOR_DEFAULT_OFFSET    0.0



//###############################################################
//###############################################################

class WaveformGenerator::Private
{
public:
	Private()
	{};

};


static EnumDict<WaveformGenerator::eWaveType> waveformGeneratorTypeDict(5);


EnumDict<WaveformGenerator::eWaveType>::tData EnumDict<WaveformGenerator::eWaveType>::data[]
      = { {"Sinusoidal", KSimLibFloatingPoint::WaveformGenerator::eSinusoidal},
          {"Square",     KSimLibFloatingPoint::WaveformGenerator::eSquare},
          {"Sawtooth",   KSimLibFloatingPoint::WaveformGenerator::eSawtooth},
          {"Triangular", KSimLibFloatingPoint::WaveformGenerator::eTriangular},
          {"Dirac",      KSimLibFloatingPoint::WaveformGenerator::eDirac},
          {0,            (KSimLibFloatingPoint::WaveformGenerator::eWaveType)0}};


//###############################################################
//###############################################################


WaveformGenerator::WaveformGenerator(CompContainer * container, const ComponentInfo * ci)
	: Float1Out(container, ci),
		m_waveType(WAVEFORMGENERATOR_DEFAULT_WAVEFORM),
		m_amplitude(WAVEFORMGENERATOR_DEFAULT_AMPLITUDE),
		m_offset(WAVEFORMGENERATOR_DEFAULT_OFFSET),
		m_period(getTimeServer()),
		m_phase(WAVEFORMGENERATOR_DEFAULT_PHASE)
{
	m_period.setValue(1.0, unit_sec);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new WaveformGeneratorView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

WaveformGenerator::~WaveformGenerator()
{
}

/** Reset all simulation variables */
void WaveformGenerator::reset()
{
	Float1Out::reset();
	
	m_periodLength = getPeriod().raw();
	m_phaseLength = qRound((m_periodLength * getPhase()) / 360.0);
	
	m_lastFrac = 100000; // A pos value
	
	calculate();
}

/** Executes the simulation of this component */
void WaveformGenerator::calculate()
{
	Float1Out::calculate();
	
	double frac = ((double)((getTimeServer().raw() + m_phaseLength) % getPeriod().raw()))
	                                  / m_periodLength;
	double val = 0;
	
	switch(getWaveform())
	{
		case eSinusoidal:
			val = sin(2.0 * M_PI * frac);
			break;
		
		case eSquare:
			if (frac >= 0.5)
			{
				val = 1.0;
			}
			else
			{
				val = -1.0;
			}
			break;
		
		case eSawtooth:
			val = 2.0 * frac - 1.0;
			break;
		
		case eTriangular:
			if (frac >= 0.5)
			{
				val = 3.0 - 4.0 * frac;
			}
			else
			{
				val = 4.0 * frac - 1.0;
			}
			break;
		
		case eDirac:
			if (frac < m_lastFrac)
			{
				val = 1.0/getTimeServer().getTick().getValue(unit_sec);
			}
			m_lastFrac = frac;
			break;
	}
	setValue(m_amplitude * val + m_offset);
	executeNext();
}

void WaveformGenerator::save(KSimData & file) const
{
	Float1Out::save(file);
	
	QString oldGroup(file.group());
	file.setGroup(oldGroup + "Period/");
	m_period.save(file);
	file.setGroup(oldGroup);
	
	
	if (getWaveform() != WAVEFORMGENERATOR_DEFAULT_WAVEFORM)
	{
		waveformGeneratorTypeDict.save(file, "Wave Type", getWaveform());
	}
	if (getPhase() != WAVEFORMGENERATOR_DEFAULT_PHASE)
	{
		file.writeEntry("Phase", getPhase());
	}
	if (getAmplitude() != WAVEFORMGENERATOR_DEFAULT_AMPLITUDE)
	{
		file.writeEntry("Amplitude", getAmplitude());
	}
	if (getOffset() != WAVEFORMGENERATOR_DEFAULT_OFFSET)
	{
		file.writeEntry("Offset", getOffset());
	}
	
	
	
}

bool WaveformGenerator::load(KSimData & file, bool copyLoad)
{
	QString oldGroup(file.group());
	if (file.hasGroup(oldGroup + "Period/"))
	{
		file.setGroup(oldGroup + "Period/");
		m_period.load(file);
	}
	else
	{
		m_period.setValue(1.0, unit_sec);
	}
	file.setGroup(oldGroup);
	
	setWaveform(waveformGeneratorTypeDict.load(file, "Wave Type", WAVEFORMGENERATOR_DEFAULT_WAVEFORM));
	setPhase(file.readDoubleNumEntry("Phase", WAVEFORMGENERATOR_DEFAULT_PHASE));
	setAmplitude(file.readDoubleNumEntry("Amplitude", WAVEFORMGENERATOR_DEFAULT_AMPLITUDE));
	setOffset(file.readDoubleNumEntry("Offset", WAVEFORMGENERATOR_DEFAULT_OFFSET));
	
	return Float1Out::load(file, copyLoad);
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * WaveformGenerator::createGeneralProperty(QWidget *parent)
{
	WaveformGeneratorPropertyGeneralWidget * wid;
	wid = new WaveformGeneratorPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

void WaveformGenerator::setWaveform(eWaveType waveType)
{
	m_waveType = waveType;	
}

void WaveformGenerator::setPeriod(KSimTime period)
{
	m_period = period;	
}
	
void WaveformGenerator::setPhase(double phase)
{
	m_phase = fmod(phase, 360.0);	
}

void WaveformGenerator::setAmplitude(double amplitude)
{
	m_amplitude = amplitude;	
}

void WaveformGenerator::setOffset(double offset)
{
	m_offset = offset;	
}


//###############################################################
//###############################################################


//###############################################################
//###############################################################

class WaveformGeneratorView::Private
{
public:
	static QPixmap * sinusoidalPix;
	static QPixmap * squarePix;
	static QPixmap * sawtoothPix;
	static QPixmap * triangularPix;
	static QPixmap * diracPix;
	static unsigned int viewCounter;
	static const char * sinus[];
	static const char * square[];
	static const char * sawtooth[];
	static const char * triangular[];
	static const char * dirac[];
};


QPixmap * WaveformGeneratorView::Private::sinusoidalPix = (QPixmap*)0;
QPixmap * WaveformGeneratorView::Private::squarePix     = (QPixmap*)0;
QPixmap * WaveformGeneratorView::Private::sawtoothPix   = (QPixmap*)0;
QPixmap * WaveformGeneratorView::Private::triangularPix = (QPixmap*)0;
QPixmap * WaveformGeneratorView::Private::diracPix      = (QPixmap*)0;

unsigned int WaveformGeneratorView::Private::viewCounter = 0;

const char * WaveformGeneratorView::Private::sinus[]={
"16 16 3 1",
"a c #ff0000",
". c #000000",
"# c #ffffff",
".##aaa##########",
".#a###a#########",
".#a###a#########",
".a####a#########",
".a#####a########",
"a######a########",
"a######a########",
"a.......a......a",
".#######a######a",
".#######a######a",
".########a#####a",
".########a####a#",
".#########a###a#",
".#########a###a#",
".##########aaa##",
"################"};

const char * WaveformGeneratorView::Private::square[]={
"16 16 3 1",
"a c #000000",
". c #ff0000",
"# c #ffffff",
"........########",
".######.########",
".######.########",
".######.########",
".######.########",
".######.########",
".######.########",
".aaaaaa.aaaaaaa.",
"a######.#######.",
"a######.#######.",
"a######.#######.",
"a######.#######.",
"a######.#######.",
"a######.#######.",
"a######.........",
"################"};

const char * WaveformGeneratorView::Private::sawtooth[]={
"16 16 3 1",
"a c #ff0000",
". c #000000",
"# c #ffffff",
".########a######",
".#######aa######",
".######a#a######",
".######a#a######",
".#####a##a#####a",
".####a###a####a#",
".####a###a###a##",
"....a....a...a..",
".##a#####a##a###",
".##a#####a##a###",
".#a######a#a####",
".a#######aa#####",
".a#######aa#####",
"a########a######",
"a########a######",
"################"};

const char * WaveformGeneratorView::Private::triangular[]={
"16 16 3 1",
"a c #ff0000",
". c #000000",
"# c #ffffff",
".######a########",
".#####aa########",
".#####a#a#######",
".####a##a#######",
".####a###a######",
".###a####a######",
".###a#####a#####",
"...a.......a....",
".##a#######a####",
".#a#########a###",
".#a#########a###",
".a###########a##",
".a###########a##",
"a#############a#",
"a##############a",
"################"};

const char * WaveformGeneratorView::Private::dirac[]={
"16 16 3 1",
"a c #ff0000",
". c #000000",
"# c #ffffff",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
".##a#########a##",
"aaaaaaaaaaaaaaaa",
"################"};

//###############################################################
//###############################################################


WaveformGeneratorView::WaveformGeneratorView(WaveformGenerator * comp, eViewType viewType)
		: Float1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		Private::viewCounter++;

		if (Private::sinusoidalPix == (QPixmap*)0)
		{
			Private::sinusoidalPix = new QPixmap(Private::sinus);
			CHECK_PTR(Private::sinusoidalPix);
		}
		if (Private::squarePix == (QPixmap*)0)
		{
			Private::squarePix = new QPixmap(Private::square);
			CHECK_PTR(Private::squarePix);
		}
		if (Private::sawtoothPix == (QPixmap*)0)
		{
			Private::sawtoothPix = new QPixmap(Private::sawtooth);
			CHECK_PTR(Private::sawtoothPix);
		}
		if (Private::triangularPix == (QPixmap*)0)
		{
			Private::triangularPix = new QPixmap(Private::triangular);
			CHECK_PTR(Private::triangularPix);
		}
		if (Private::diracPix == (QPixmap*)0)
		{
			Private::diracPix = new QPixmap(Private::dirac);
			CHECK_PTR(Private::diracPix);
		} 	
		getComponentLayout()->setMinSize(4,3);
		getComponentLayout()->updateLayout();
	}
};

WaveformGeneratorView::~WaveformGeneratorView()
{
	Private::viewCounter--;
	
	if (Private::viewCounter == 0)
	{
		if (Private::sinusoidalPix != (QPixmap*)0)
		{
			delete Private::sinusoidalPix;
			Private::sinusoidalPix = (QPixmap*)0;
		}
		if (Private::squarePix != (QPixmap*)0)
		{
			delete Private::squarePix;
			Private::squarePix = (QPixmap*)0;
		}
		if (Private::sawtoothPix != (QPixmap*)0)
		{
			delete Private::sawtoothPix;
			Private::sawtoothPix = (QPixmap*)0;
		}
		if (Private::triangularPix != (QPixmap*)0)
		{
			delete Private::triangularPix;
			Private::triangularPix = (QPixmap*)0;
		}
		if (Private::diracPix != (QPixmap*)0)
		{
			delete Private::diracPix;
			Private::diracPix = (QPixmap*)0;
		}
	}
}



void WaveformGeneratorView::draw(QPainter * p)
{
	Float1OutView::draw(p);
	
	switch(getComponent()->getWaveform())
	{
		case WaveformGenerator::eSinusoidal:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - Private::sinusoidalPix->width()) / 2;
			int y = place.top() + (place.height() - Private::sinusoidalPix->height()) / 2;
			p->drawPixmap(x,y,*Private::sinusoidalPix);
		}
		break;
		
		case WaveformGenerator::eSquare:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - Private::squarePix->width()) / 2;
			int y = place.top() + (place.height() - Private::squarePix->height()) / 2;
			p->drawPixmap(x,y,*Private::squarePix);
		}
		break;
		
		case WaveformGenerator::eSawtooth:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - Private::sawtoothPix->width()) / 2;
			int y = place.top() + (place.height() - Private::sawtoothPix->height()) / 2;
			p->drawPixmap(x,y,*Private::sawtoothPix);
		}
		break;
		
		case WaveformGenerator::eTriangular:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - Private::triangularPix->width()) / 2;
			int y = place.top() + (place.height() - Private::triangularPix->height()) / 2;
			p->drawPixmap(x,y,*Private::triangularPix);
		}
		break;
		
		case WaveformGenerator::eDirac:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - Private::diracPix->width()) / 2;
			int y = place.top() + (place.height() - Private::diracPix->height()) / 2;
			p->drawPixmap(x,y,*Private::diracPix);
		}
		break;
		
	}
}


//###############################################################
//###############################################################

WaveformGeneratorPropertyGeneralWidget::WaveformGeneratorPropertyGeneralWidget(WaveformGenerator * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_waveformLabel = new QLabel(i18n("FloatingPoint", "Waveform: "), this, "m_waveformLabel");
	CHECK_PTR(m_waveformLabel);
	
	m_waveform = new QComboBox(this, "m_waveform");
	CHECK_PTR(m_waveform);
	m_waveform->insertItem(*WaveformGeneratorView::Private::sinusoidalPix, i18n("FloatingPoint", "Sinusoidal"), (int)WaveformGenerator::eSinusoidal);
	m_waveform->insertItem(*WaveformGeneratorView::Private::squarePix,     i18n("FloatingPoint", "Square"),     (int)WaveformGenerator::eSquare);
	m_waveform->insertItem(*WaveformGeneratorView::Private::sawtoothPix,   i18n("FloatingPoint", "Sawtooth"),   (int)WaveformGenerator::eSawtooth);
	m_waveform->insertItem(*WaveformGeneratorView::Private::triangularPix, i18n("FloatingPoint", "Triangular"), (int)WaveformGenerator::eTriangular);
	m_waveform->insertItem(*WaveformGeneratorView::Private::diracPix,      i18n("FloatingPoint", "Dirac"),      (int)WaveformGenerator::eDirac);
	m_waveform->setFixedHeight(QMAX(WaveformGeneratorView::Private::sinusoidalPix->height(),m_waveform->height()));
	
	tip = i18n("FloatingPoint", "Sets the waveform.");
	addToolTip(tip, m_waveform, m_waveformLabel);
	addWhatsThis(tip, m_waveform, m_waveformLabel);
	
	
	
	m_periodLabel = new QLabel(i18n("FloatingPoint", "Period: "), this, "m_periodLabel");
	CHECK_PTR(m_periodLabel);
	
	m_period = new KSimTimeSpinBox(comp->getPeriod(), this, "m_period");
	CHECK_PTR(m_period);
	tip = i18n("FloatingPoint", "The period or frequency of the wave.");
	addToolTip(tip, m_period, m_periodLabel);
	addWhatsThis(tip, m_period, m_periodLabel);
	
	
	
	m_frequencyLabel = new QLabel(i18n("FloatingPoint", "Frequency: "), this, "m_frequencyLabel");
	CHECK_PTR(m_frequencyLabel);
	m_frequency = new QLabel(QString::null, this, "m_frequency");
	CHECK_PTR(m_frequency);
	m_frequency->setAlignment(AlignRight);
	tip = i18n("FloatingPoint", "Shows the frequency (depends on the period).");
	addToolTip(tip, m_frequency, m_frequencyLabel);
	addWhatsThis(tip, m_frequency, m_frequencyLabel);

		
	
	
	m_phaseLabel = new QLabel(i18n("FloatingPoint", "Phase: "), this, "m_phaseLabel");
	CHECK_PTR(m_phaseLabel);
	
	m_phase = new KSimDoubleEdit(this, "m_phase");
	CHECK_PTR(m_phase);
	tip = i18n("FloatingPoint", "The phase of the wave. The phase is measured in degree.");
	addToolTip(tip, m_phase, m_phaseLabel);
	addWhatsThis(tip, m_phase, m_phaseLabel);
	
	
	
	
	m_amplitudeLabel = new QLabel(i18n("FloatingPoint", "Amplitude: "), this, "m_amplitudeLabel");
	CHECK_PTR(m_amplitudeLabel);
	
	m_amplitude = new KSimDoubleEdit(this, "m_amplitude");
	CHECK_PTR(m_amplitude);
	tip = i18n("FloatingPoint", "The amplitude of the wave.");
	addToolTip(tip, m_amplitude, m_amplitudeLabel);
	addWhatsThis(tip, m_amplitude, m_amplitudeLabel);
	
	
	
	
	m_offsetLabel = new QLabel(i18n("FloatingPoint", "Offset: "), this, "m_offsetLabel");
	CHECK_PTR(m_offsetLabel);
	
	m_offset = new KSimDoubleEdit(this, "m_offset");
	CHECK_PTR(m_offset);
	tip = i18n("FloatingPoint", "The offset of the wave.");
	addToolTip(tip, m_offset, m_offsetLabel);
	addWhatsThis(tip, m_offset, m_offsetLabel);
	
	
	
	connect(m_period, SIGNAL(valueChanged(double)), this, SLOT(valChanged()));

		
	// Setup value
	m_waveform->setCurrentItem((int)comp->getWaveform());
	m_phase->setValue(comp->getPhase());
	m_amplitude->setValue(comp->getAmplitude());
	m_offset->setValue(comp->getOffset());
	
	valChanged();
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void WaveformGeneratorPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getWaveform() != (WaveformGenerator::eWaveType)m_waveform->currentItem())
	{
		changeData();
		getComponent()->setWaveform((WaveformGenerator::eWaveType)m_waveform->currentItem());
	}
	if (getComponent()->getPeriod() != m_period->value())
	{
		changeData();
		getComponent()->setPeriod(m_period->value());
	}
	if (getComponent()->getPhase() != m_phase->value())
	{
		changeData();
		getComponent()->setPhase(m_phase->value());
	}
	if (getComponent()->getAmplitude() != m_amplitude->value())
	{
		changeData();
		getComponent()->setAmplitude(m_amplitude->value());
	}
	if (getComponent()->getOffset() != m_offset->value())
	{
		changeData();
		getComponent()->setOffset(m_offset->value());
	}

}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void WaveformGeneratorPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_waveform->setCurrentItem((int)WAVEFORMGENERATOR_DEFAULT_WAVEFORM);
	m_period->setValue(KSimTimeBase(1.0, unit_sec));
	m_phase->setValue(WAVEFORMGENERATOR_DEFAULT_PHASE);
	m_amplitude->setValue(WAVEFORMGENERATOR_DEFAULT_AMPLITUDE);
	m_offset->setValue(WAVEFORMGENERATOR_DEFAULT_OFFSET);
}

void WaveformGeneratorPropertyGeneralWidget::valChanged()
{
	QString str(i18n("FloatingPoint Frequency", "%1 Hz").arg(1.0/m_period->value().getValue(unit_sec)));
	m_frequency->setText(str);
}

//###############################################################
//###############################################################

#undef WAVEFORMGENERATOR_DEFAULT_WAVEFORM
#undef WAVEFORMGENERATOR_DEFAULT_PHASE
#undef WAVEFORMGENERATOR_DEFAULT_AMPLITUDE
#undef WAVEFORMGENERATOR_DEFAULT_OFFSET



};  //namespace KSimLibFloatingPoint

