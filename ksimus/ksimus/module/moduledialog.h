/***************************************************************************
                          moduledialog.h  -  description
                             -------------------
    begin                : Mon Jul 17 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef MODULEDIALOG_H
#define MODULEDIALOG_H

// C-Includes

// QT-Includes
#include <qwidget.h>
#include <qptrlist.h>
#include <qvbox.h>

// KDE-Includes

// Project-Includes
#include "types.h"
#include "moduledata.h"

// Forward declaration


/**Module view dialog
  *@author Rasmus Diekenbrock
  */

class QRadioButton;
class QPushButton;
class QLineEdit;
class QListView;
class QListViewItem;
class QScrollView;

class CompContainer;
class ModuleEditor;
class ExternalConnector;

class ModuleDialog : public QVBox
{
friend class ModuleEditor;

   Q_OBJECT
public: 
	ModuleDialog(CompContainer * container, QWidget *parent, const char *name=0);
	~ModuleDialog();
	CompContainer * getContainer() const;
	/** Setup module data and refreshs view */
	void setup();
	
	ModuleData * getData() const { return mdata; };
	
	/** Call, if external datas are changed */
	void modifyExternals(const QString & description);
	/** Call, if module properties are changed */
	void modifyProperties(const QString & description);

protected: // Protected methods
	CompContainer * m_container;
	ModuleData * mdata;
	virtual void resizeEvent(QResizeEvent * ev);
	void posSelected(QPoint & pos, bool insert, ConnOrientationType orientation);
	
	
	/** Creates the widget view */
	void createView();
	/** Init the data of the view */
	void initView();
	
	/** Creates the type selection widget
		Returns a pointer to type selection widget */
	QWidget * createTypeSelectionWidget(QWidget * parent);
	
	/** Creates the connector selection widget
		Returns a pointer to connector selection widget */
	QWidget * createConnSelectionWidget(QWidget * parent);
	QListView * connView;
	/** List of all connectors
		Same order as connector or position lists in mdata */
	QPtrList<QListViewItem> * connViewItems;
	/** Last selected connector
			0, if no connector is selected */
	ExternalConnector * selectedConn;

	/** Creates the module editor widget
		Returns a pointer to connector selection widget */
	QWidget * createEditorWidget(QWidget * parent);
	QScrollView * m_scrView;
	ModuleEditor * modEdit;

	/** update used information in connView */
	void setupUsedData();

protected slots:
	/** set the type of module view
		Types are: MV_GENERIC, MV_USERVIEW, MV_PIXMAP */
	void setModuleView(ModuleViewType viewType);

	/** the slot is called if connector is selected in connBox */
	void slotSelectConnBox(QListViewItem * selItem);

	/** Called, if ModulePropertyWidget is changed */
	void slotPropertyChanged();
	
signals:
	void signalSetup();
};

#endif
