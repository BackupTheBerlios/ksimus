/***************************************************************************
                          zoomwidget.h  -  description
                             -------------------
    begin                : Fri Oct 5 2001
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

#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

// C-Includes

// QT-Includes
#include <qwidget.h>
#include <qvbox.h>

// KDE-Includes

// KSimus-Includes

// Project-Includes

// Forward declaration
class QLabel;
class KSimDoubleUnitSpinBox;
class KSimData;


namespace KSimLibDataRecorder
{

// Forward declaration (namespace)
class DataRecorderDataView;


/**Time division selector.
  *@author Rasmus Diekenbrock
  */

class ZoomWidget : public QVBox
{
   Q_OBJECT
public: 
	ZoomWidget(DataRecorderDataView * recorder, QWidget *parent=0, const char *name=0);
	~ZoomWidget();
	
	DataRecorderDataView * getRecorderDataView() const { return m_recorderDataView; };
	
	const QString & getUnitList() const { return m_unitList; };
	void setUnitList(const QString & unitListStr);
	const QString & getUnit() const;
	
	double getRawValue() const;
	void setRawValue(double rawValue);
	
	const QString & getFixedUnit() const;
	void setFixedUnit(const QString & fixedUnit);
	
  bool getExponentEnabled() const;
	void setExponentEnabled(bool ena);
		
	double getTimePerPixel() const;
	double getSamplePerPixel() const;
	
	
	double getMinZoom() const;
	double getMaxZoom() const;
	
public slots:
	void setMinZoom(double val);	
	void updateMinZoom();	
	void setMaxZoom(double val);	



private slots:
	void slotChanged();
	void slotUndoRequest();

private:
	DataRecorderDataView * m_recorderDataView;
	KSimDoubleUnitSpinBox * m_time;
	QLabel * m_label;
	QString m_unitList;

signals:
	void changedTimePixel(double timePerPixel);
	void changedSamplePixel(double samplePerPixel);
	void undoRequest();

};

//##########################################################################################
//##########################################################################################

class ZoomWidgetVar
{
public:
	
	ZoomWidgetVar(const double & sampleTime);
	ZoomWidgetVar(const ZoomWidget & zoom);
	~ZoomWidgetVar();

	bool operator==(const ZoomWidgetVar & var) const;
	bool operator!=(const ZoomWidgetVar & var) const { return ! operator==(var);};
	
	
	void copyFrom(const ZoomWidget & zoom);
	void copyTo(ZoomWidget & zoom) const;
	
	/** save properties */
	void save(KSimData & file) const;
	/** load properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	bool load(KSimData & file, bool copyLoad);
	
private:
	double m_rawValue;
	QString m_unitListString;
	QString m_fixedUnitString;
	bool m_exponentSteps;
	

};




};  //namespace KSimLibDataRecorder


#endif
