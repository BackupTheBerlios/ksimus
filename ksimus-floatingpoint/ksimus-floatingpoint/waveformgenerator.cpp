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
#include <qgrid.h>
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

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new WaveformGenerator(container, ci);
}

const ComponentInfo * getWaveformGeneratorInfo()
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

static EnumDict<eWaveType> waveTypeDict;

EnumDict<eWaveType>::tData EnumDict<eWaveType>::data[]
			= { {"Sinusoidal", KSimLibFloatingPoint::eSinusoidal},
          {"Square",     KSimLibFloatingPoint::eSquare},
					{"Sawtooth",   KSimLibFloatingPoint::eSawtooth},
					{"Triangular", KSimLibFloatingPoint::eTriangular},
					{"Dirac",      KSimLibFloatingPoint::eDirac},
          {0,            (KSimLibFloatingPoint::eWaveType)0}};



#define DEFAULT_WAVEFORM  eSinusoidal
#define DEFAULT_PHASE     0.0
#define DEFAULT_AMPLITUDE 1.0
#define DEFAULT_OFFSET    0.0



//###############################################################
//###############################################################

static QPixmap * sinusoidalPix = 0;
static QPixmap * squarePix = 0;
static QPixmap * sawtoothPix = 0;
static QPixmap * triangularPix = 0;
static QPixmap * diracPix = 0;

static int viewCounter = 0;

static const char *sinus[]={
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

static const char *square[]={
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

static const char *sawtooth[]={
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

static const char *triangular[]={
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

static const char *dirac[]={
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


WaveformGenerator::WaveformGenerator(CompContainer * container, const ComponentInfo * ci)
	: Float1Out(container, ci),
		m_waveType(DEFAULT_WAVEFORM),
		m_amplitude(DEFAULT_AMPLITUDE),
		m_offset(DEFAULT_OFFSET),
		m_period(getTimeServer()),
		m_phase(DEFAULT_PHASE)
{
	m_period.setValue(1.0, unit_sec);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new WaveformGeneratorView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Reset all simulation variables */
void WaveformGenerator::reset()
{
	Float1Out::reset();
	
	m_periodLength = getPeriod().raw();
	m_phaseLength = qRound((m_periodLength * getPhase()) / 360.0);
	
	m_lastFrac = 100000; // A pos value
	
	calculate();
	updateOutput();

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
	
	
	if (getWaveform() != DEFAULT_WAVEFORM)
	{
		waveTypeDict.save(file, "Wave Type", getWaveform());
	}
	if (getPhase() != DEFAULT_PHASE)
	{
		file.writeEntry("Phase", getPhase());
	}
	if (getAmplitude() != DEFAULT_AMPLITUDE)
	{
		file.writeEntry("Amplitude", getAmplitude());
	}
	if (getOffset() != DEFAULT_OFFSET)
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
	
	setWaveform(waveTypeDict.load(file, "Wave Type", DEFAULT_WAVEFORM));
	setPhase(file.readDoubleNumEntry("Phase", DEFAULT_PHASE));
	setAmplitude(file.readDoubleNumEntry("Amplitude", DEFAULT_AMPLITUDE));
	setOffset(file.readDoubleNumEntry("Offset", DEFAULT_OFFSET));
	
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


WaveformGeneratorView::WaveformGeneratorView(WaveformGenerator * comp, eViewType viewType)
		: Float1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		viewCounter++;

		if (sinusoidalPix == 0)
		{
			sinusoidalPix = new QPixmap(sinus);
			CHECK_PTR(sinusoidalPix);
		}
		if (squarePix == 0)
		{
			squarePix = new QPixmap(square);
			CHECK_PTR(squarePix);
		}
		if (sawtoothPix == 0)
		{
			sawtoothPix = new QPixmap(sawtooth);
			CHECK_PTR(sawtoothPix);
		}
		if (triangularPix == 0)
		{
			triangularPix = new QPixmap(triangular);
			CHECK_PTR(triangularPix);
		}
		if (diracPix == 0)
		{
			diracPix = new QPixmap(dirac);
			CHECK_PTR(diracPix);
		} 	
		getComponentLayout()->setMinSize(4,3);
		getComponentLayout()->updateLayout();
	}
};

WaveformGeneratorView::~WaveformGeneratorView()
{
	viewCounter--;
	
	if (viewCounter == 0)
	{
		if (sinusoidalPix != 0)
		{
			delete sinusoidalPix;
			sinusoidalPix = 0;
		}
		if (squarePix != 0)
		{
			delete squarePix;
			squarePix = 0;
		}
		if (sawtoothPix != 0)
		{
			delete sawtoothPix;
			sawtoothPix = 0;
		}
		if (triangularPix != 0)
		{
			delete triangularPix;
			triangularPix = 0;
		}
		if (diracPix != 0)
		{
			delete diracPix;
			diracPix = 0;
		}
	}
}



void WaveformGeneratorView::draw(QPainter * p)
{
	Float1OutView::draw(p);
	
	switch(getComponent()->getWaveform())
	{
		case eSinusoidal:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - sinusoidalPix->width()) / 2;
			int y = place.top() + (place.height() - sinusoidalPix->height()) / 2;
			p->drawPixmap(x,y,*sinusoidalPix);
		}
		break;
		
		case eSquare:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - squarePix->width()) / 2;
			int y = place.top() + (place.height() - squarePix->height()) / 2;
			p->drawPixmap(x,y,*squarePix);
		}
		break;
		
		case eSawtooth:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - sawtoothPix->width()) / 2;
			int y = place.top() + (place.height() - sawtoothPix->height()) / 2;
			p->drawPixmap(x,y,*sawtoothPix);
		}
		break;
		
		case eTriangular:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - triangularPix->width()) / 2;
			int y = place.top() + (place.height() - triangularPix->height()) / 2;
			p->drawPixmap(x,y,*triangularPix);
		}
		break;
		
		case eDirac:
		{
			QRect place(getDrawingPlace());
			int x = place.left() + (place.width() - diracPix->width()) / 2;
			int y = place.top() + (place.height() - diracPix->height()) / 2;
			p->drawPixmap(x,y,*diracPix);
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
	
	m_waveformLabel = new QLabel(i18n("FloatingPoint", "Waveform: "), getGrid(), "m_waveformLabel");
	CHECK_PTR(m_waveformLabel);
	
	m_waveform = new QComboBox(getGrid(), "m_waveform");
	CHECK_PTR(m_waveform);
	m_waveform->insertItem(*sinusoidalPix, i18n("FloatingPoint", "Sinusoidal"), (int)eSinusoidal);
	m_waveform->insertItem(*squarePix,     i18n("FloatingPoint", "Square"),     (int)eSquare);
	m_waveform->insertItem(*sawtoothPix,   i18n("FloatingPoint", "Sawtooth"),   (int)eSawtooth);
	m_waveform->insertItem(*triangularPix, i18n("FloatingPoint", "Triangular"), (int)eTriangular);
	m_waveform->insertItem(*diracPix,      i18n("FloatingPoint", "Dirac"),      (int)eDirac);
	m_waveform->setFixedHeight(QMAX(sinusoidalPix->height(),m_waveform->height()));
	
	tip = i18n("FloatingPoint", "Sets the waveform.");
	addToolTip(tip, m_waveform, m_waveformLabel);
	addWhatsThis(tip, m_waveform, m_waveformLabel);
	
	
	
	m_periodLabel = new QLabel(i18n("FloatingPoint", "Period: "), getGrid(), "m_periodLabel");
	CHECK_PTR(m_periodLabel);
	
	m_period = new KSimTimeSpinBox(comp->getPeriod(), getGrid(), "m_period");
	CHECK_PTR(m_period);
	tip = i18n("FloatingPoint", "The period or frequency of the wave.");
	addToolTip(tip, m_period, m_periodLabel);
	addWhatsThis(tip, m_period, m_periodLabel);
	
	
	
	m_frequencyLabel = new QLabel(i18n("FloatingPoint", "Frequency: "), getGrid(), "m_frequencyLabel");
	CHECK_PTR(m_frequencyLabel);
	m_frequency = new QLabel(QString::null, getGrid(), "m_frequency");
	CHECK_PTR(m_frequency);
	m_frequency->setAlignment(AlignRight);
	tip = i18n("FloatingPoint", "Shows the frequency (depends on the period).");
	addToolTip(tip, m_frequency, m_frequencyLabel);
	addWhatsThis(tip, m_frequency, m_frequencyLabel);

		
	
	
	m_phaseLabel = new QLabel(i18n("FloatingPoint", "Phase: "), getGrid(), "m_phaseLabel");
	CHECK_PTR(m_phaseLabel);
	
	m_phase = new KSimDoubleEdit(getGrid(), "m_phase");
	CHECK_PTR(m_phase);
	tip = i18n("FloatingPoint", "The phase of the wave. The phase is measured in degree.");
	addToolTip(tip, m_phase, m_phaseLabel);
	addWhatsThis(tip, m_phase, m_phaseLabel);
	
	
	
	
	m_amplitudeLabel = new QLabel(i18n("FloatingPoint", "Amplitude: "), getGrid(), "m_amplitudeLabel");
	CHECK_PTR(m_amplitudeLabel);
	
	m_amplitude = new KSimDoubleEdit(getGrid(), "m_amplitude");
	CHECK_PTR(m_amplitude);
	tip = i18n("FloatingPoint", "The amplitude of the wave.");
	addToolTip(tip, m_amplitude, m_amplitudeLabel);
	addWhatsThis(tip, m_amplitude, m_amplitudeLabel);
	
	
	
	
	m_offsetLabel = new QLabel(i18n("FloatingPoint", "Offset: "), getGrid(), "m_offsetLabel");
	CHECK_PTR(m_offsetLabel);
	
	m_offset = new KSimDoubleEdit(getGrid(), "m_offset");
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
	
	if (getComponent()->getWaveform() != (eWaveType)m_waveform->currentItem())
	{
		changeData();
		getComponent()->setWaveform((eWaveType)m_waveform->currentItem());
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

	m_waveform->setCurrentItem((int)DEFAULT_WAVEFORM);
	m_period->setValue(KSimTimeBase(1.0, unit_sec));
	m_phase->setValue(DEFAULT_PHASE);
	m_amplitude->setValue(DEFAULT_AMPLITUDE);
	m_offset->setValue(DEFAULT_OFFSET);
}

void WaveformGeneratorPropertyGeneralWidget::valChanged()
{
	QString str(i18n("FloatingPoint Frequency", "%1 Hz").arg(1.0/m_period->value().getValue(unit_sec)));
	m_frequency->setText(str);
}

//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

