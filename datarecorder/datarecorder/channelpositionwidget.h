/***************************************************************************
                          channelpositionwidget.h  -  description
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

#ifndef CHANNELPOSITIONWIDGET_H
#define CHANNELPOSITIONWIDGET_H

// C-Includes

// QT-Includes
#include <qgrid.h>

// KDE-Includes

// KSimus-Includes

// Project-Includes

// Forward declaration
class DataRecorderChannelBase;
class ChannelPositionWidgetPrivate;
class KSimDoubleSpinBox;


/**Widget for channel positioning
  *@author Rasmus Diekenbrock
  */

class ChannelPositionWidget : public QGrid
{
   Q_OBJECT

public: // Public methods
	ChannelPositionWidget(DataRecorderChannelBase * channel, QWidget *parent=0, const char *name=0);
	~ChannelPositionWidget();

  /** Returns the assigned channel. */
  DataRecorderChannelBase * getChannel() const;
  /** Returns the offset spinbox. */
  KSimDoubleSpinBox * getOffsetSpinBox() const;
  /** Returns the gain spinbox. */
  KSimDoubleSpinBox * getGainSpinBox() const;

	/** Returns the offset. */
	double getOffsetValue() const;
	/** Returns the gain. */
	double getGainValue() const;

	/** Sets the maximum offset. */
	void setOffsetMaxValue(double value);
	/** Sets the minimum offset. */
	void setOffsetMinValue(double value);
	/** Sets the offset line step. */
	void setOffsetLineStep(double value);

	/** Sets the maximum gain. */
	void setGainMaxValue(double value);
	/** Sets the minimum gain. */
	void setGainMinValue(double value);
	/** Sets the gain line step. */
	void setGainLineStep(double value);

public slots:
	/** Sets the offset. */
	void setOffsetValue(double value);
	/** Sets the gain. */
	void setGainValue(double value);


private: // Private attributes
  /** The assigned channel */
  DataRecorderChannelBase * m_channel;
  /** Privates */
  ChannelPositionWidgetPrivate * m_p;

private slots:
	void slotOffsetChanged(double offset);
	void slotGainChanged(double gain);
	void slotSetCaption(const QString & caption);

signals:
	void offsetChanged(double offset);	
	void gainChanged(double gain);	

};

#endif
