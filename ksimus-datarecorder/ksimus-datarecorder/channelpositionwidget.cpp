/***************************************************************************
                          channelpositionwidget.cpp  -  description
                             -------------------
    begin                : Mon Aug 20 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// KSimus-Includes
#include <ksimus/ksimdoublespinbox.h>
#include <ksimus/connectorbase.h>

// Project-Includes
#include "channelpositionwidget.h"
#include "datarecorderchannelbase.h"

// Forward declaration

		

namespace KSimLibDataRecorder
{

//#######################################################################
//#######################################################################

class ChannelPositionWidgetPrivate
{
public:
	ChannelPositionWidgetPrivate()
	{};
	~ChannelPositionWidgetPrivate()
	{};

	KSimDoubleSpinBox * offset;
	KSimDoubleSpinBox * gain;
	
};






ChannelPositionWidget::ChannelPositionWidget(DataRecorderChannelBase * channel, QWidget *parent, const char *name )
	: QGrid(2, parent, name),
		m_channel(channel)
{
	m_p = new ChannelPositionWidgetPrivate();
	CHECK_PTR(m_p);

	QLabel * label;
	label = new QLabel(i18n("DataRecorder", "&Offset: "), this, "Offset Label");
	CHECK_PTR(label);
	m_p->offset = new KSimDoubleSpinBox(0.0, -1000.0, 1000.0, 0.5, this, "Offset");
	CHECK_PTR(m_p->offset);
	m_p->offset->setSuffix(" Div");
	m_p->offset->setValue(0.0);
	label->setBuddy(m_p->offset);
	connect(m_p->offset,SIGNAL(valueChanged(double)),SLOT(slotOffsetChanged(double)));
	
	label = new QLabel(i18n("DataRecorder", "&Gain: "), this, "Gain Label");
	CHECK_PTR(label);
	m_p->gain = new KSimDoubleSpinBox(1.0, 0.001, 10000.0, 0.1, this, "Gain");
	CHECK_PTR(m_p->gain);
	m_p->gain->setNewExponentSteps(1.0, 2.0, 5.0);
	m_p->gain->setSuffix(" Div");
	m_p->gain->setValue(1.0);
	label->setBuddy(m_p->gain);
	connect(m_p->gain,SIGNAL(valueChanged(double)),SLOT(slotGainChanged(double)));
	
	slotSetCaption(getChannel()->getConnector()->getName());
	connect(getChannel()->getConnector(), SIGNAL(signalSetName(const QString &)),SLOT(slotSetCaption(const QString &)));
}

ChannelPositionWidget::~ChannelPositionWidget()
{
	delete m_p;
}

/** Returns the assigned channel */
DataRecorderChannelBase * ChannelPositionWidget::getChannel() const
{
	return m_channel;
}

/** Returns the offset spinbox. */
KSimDoubleSpinBox * ChannelPositionWidget::getOffsetSpinBox() const
{
	return m_p->offset;
}

/** Returns the gain spinbox. */
KSimDoubleSpinBox * ChannelPositionWidget::getGainSpinBox() const
{
	return m_p->gain;
}

/** Returns the offset. */
double ChannelPositionWidget::getOffsetValue() const
{
	return getOffsetSpinBox()->value();
}

/** Returns the gain. */
double ChannelPositionWidget::getGainValue() const
{
	return getGainSpinBox()->value();
}

/** Sets the offset. */
void ChannelPositionWidget::setOffsetValue(double value)
{
	getOffsetSpinBox()->setValue(value);
}

/** Sets the maximum offset. */
void ChannelPositionWidget::setOffsetMaxValue(double value)
{
	getOffsetSpinBox()->setMaxValue(value);
}

/** Sets the minimum offset. */
void ChannelPositionWidget::setOffsetMinValue(double value)
{
	getOffsetSpinBox()->setMinValue(value);
}

/** Sets the offset line step. */
void ChannelPositionWidget::setOffsetLineStep(double value)
{
	getOffsetSpinBox()->setLineStep(value);
}

/** Sets the gain. */
void ChannelPositionWidget::setGainValue(double value)
{
	getGainSpinBox()->setValue(value);
}

/** Sets the maximum gain. */
void ChannelPositionWidget::setGainMaxValue(double value)
{
	getGainSpinBox()->setMaxValue(value);
}

/** Sets the minimum gain. */
void ChannelPositionWidget::setGainMinValue(double value)
{
	getGainSpinBox()->setMinValue(value);
}

/** Sets the gain line step. */
void ChannelPositionWidget::setGainLineStep(double value)
{
	getGainSpinBox()->setLineStep(value);
}

void ChannelPositionWidget::slotOffsetChanged(double offset)
{
	emit offsetChanged(offset);
}

void ChannelPositionWidget::slotGainChanged(double gain)
{
	emit gainChanged(gain);
}

void ChannelPositionWidget::slotSetCaption(const QString & caption)
{
	topLevelWidget()->setCaption(caption);
}



};  //namespace KSimLibDataRecorder

#include "channelpositionwidget.moc"

