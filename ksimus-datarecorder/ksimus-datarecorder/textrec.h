/***************************************************************************
                          textrec.h  -  description
                             -------------------
    begin                : Sat Mar 9 2002
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

#ifndef TEXTREC_H
#define TEXTREC_H

// C-Includes

// QT-Includes
#include <qlist.h>

// KDE-Includes

// KSimus-Includes
#include "ksimus/component.h"
#include "ksimus/resource.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorinfo.h"
#include "ksimus/connectorbase.h"
#include "ksimus/compview.h"
#include "ksimus/ksimfilename.h"
#include "ksimus/componentpropertygeneralwidget.h"

// Project-Includes

// Forward declaration
class ConnectorBase;
class ConnectorInputList;
class ConnectorBoolInEdge;
class QFile;
class QTextStream;
class QCheckBox;
class QLineEdit;

namespace KSimLibDataRecorder
{

class ConnectorContainer
{
public:
	ConnectorContainer(ConnectorBase * connector, unsigned int serialNumber);
	~ConnectorContainer();
		
	
 	ConnectorBase * getConnector() const;
  	
 	void setSerialNumber(unsigned int serial);
 	unsigned int getSerialNumber() const;
  	
	/** save component properties */
	void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	bool load(KSimData & file, bool copyLoad);


 private:
 	ConnectorBase * m_conn;
	unsigned int m_serialNumber;
};

class ConnectorContainerList : public QList<ConnectorContainer>
{
public:
	ConnectorContainer * findSerial(unsigned int no) const;

protected:
	virtual int compareItems(QCollection::Item di1, QCollection::Item di2);
};

#define FOR_EACH_CONNECTOR_CONTAINER(_it_,_connContList_)	\
		for(QListIterator<ConnectorContainer> _it_(_connContList_);_it_.current();++_it_)


//###################################################################
//###################################################################



class TextRecView : public CompView
{
	Q_OBJECT

public:	
	TextRecView(Component * comp, eViewType viewType, const char * name);
	virtual ~TextRecView();
	
	virtual void draw(QPainter * p);
//	virtual QWidget * createCompViewWidget(QWidget * parent);

protected:

};
//###############################################################
//###############################################################



/**A textual data recorder
  *@author Rasmus Diekenbrock
  */

class TextRec : public Component
{
   Q_OBJECT

public:
	TextRec(CompContainer * container, const ComponentInfo * ci);
	~TextRec();

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
	/** Checks the component
	*   Eg. all required inputs are connected.
	*   The default implementation calls the checks functions of the connectors.
	*   Returns the number of errors.
	*/
	virtual int checkCircuit();
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref TextRecPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Adds a new connector. */
	ConnectorContainer * newConnector(const QString connLibName);
	/** Appends a connector to the connector list. */
	void appendConnector(ConnectorContainer * connCont);
	/** Removes a connector. */
	void removeConnector(ConnectorContainer * connCont);
	/** Returns the connector list. */
	ConnectorContainerList * getConnectorContainerList() const;
	
	/** Returns the trigger connector. */
	ConnectorBoolInEdge * getTrigger() const { return m_trigger; };
	
	/** Returns a unique number for the channels. */
	int nextSerialNumber();

	/** Returns the filename class. */
	KSimFilename & getFilename() { return m_filename; };

	/** Append data to file.
	  * @param append If true the new data is appended to an existing file. If false
	  *               a new file is created each simulation start.
	  */
	void setAppendEnabled(bool append);

	/** Returns true if the new data is appended to an existing file.
	  * Returns false if a new file is created each simulation start.
	  */
	bool isAppendEnabled() const;
	
	/** Add the line number in front of each data line.
	  * @param enabled If true each line has a leading line number. If false
	  *                the line number is suppressed.
	  */
	void setLineNoEnabled(bool enabled);

	/** Returns true if each line has a leading line number.
	  * Returns false if the line number is suppressed.
	  */
	bool isLineNoEnabled() const;
	
	/** Add the time stamp in front of each data line (after the lin number).
	  * The elapsed time since simulation time is used.
	  * @param enabled If true each line has a leading time stamp. If false
	  *                the time stamp is suppressed.
	  */
	void setTimeStampEnabled(bool enabled);

	/** Returns true if each line has a time stamp.
	  * Returns false if the time stamp suppressed.
	  */
	bool isTimeStampEnabled() const;
	
	/** Add a header with date and time of the simulation start.
	  * @param enabled If true the header is added. If false
	  *                the header is suppressed.
	  */
	void setHeaderDateEnabled(bool enabled);

	/** Returns true if the date header should be added.
	  * Returns false if the date header should be suppressed.
	  */
	bool isHeaderDateEnabled() const;
	
	/** Add the connector names at the top.
	  * @param enabled If true the names are added. If false
	  *                the names are suppressed.
	  */
	void setConnectorNamesEnabled(bool enabled);

	/** Returns true if the connector names should be added.
	  * Returns false if the connector names should be suppressed.
	  */
	bool isConnectorNamesEnabled() const;
	
	/** Sets the column separator.
	  * @param separator The new separator.
	  */
	void setSeparator(QString separator);

	/** Returns the column separator.
	  */
	QString getSeparator() const;
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

	static const char * const sNumberChannels;
	static const char * const sChannelX;
	static const char * const sChannelType;
	static const char * const sSerialList;
	static const char * const sLastSerialNumber;
	static const char * const sSerialNumber;
	static const char * const sFile;
	static const char * const sAppendFile;
	static const char * const sAddHeaderDate;
	static const char * const sAddConnNames;
	static const char * const sAddLineNo;
	static const char * const sAddTimeStamp;
	static const char * const sSeparator;

private:
	void arrangeConnectors();
	
	ConnectorBoolInEdge * m_trigger;
	ConnectorContainerList * m_connectorContainerList;
	unsigned int m_dataCount;
	unsigned int m_serialNumberGenerator;
	QFile * m_file;
	QTextStream * m_stream;
	KSimFilename m_filename;
	QString m_separator;
	unsigned int m_lineCnt;
	Q_UINT32 m_flags;

private slots:
	void slotRemoveChannelConn(ConnectorBase * conn);
	/** Adds a new boolean channel. */
	void slotAddBoolChannel();
	/** Adds a new floating point channel. */
	void slotAddFloatChannel();

	void slotStopSim();
	
	void slotTriggerProperty();
};

//###############################################################

/** The property widget for @ref TextRec component.
  *
  * @author Rasmus Diekenbrock
  */


class TextRecPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	TextRecPropertyGeneralWidget(TextRec * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	TextRec * getComponent() { return (TextRec*)ComponentPropertyGeneralWidget::getComponent(); };
	
	/** Returns the a pointer to the label of the @ref KSimFilenameWidget. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getFilenameWidgetLabel() const { return m_filenameLabel; };
	
	/** Returns the a pointer to the @ref KSimFilenameWidget. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimFilenameWidget * getFilenameWidget() const { return m_filename; };
	
	/** Returns the a pointer to the label of the separator input. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getSeparatorEditLabel() const { return m_separatorLabel; };

	/** Returns the a pointer to the separator @ref QLineEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLineEdit * getSeparatorEdit() const { return m_separator; };

	/** Returns the a pointer to the label of the switch box label. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getSwitchBoxLabel() const { return m_switchLabel; };

	/** Returns the a pointer to the append file @ref QCheckBox. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QCheckBox * getAppendFileCheckBox() const { return m_append; };

	/** Returns the a pointer to the line number @ref QCheckBox. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QCheckBox * getLineNumberCheckBox() const { return m_lineNo; };

	/** Returns the a pointer to the time stamp @ref QCheckBox. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QCheckBox * getTimeStampCheckBox() const { return m_timeStamp; };

	/** Returns the a pointer to the header date @ref QCheckBox. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QCheckBox * getHeaderDateCheckBox() const { return m_headerDate; };

	/** Returns the a pointer to the connector names @ref QCheckBox. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QCheckBox * getConnectorNamesCheckBox() const { return m_connectorNames; };


protected:
	QLabel * m_filenameLabel;
	KSimFilenameWidget * m_filename;

	QLabel * m_separatorLabel;
	QLineEdit * m_separator;
		
	QLabel * m_switchLabel;
	QCheckBox * m_append;
	QCheckBox * m_lineNo;
	QCheckBox * m_timeStamp;
	QCheckBox * m_headerDate;
	QCheckBox * m_connectorNames;
	

};


//###############################################################


};  //namespace KSimLibDataRecorder


#endif
