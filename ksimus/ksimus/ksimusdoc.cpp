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

static const char * sDocProperty = "Document Property/";
static const char * sTiming      = "Timing/";
static const char * sLastView    = "Last View";

KSimusDoc::KSimusDoc(QWidget *parent, const char *name)
	:	QObject(parent, name),
		m_files(0),
		m_named(false),
		m_simRunning(false)
{
	if (!g_docList)
		g_docList = new KSimusDocList;
	g_docList->append(this);
		
	m_pViewList = new QList<KSimusView>();
	m_pViewList->setAutoDelete(true);

	m_activeView = 0;
	m_container = new CompContainer(this);
	m_undo = new KSimUndo(this);
	
	m_sheetGrid = new KSimGrid(GridDots, lightGray);
	m_userGrid = new KSimGrid(GridDots, gray);
	
	m_timing = new SimulationTiming(this);
}

KSimusDoc::~KSimusDoc()
{
	delete m_undo;
//	delete m_container;
	delete m_files;
	g_docList->remove(this);
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
			m_activeView = 0;
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
  doc_url=url;
}

const KURL& KSimusDoc::URL() const
{
  return doc_url;
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
      case KMessageBox::Cancel:
           completed = false;
           break;

      case KMessageBox::Yes:
           if (doc_url.fileName() == i18n("Untitled"))
           {
             win->slotFileSaveAs();
           }
           else
           {
             saveDocument(URL());
       	   };

       	   deleteContents();
           completed = true;
           break;

      case KMessageBox::No:
           setModified(false);
           deleteContents();
           completed = true;
           break;	

      default:
           completed = false;
           break;
    }
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

   	getContainer()->setSheetSize(QSize(1000,500));
   	getContainer()->setUserSize(QSize(1000,500));
   	
    // Init Editors
    for(KSimusView* w=m_pViewList->first(); w!=0; w=m_pViewList->next())
    {
        w->getEditor()->setEditorView(w->getEditor()->getEditorView());
    }
	slotUpdateAllViews(0);

	
	setModified(false);
	setNamed(false);

	doc_url.setFileName(i18n("Untitled"));

	return true;
}

bool KSimusDoc::openDocument(const KURL& url, const char */*format =0*/)
{
	QString tmpfile;
	if(KIO::NetAccess::download( url, tmpfile ))
	{
		/////////////////////////////////////////////////
		// TODO: Add your document opening code here
		/////////////////////////////////////////////////
		deleteContents();

		if (url.isLocalFile())
		{
			setNamed(true);
			setURL(url);
		}
		else
		{
			setNamed(false);
		}
		
		KSimData file(tmpfile);
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
		KSIMDEBUG_VAR("",KIO::NetAccess::lastErrorString())
		KMessageBox::sorry(0,i18n("File is not readable"),i18n("Open Document"));
		return false;
	}
}

bool KSimusDoc::saveDocument(const KURL& url, const char */*format =0*/)
{
	/////////////////////////////////////////////////
	// TODO: Add your document saving code here
	/////////////////////////////////////////////////

	if (url.isLocalFile())
	{
		QFileInfo file(url.path());
		
		if (file.exists() && !file.isWritable())
		{
			// File is not writable
			KMessageBox::sorry(0,i18n("File is not writable"),i18n("Save Document"));
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
			
			setURL(url);
			{
				KSimData file(url.path());
				file.setGroup("/");
				saveProperty(file);
				getContainer()->save(file);
//			file.sync();
			}
			chmod(url.path(), oldMode);
			
			setModified(false);
			setNamed(true);
		}
	}
	else
	{
		KMessageBox::sorry(0,i18n("No network access allowed"),i18n("Save Document"));
	}
	return true;
}

/** Load document property */
void KSimusDoc::loadProperty(KSimData & config)
{
	QString baseGroup = config.group();
	QString tmp = baseGroup + sDocProperty;
	if (config.hasGroup(tmp))
	{
		config.setGroup(tmp);
		
		getApp()->setCurrentView(config.readEntry(sLastView));
		
		QString group = config.group();
		if (config.hasGroup(group + sTiming))
		{
			config.setGroup(group + sTiming);
			m_timing->load(config);
			config.setGroup(group);
		}
			
		config.setGroup(baseGroup);
	}
}

/** Save document property */
void KSimusDoc::saveProperty(KSimData & config) const
{
	QString baseGroup = config.group();
	QString group;
	
	config.setGroup(baseGroup + sDocProperty);
	// Add properties
	config.writeEntry(sLastView,getApp()->getCurrentViewString());
	
	
	group = config.group();
	config.setGroup(group + sTiming);
	config.writeEntry("Dummy",true);	// Need for group creation
	m_timing->save(config);
	config.setGroup(group);
	
	
	
	config.setGroup(baseGroup);
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
		getApp()->getLogList()->info(i18n("Simulate %1 components").arg(getContainer()->getComponentNumber()));
	}
	else
	{
		getApp()->getLogList()->error(i18n("Circuit check failed (%i errors)"),errorCounter);
	}
	
	return !errorCounter;
}	

/** Resets simulation */
void KSimusDoc::simulationReset(void)
{
	m_timing->reset();
	getApp()->slotSimulationTimeChanged();
	getContainer()->setupSimulationList();
	getContainer()->resetComponents();
}

/** Starts simulation */
void KSimusDoc::simulationStart(void)
{
	getActiveView()->getEditor()->setEditorMode(EM_SELECT);
	m_timing->reset();
	getApp()->slotSimulationTimeChanged();
  m_simRunning = true;

	// Check circiut
	if (simulationCheckCirciut())
	{
		getContainer()->setupSimulationList();
		getContainer()->resetComponents();
		slotUpdateAllViews(0);
		m_timing->slotStart();
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
	m_timing->slotStop();
	slotUpdateAllViews(0);
}

/** Pause simulation if "pause" is set, or restart simulation if not set */
void KSimusDoc::simulationPause(bool /*pause*/)
{
}

/** Simulates one step */
void KSimusDoc::slotSimulateExecute()
{
	getApp()->slotSimulationTimeChanged();
	
	// Calulate
	FOR_EACH_COMP(it,*getContainer()->getCalculateComponentList())
	{
		it.current()->calculate();
	}
	
	// Set outputs
	FOR_EACH_COMP(it,*getContainer()->getUpdateOutputComponentList())
	{
		it.current()->updateOutput();
	}
}

void KSimusDoc::slotSimulateUpdate()
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

	// Reset view changed
	FOR_EACH_COMPVIEW(it,*getContainer()->getUpdateSheetViewComponentList())
	{
		it.current()->setViewChanged(false);
	}
	FOR_EACH_COMPVIEW(it,*getContainer()->getUpdateUserViewComponentList())
	{
		it.current()->setViewChanged(false);
	}
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
