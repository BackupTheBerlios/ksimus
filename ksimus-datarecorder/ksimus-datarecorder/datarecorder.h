/***************************************************************************
                          datarecorder.h  -  description
                             -------------------
    begin                : Thu May 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DATARECORDER_H
#define DATARECORDER_H

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus-Includes
#include "ksimus/component.h"
#include "ksimus/resource.h"
#include "ksimus/componentinfo.h"

// Project-Includes
#include "datarecorderchannelbase.h"

// Forward declaration
class KLocale;
class ConnectorBase;
class ConnectorInputList;


namespace KSimLibDataRecorder
{

// Forward declaration (namespace)
class DataRecorderWidget;
class DataRecorderChannelBase;
class DataRecorderChannelList;
class ZoomWidgetVar;

/**The Data Recorder
  *@author Rasmus Diekenbrock
  */




/******************************************************************************************
 ******************************************************************************************
 **
 **  Modify the package properties below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

/**  Enter the package name. E.g. "Boolean" */
#define KSIMUS_PACKAGE_NAME              "DataRecorder"

/**  Enter the lower case package name. E.g. "ksimus-boolean" */
#define KSIMUS_PACKAGE_LOWER_NAME        "ksimus-datarecorder"

/**  Enter the init function name. Format init_lib + lower case package name. E.g. init_libboolean
     E.g. init_libboolean  (use "_" instead of "-") */
#define KSIMUS_PACKAGE_INIT_FUNCTION     init_libksimus_datarecorder


	KInstance * getPackageInstance();
	const PackageInfo * getPackageInfo();

};  //namespace KSimLibDataRecorder

/** The init function. KSIMUS_PACKAGE_INIT_FUNCTION is a macro which expands to the real function name. */
extern "C" const PackageInfo * KSIMUS_PACKAGE_INIT_FUNCTION(KLocale * ksimusLocale);



//#################################################################
//#################################################################

namespace KSimLibDataRecorder
{

class DataRecorder : public Component
{
   Q_OBJECT
public:
	DataRecorder(CompContainer * container, const ComponentInfo * ci);
	~DataRecorder();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();
	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);
	

	/** Creates a new channel according to the given type. */
	DataRecorderChannelBase * createChannel(eChannelType type);
	/** Adds a new channel. */
	void newChannel(DataRecorderChannelBase * channel);
	/** Appends a channel to the channel list. */
	void appendChannel(DataRecorderChannelBase * channel);
	/** Removes a channel. */
	void removeChannel(DataRecorderChannelBase * channel);
	/** Returns the channel list. */
	DataRecorderChannelList * getChannelList() const;
	
	unsigned int getDataCount() const;

	DataRecorderWidget * getDataRecoderWidget();
	const DataRecorderWidget * getDataRecoderWidget() const;
	
	/** Returns a unique number for the channels. */
	int nextSerialNumber();


	/** Returns the sample time [s].
			The time will be updated during executing @ref reset(). */
	double getSampleTime() const;
	
	/** Returns the zoom widget property */
	const ZoomWidgetVar & getZoomVar() const { return * m_zoomVar;};
	/** Returns the zoom widget property */
	ZoomWidgetVar & getZoomVar() { return * m_zoomVar;};

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

public slots:
	void slotRemoveChannelConn(ConnectorBase * conn);
	void updateRequest();
	void undoZoom();

private:
	void arrangeChannelInputs();
	void readSampleTime();
	
	DataRecorderWidget * m_widget;
	DataRecorderChannelList * m_channelList;
	ConnectorInputList * m_channelInputList;
	unsigned int m_dataCount;
	unsigned int m_serialNumberGenerator;
	double m_sampleTime;
	ZoomWidgetVar * m_zoomVar;

private slots:
	void slotOpenWidget();
	void widgetDestroyed();
	/** Adds a new boolean channel. */
	void slotAddBoolChannel();
	/** Adds a new floating point channel. */
	void slotAddFloatChannel();
};

};  //namespace KSimLibDataRecorder

#endif
