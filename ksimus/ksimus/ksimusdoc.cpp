/***************************************************************************
                          ksimusdoc.cpp  -  description
                             -------------------
    begin                : Sam Dez 23 15:44:46 CET 2000
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

// C-Includes
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// include files for Qt
#include <qdir.h>
#include <qwidget.h>
#include <qmessagebox.h>

// include files for KDE
#include <klocale.h>
#include <kmessagebox.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <ktempfile.h>

// application specific includes
#include "ksimdata.h"
#include "globals.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimusview.h"
#include "ksimeditor.h"
#include "ksimdebug.h"

#include "compcontainer.h"
#include "ksimundo.h"
#include "ksimfile.h"
#include "ksimgrid.h"
#include "ksimtimeserver.h"
#include "simulationtiming.h"
#include "loglist.h"
#include "watchwidget.h"

#include "simulationexecute.h"

static const char * sDocProperty = "Document Property/";
static const char * sTiming      = "Timing/";
static const char * sSheetGrid   = "Sheet Grid/";
static const char * sUserGrid    = "User Grid/";
static const char * sLastView    = "Last View";
static const char * sSheetPos    = "Sheet Pos";
static const char * sUserPos     = "User Pos";


#define DEFAULT_GRID_STYLE	GridDots
#define DEFAULT_GRID_COLOR	gray

//##########################################################################################
//##########################################################################################

class KSimusDoc::Private
{
public:
	Private() {};
	~Private() {};

	KURL doc_url;
};

//##########################################################################################
//##########################################################################################

KSimusDoc::KSimusDoc(QWidget *parent, const char *name)
	:	QObject(parent, name),
		m_modified(false),
		m_files(0),
		m_named(false),
		m_simRunning(false),
		m_simPaused(false)
{
	m_p = new Private();
	CHECK_PTR(m_p);
	
	if (!g_docList)
		g_docList = new KSimusDocList;
	g_docList->append(this);
		
	m_pViewList = new QList<KSimusView>();
	CHECK_PTR(m_pViewList);
	m_pViewList->setAutoDelete(true);

	m_activeView = (KSimusView *)0;
	m_container = new CompContainer(this);
	CHECK_PTR(m_container);
	m_undo = new KSimUndo(this);
	CHECK_PTR(m_undo);
	
	m_sheetGrid = new KSimGrid(DEFAULT_GRID_STYLE, DEFAULT_GRID_COLOR);
	CHECK_PTR(m_sheetGrid);
	m_userGrid = new KSimGrid(DEFAULT_GRID_STYLE, DEFAULT_GRID_COLOR);
	CHECK_PTR(m_userGrid);
	
	m_timing = new SimulationTiming(this);
	CHECK_PTR(m_timing);
	m_execute = new SimulationExecute(this);
	CHECK_PTR(m_execute);
}

KSimusDoc::~KSimusDoc()
{
	delete m_undo;
//	delete m_container;
	delete m_files;
	g_docList->remove(this);
	delete m_p;
}

KSimusApp * KSimusDoc::getApp() const
{
	return (KSimusApp*)parent();
};

void KSimusDoc::addView(KSimusView *view)
{
	m_pViewList->append(view);
	connect(m_container, SIGNAL(signalDelete(Component *)),
					view->getEditor(), SLOT(slotDelete(Component *)));
	if (!m_activeView)
		m_activeView = view;
}

void KSimusDoc::removeView(KSimusView *view)
{
	m_pViewList->remove(view);
	if (m_activeView == view)
	{
		if (m_pViewList->count())
		{
			m_activeView = m_pViewList->first();
		}
		else
		{
			m_activeView = (KSimusView *)0;
		}
	}
}
/** Set active view */
void KSimusDoc::setActiveView(KSimusView * view)
{
	if (-1 != m_pViewList->find(view))
	{
		m_activeView = view;
	}
	else
	{
		KSIMDEBUG("Unknown View");
	}
}
/** Returns the active view */
KSimusView * KSimusDoc::getActiveView() const
{
	return m_activeView;
}

/** Returns the active editor */
KSimEditor * KSimusDoc::getActiveEditor() const
{
	return m_activeView->getEditor();
}

void KSimusDoc::addComponentToEditor(Component * comp)
{
	// Add widgets to all editors
	for(QListIterator<KSimusView> it(*m_pViewList);it.current();++it)
	{
		KSimEditor * ed = it.current()->getEditor();
		switch (ed->getEditorView())
		{
			case EV_SHEETVIEW:
				if (comp->getSheetView())
					comp->getSheetView()->makeWidget(ed);
				break;
				
			case EV_USERVIEW:
				if (comp->getUserView())
					comp->getUserView()->makeWidget(ed);
				break;
			
			default:
				break;
		}
	}
}

void KSimusDoc::setURL(const KURL &url)
{
  m_p->doc_url=url;
}

const KURL& KSimusDoc::URL() const
{
  return m_p->doc_url;
}

void KSimusDoc::slotUpdateAllViews(KSimusView *sender)
{
	KSimusView *w;
	if(m_pViewList)
	{
		for(w=m_pViewList->first(); w!=0; w=m_pViewList->next())
		{
			if(w!=sender)
			{
				w->getEditor()->updateDrawMap();
				w->getEditor()->update();
//				w->repaint();
			}
		}
	}
}

bool KSimusDoc::saveModified()
{
	bool completed=true;

	if(isModified())
	{
		KSimusApp *win = getApp();
		int want_save = KMessageBox::warningYesNoCancel(win,
		                          i18n("The current file has been modified.\n"
		                               "Do you want to save it?"),
		                          i18n("Warning"));
		switch(want_save)
		{
			case KMessageBox::Yes:
				if (m_p->doc_url.fileName() == i18n("Untitled"))
					{
						win->slotFileSaveAs();
					}
					else
					{
						saveDocument(URL());
					}
					newDocument();
//					completed = true;
					break;

			case KMessageBox::No:
				newDocument();
//				completed = true;
				break;	

			case KMessageBox::Cancel:
			default:
				completed = false;
				break;
		}
	}
	else
	{
		newDocument();
	}

	return completed;
}

void KSimusDoc::closeDocument()
{
	deleteContents();
	delete m_files;
}

bool KSimusDoc::newDocument()
{
	/////////////////////////////////////////////////
	// TODO: Add your document initialization code here
	/////////////////////////////////////////////////

	deleteContents();

/*	getContainer()->setSheetSize(QSize(1000,500));
	getContainer()->setUserSize(QSize(1000,500));*/
	setSheetSize(QSize(1000,500));
	setUserSize(QSize(1000,500));
   	
	// Init Editors
	for(KSimusView* w=m_pViewList->first(); w!=0; w=m_pViewList->next())
	{
		w->getEditor()->setEditorView(w->getEditor()->getEditorView());
	}
	slotUpdateAllViews(0);
	m_timing->setDefault();

	
	setModified(false);
	setNamed(false);

	m_p->doc_url.setFileName(i18n("Untitled"));

	emit signalNewDoc();

	return true;
}

bool KSimusDoc::openDocument(const KURL& url, const char */*format =0*/)
{
	QString tmpfile;
	if(KIO::NetAccess::download( url, tmpfile ))
	{
		deleteContents();

/*		if (url.isLocalFile())
		{*/
			setNamed(true);
			setURL(url);
/*		}
		else
		{
			setNamed(false);
		}*/
		
		KSimData file(tmpfile, KSimData::versionAsIs, true);   // Read only
		file.setGroup("/");
		getContainer()->load(file);
		loadProperty(file);
	
	
		KIO::NetAccess::removeTempFile( tmpfile );

		setModified(false);
		getContainer()->routeComponents();
		slotUpdateAllViews(0);

		return true;
	}
	else
	{
		KMessageBox::error(0,i18n("File access failed!\n\n%1").arg(KIO::NetAccess::lastErrorString()),i18n("Open Document"));
		return false;
	}
}

bool KSimusDoc::saveDocument(const KURL& url, const char */*format =0*/)
{
	bool res = true;
	KURL oldUrl(URL()); // Store old URL
	setURL(url);
	
	
	if (url.isLocalFile())
	{
		QFileInfo file(url.path());
		
		if (file.exists() && !file.isWritable())
		{
			// File is not writable
			KMessageBox::sorry(0,i18n("File is not writable"),i18n("Save Document"));
			res = false;
		}
		else
		{
			struct stat attrib;
			mode_t oldMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;		// default
			if(file.exists())
			{
				if (0 == stat(url.path(), &attrib))
				{
					oldMode = attrib.st_mode; // & (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
				}
				QFile::remove(url.path());
			}
			
			{
				// see also below
				KSimData file(url.path());
				file.setGroup("/");
				saveProperty(file);
				getContainer()->save(file);
			}
			chmod(url.path(), oldMode);
			
			if (!file.exists())
			{
				// Ups, file does not exist
				KMessageBox::error(0,i18n("Ups, file does not exist after writing it!"),i18n("Save Document"));
				setURL(oldUrl); // Resore old name
				res = false;
			}
			else
			{
				setModified(false);
				setNamed(true);
			}
		}
	}
	else
	{
/*		KMessageBox::sorry(0,i18n("No network access allowed"),i18n("Save Document"));
		res = false;*/
		KTempFile tmpFile;
		
		{
			// see also above
			KSimData file(tmpFile.name());
			file.setGroup("/");
			saveProperty(file);
			getContainer()->save(file);
		}
				
		if(!KIO::NetAccess::upload( tmpFile.name(), url ))
		{
			KMessageBox::error(0,i18n("File not writen!\n\n").arg(KIO::NetAccess::lastErrorString()),i18n("Save Document"));
			setURL(oldUrl); // Resore old name
			res = false;
		}
		else
		{
			setModified(false);
			setNamed(true);
		}
		
		tmpFile.unlink();
	}
	return res;
}

/** Load document property */
void KSimusDoc::loadProperty(KSimData & config)
{
	if (config.hasGroupRel(sDocProperty))
	{
		// Add to undo (if required)
		getUndo()->reloadDocumentProperty();
		
		config.pushGroupRel(sDocProperty);
		
		getApp()->setCurrentView(config.readEntry(sLastView));
		
		QPoint defPos(0,0);
		QPoint pos;
		
		pos = config.readPointEntry(sSheetPos, &defPos);
		
		for(KSimusView* w=m_pViewList->first(); w!=0; w=m_pViewList->next())
		{
			if (w->getEditor()->getEditorView() == EV_SHEETVIEW)
			{
				w->setContentsPos(pos.x(),pos.y());
			}
		}
		
		pos = config.readPointEntry(sUserPos, &defPos);
		
		for(KSimusView* w=m_pViewList->first(); w!=0; w=m_pViewList->next())
		{
			if (w->getEditor()->getEditorView() == EV_USERVIEW)
			{
				w->setContentsPos(pos.x(),pos.y());
			}
		}
		
		if (config.hasGroupRel(sTiming))
		{
			config.pushGroupRel(sTiming);
			m_timing->load(config);
			config.popGroup();
		}
			
		config.pushGroupRel(sSheetGrid);
		getSheetGrid()->load(config);
		config.popGroup();
			
		config.pushGroupRel(sUserGrid);
		getUserGrid()->load(config);
		config.popGroup();
		
		config.popGroup();
	}
}

/** Save document property */
void KSimusDoc::saveProperty(KSimData & config) const
{
	config.pushGroupRel(sDocProperty);
	// Add properties
	config.writeEntry(sLastView,getApp()->getCurrentViewString());
	
	QPoint pos(0,0);
	for(KSimusView* w=m_pViewList->first(); w!=0; w=m_pViewList->next())
	{
		if (w->getEditor()->getEditorView() == EV_SHEETVIEW)
		{
			pos = QPoint(w->contentsX(),w->contentsY());
			break;
		}
	}
	config.writeEntry(sSheetPos, pos);
		
	pos = QPoint(0,0);
	for(KSimusView* w=m_pViewList->first(); w!=0; w=m_pViewList->next())
	{
		if (w->getEditor()->getEditorView() == EV_USERVIEW)
		{
			pos = QPoint(w->contentsX(),w->contentsY());
			break;
		}
	}
	config.writeEntry(sUserPos, pos);
	
	config.pushGroupRel(sTiming);
	config.writeEntry("Dummy",true);	// Need for group creation
	m_timing->save(config);
	config.popGroup();
	
	
	config.pushGroupRel(sSheetGrid);
	getSheetGrid()->save(config);
	config.popGroup();
	
	config.pushGroupRel(sUserGrid);
	getUserGrid()->save(config);
	config.popGroup();
	
	config.popGroup();
}

/** sets the modified flag for the document after a modifying action on the view connected to the document.*/
void KSimusDoc::setModified(bool _m)
{
	if (m_modified != _m)
	{
		m_modified=_m;
	    getApp()->slotSaveAllowed(_m);
	}
};

/** returns if the document is modified or not. Use this to determine if your document needs saving by the user on closing.*/
bool KSimusDoc::isModified() const
{
	return m_modified;
};

void KSimusDoc::deleteContents()
{
  /////////////////////////////////////////////////
  // TODO: Add implementation to delete the document contents
  /////////////////////////////////////////////////
	if (isSimulationRunning())
		getApp()->slotExecuteStop();
	
	for(KSimusView* w=m_pViewList->first(); w!=0; w=m_pViewList->next())
	{
		w->getEditor()->unselectAll();
		w->setContentsPos(0,0);
	}
	
	getContainer()->deleteAll();

  getApp()->setCurrentView(APP_SHEET_VIEW);

	getUndo()->removeUndoHistory();
	getUndo()->removeRedoHistory();
	if(getApp()->getLogList())
		getApp()->getLogList()->clear();
		
	if (m_files)
		delete m_files;	
	m_files = new KSimFile();
	
	*m_sheetGrid = KSimGrid(DEFAULT_GRID_STYLE, DEFAULT_GRID_COLOR);
	*m_userGrid = KSimGrid(DEFAULT_GRID_STYLE, DEFAULT_GRID_COLOR);
	
	bool res = m_files->setupDir();
	if (!res)
	{
		QString name = ((KSimusApp*)parent())->caption();
		QMessageBox::critical (0, name,
								  m_files->getErrorText(),
								  QMessageBox::Abort,0,0);
		((KSimusApp*)parent())->slotFileQuit();
		return;
	}
}
void KSimusDoc::undo()
{
	getUndo()->undo();
	getContainer()->routeComponents();
	slotUpdateAllViews(0);
}
void KSimusDoc::redo()
{
	getUndo()->redo();
	getContainer()->routeComponents();
	slotUpdateAllViews(0);
}

/** Display cost map */
bool KSimusDoc::isCostMapVisible() const
{
	return getContainer()->isCostMapVisible();
}

void KSimusDoc::setCostMapVisible(bool enable)
{
	if (enable != isCostMapVisible())
	{
		getContainer()->setCostMapVisible(enable);
		slotUpdateAllViews(0);
	}
}

/** Starts the circuit check. Returns true if no error is found */
bool KSimusDoc::simulationCheckCirciut()
{
	int errorCounter;
	
	getApp()->getLogList()->clear();
	getApp()->getLogList()->info(i18n("Start circuit check ..."));
	
	errorCounter = getContainer()->checkCircuit();
	
	if (!errorCounter)
	{
		getApp()->getLogList()->info(i18n("Circuit check OK"));
//		getApp()->getLogList()->info(i18n("Simulate %1 components").arg(getContainer()->getComponentNumber()));
	}
	else
	{
		getApp()->getLogList()->error(i18n("Circuit check failed (%1 errors)").arg(errorCounter));
	}
	
	return !errorCounter;
}	

/** Resets simulation */
void KSimusDoc::simulationReset(void)
{
	m_simPaused = false;
	m_timing->reset();
	getApp()->slotSimulationTimeChanged();
	getContainer()->setupSimulationList();
	emit signalPreReset();
	getContainer()->setupCircuit();
	getContainer()->resetComponents();
	emit signalPostReset();
}

/** Starts simulation */
void KSimusDoc::simulationStart(void)
{
	m_simPaused = false;
	getActiveView()->getEditor()->setEditorMode(EM_SELECT);
	m_timing->reset();
	m_execute->reset();
	
	getApp()->slotSimulationTimeChanged();
  m_simRunning = true;

	// Check circiut
	if (simulationCheckCirciut())
	{
		emit signalPreReset();
		getContainer()->setupSimulationList();
		getContainer()->setupCircuit();
		getContainer()->resetComponents();
		emit signalPostReset();
		slotUpdateAllViews(0);
		emit signalStart();
		m_timing->slotStart();
		getApp()->getLogList()->info(i18n("KSimus", "Running..."));
	}
	else
	{
		// Do not call immediatly
		QTimer::singleShot(1,getApp(),SLOT(slotExecuteStop()));
	}	
}

/** Stops simulation */
void KSimusDoc::simulationStop(void)
{
	m_simRunning = false;
	m_simPaused = false;
	m_timing->slotStop();
	emit signalStop();
	slotUpdateAllViews(0);
	getApp()->getLogList()->info(i18n("KSimus", "Stopped!"));
}

/** Pause simulation if "pause" is set, or restart simulation if not set */
void KSimusDoc::simulationPause(bool pause)
{
	if (pause)
	{
		m_timing->slotStop();
		getApp()->getLogList()->info(i18n("KSimus", "Paused..."));
	}
	else
	{
		m_timing->slotStart();
		getApp()->getLogList()->info(i18n("KSimus", "Running..."));
	}
	m_simPaused = pause;
	emit signalPaused(pause);
}

/** Simulates one step */
void KSimusDoc::simulateExecute()
{
	getApp()->slotSimulationTimeChanged();
	
	// Calulate
	emit signalCalculate();
	m_execute->execute();
	getApp()->getWatchWidget()->execute();

}

void KSimusDoc::simulateUpdate()
{
	// Update views
	KSimusView *w;
	if(m_pViewList)
	{
		for(w=m_pViewList->first(); w!=0; w=m_pViewList->next())
		{
			w->getEditor()->refreshSimMode(getContainer()->getUpdateSheetViewComponentList(),
			                               getContainer()->getUpdateUserViewComponentList());
		}
	}
	
	emit signalUpdateView();
}
	
const QString KSimusDoc::getTimeString() const
{
	return m_timing->getTimeServer().getAdjustValueString(2);
}

const KSimTimeServer & KSimusDoc::getTimeServer() const
{
	return m_timing->getTimeServer();
}


/** sets the named flag for the document after saving or loading document.*/
void KSimusDoc::setNamed(bool _m)
{
	m_named = _m;
}

/** returns if the document is named or not. Use this to determine if your document needs the dialog "save as".*/
bool KSimusDoc::isNamed() const
{
	return m_named;
}

void KSimusDoc::setSheetSize(const QSize & newSize)
{
	getContainer()->setSheetSize(newSize);
	// Fixed to grid
	QSize size = getContainer()->getSheetSize();
	
	if(getViewList())
	{
		KSimusView * w;
		for(w = getViewList()->first(); w != 0; w = getViewList()->next())
		{
			if (w->getEditor()->getEditorView() == EV_SHEETVIEW)
			{
				w->getEditor()->setSize(size);
			}
		}
	}
}
	
QSize KSimusDoc::getSheetSize() const
{
	return getContainer()->getSheetSize();
}

void KSimusDoc::setUserSize(const QSize & newSize)
{
	getContainer()->setUserSize(newSize);
	// Fixed to grid
	QSize size = getContainer()->getUserSize();
	
	if(getViewList())
	{
		KSimusView * w;
		for(w = getViewList()->first(); w != 0; w = getViewList()->next())
		{
			if (w->getEditor()->getEditorView() == EV_USERVIEW)
			{
				w->getEditor()->setSize(size);
			}
		}
	}
}
	
QSize KSimusDoc::getUserSize() const
{
	return getContainer()->getUserSize();
}


