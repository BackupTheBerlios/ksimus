/***************************************************************************
                          waveformgenerator.h  -  description
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

#ifndef WAVEFORMGENERATOR_H
#define WAVEFORMGENERATOR_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1out.h"
#include "ksimus/ksimtime.h"
#include "ksimus/componentinfo.h"

// Forward declaration
class QLabel;
class QComboBox;
class KSimDoubleEdit;
class KSimTimeSpinBox;

namespace KSimLibFloatingPoint
{

/**A waveform generator.
  *
  * @author Rasmus Diekenbrock
  */

class WaveformGenerator : public Float1Out
{
public:
	class Private;
	enum eWaveType { eSinusoidal, eSquare, eSawtooth, eTriangular, eDirac };

	/** Constructs a floating point waveform generator. */
	WaveformGenerator(CompContainer * container, const ComponentInfo * ci);
	/** Destructor of a floating point waveform generator. */
	~WaveformGenerator();
	/** Reset all simulation variables */
	virtual void reset();
	/** Executes the simulation of this component */
	virtual void calculate();
	/** save module properties. */
	virtual void save(KSimData & file) const;
	/** load module properties.
		Returns true if successful. */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Returns the wave form. */
	eWaveType getWaveform() const { return m_waveType; };
	/** Sets the wave form. */
	void setWaveform(eWaveType waveType);
	
	/** Returns the period. */
	KSimTime getPeriod() const { return m_period; };
	/** Sets the period. */
	void setPeriod(KSimTime period);
	
	/** Returns the phase. */
	double getPhase() const { return m_phase; };
	/** Sets the phase. */
	void setPhase(double phase);
	
	/** Returns the amplitude. */
	double getAmplitude() const { return m_amplitude; };
	/** Sets the amplitude. */
	void setAmplitude(double amplitude);
	
	/** Returns the offset. */
	double getOffset() const { return m_offset; };
	/** Sets the offset. */
	void setOffset(double offset);
	

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	eWaveType m_waveType;
	double m_lastFrac;
	double m_amplitude;
	double m_offset;
	KSimTime m_period;
	double m_phase;
	KSimTimeType m_periodLength;
	KSimTimeType m_phaseLength;
	Private * m_p;
	
};

//###############################################################

/** The view for the floating point waveform generator.
	*
  * @author Rasmus Diekenbrock
  */
class WaveformGeneratorView : public Float1OutView
{
public:
	WaveformGeneratorView(WaveformGenerator * comp, eViewType viewType);
	~WaveformGeneratorView();
	
	virtual void draw(QPainter * p);
	
	WaveformGenerator* getComponent() { return (WaveformGenerator*) Float1OutView::getComponent(); };

	class Private;
};

//###############################################################
/** The property widget for the floating point waveform generator.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class WaveformGeneratorPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	WaveformGeneratorPropertyGeneralWidget(WaveformGenerator * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	WaveformGenerator* getComponent() { return (WaveformGenerator*) ComponentPropertyGeneralWidget::getComponent(); };
	
	
protected:
	QLabel * m_waveformLabel;
	QComboBox * m_waveform;
	KSimTimeSpinBox * m_period;
	QLabel * m_periodLabel;
	KSimDoubleEdit * m_phase;
	QLabel * m_phaseLabel;
	KSimDoubleEdit * m_amplitude;
	QLabel * m_amplitudeLabel;
	KSimDoubleEdit * m_offset;
	QLabel * m_offsetLabel;

	QLabel * m_frequencyLabel;
	QLabel * m_frequency;

private slots:

	void valChanged();
};


//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
