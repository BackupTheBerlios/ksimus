/***************************************************************************
                          ksimus.cpp  -  description
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

// include files for QT
#include <qdir.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qstring.h>

// include files for KDE
#include <kprinter.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>
#include <kstddirs.h>
#include <kstatusbar.h>

// application specific includes
#include "globals.h"
#include "ksimus.h"
#include "ksimusview.h"
#include "ksimusdoc.h"
#include "ksimeditor.h"
#include "ksimundo.h"
#include "ksimfile.h"
#include "ksimdebug.h"
#include "basewindow.h"
#include "library.h"
#include "componentlistview.h"
#include "componentinfo.h"
#include "simulationtimingwidget.h"
#include "ksimgriddialog.h"
#include "enumdict.h"
#include "moduledialog.h"
#include "module.h"
#include "loglist.h"
#include "loglistitem.h"
#include "ksimpackagefilewidget.h"
#include "mapsizeswidget.h"

#define ID_STATUS_MSG 1
#define ID_TIME_LABEL 2

#define ID_SHEET_VIEW   1
#define ID_USER_VIEW    2
#define ID_MODULE_VIEW  3



//################################################################################
//################################################################################


static EnumDict<eAppViewType> currentViewDict;

EnumDict<eAppViewType>::tData EnumDict<eAppViewType>::data[]
			= { {"Sheet View", APP_SHEET_VIEW},
          {"User View",  APP_USER_VIEW},
					{"Module View", APP_MODULE_VIEW},
          {0,(eAppViewType)0}};


//################################################################################
//################################################################################



class KSimusApp::Private
{
public:	
	
	Private()
		:	lastSelectedCI(0),
			tabWidget(0),
			sheetView(0),
			userView(0),
			moduleDialog(0),
			currentView(APP_SHEET_VIEW)
	{};	
	
/*	~Private()
	{};*/
	
	const ComponentInfo * lastSelectedCI;
	QTabWidget * tabWidget;
	KSimusView * sheetView;
	KSimusView * userView;
	ModuleDialog * moduleDialog;
	eAppViewType currentView;
	QString moduleFile;
};






//################################################################################
//################################################################################

KSimusApp::KSimusApp(QWidget* , const char* name)
	:	KMainWindow(0, name),
		baseWin(0),
		m_deleteAllowed(false),
		m_cutAllowed(false),
		m_copyAllowed(false),
	  m_pastAllowed(false),
		m_undoAllowed(false),
		m_redoAllowed(false),
		m_saveAllowed(false)
{
	bool loadLib = false;
	
	
	m_p = new Private();
	CHECK_PTR(m_p);
	
	if (!g_appList)
	{
		g_appList = new KSimusAppList();
		CHECK_PTR(g_appList);
	}
	g_appList->append(this);
	
//	KSIMDEBUG(QString("LanguageList: %1").arg(KGlobal::locale()->languageList().join(" ")));
//	KSIMDEBUG(QString("Languages: %1").arg(KGlobal::locale()->languages()));
//	KGlobal::locale()->setLanguage("C");

	if (!g_library)
	{
		g_library = new Library();
		CHECK_PTR(g_library);
		loadLib = true;
	}

	config=kapp->config();

	
	
	///////////////////////////////////////////////////////////////////
	// call inits to invoke all other construction parts
	
	initStatusBar();
	initActions();
	initDocument();
	initView();
	
	readOptions();

	///////////////////////////////////////////////////////////////////
	// disable actions at startup
	if (getDocument()->getFiles()->existCopyPastFile())
	{
		m_pastAllowed = true;
  }
	slotSetupActions();
//	fileSave->setEnabled(false);
//	fileSaveAs->setEnabled(false);
//	filePrint->setEnabled(false);
//	editCut->setEnabled(false);

	if(loadLib)
	{
		for (unsigned int i = 0; i < g_library->getMessages().count(); i++)
		{
    	LogListItem * item = new LogListItem(g_library->getMessages()[i], LOG_INFO);
    	getLogList()->append(item);
		}
	}
}

KSimusApp::~KSimusApp()
{
	g_appList->remove(this);
	
	delete m_p;
}

void KSimusApp::initActions()
{
	fileNewWindow = new KAction(i18n("New &Window"), 0, 0, this, SLOT(slotFileNewWindow()), actionCollection(),"file_new_window");
	fileNew = KStdAction::openNew(this, SLOT(slotFileNew()), actionCollection());
	fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
	fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
	fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
	fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
	fileClose = KStdAction::close(this, SLOT(slotFileClose()), actionCollection());
	filePrint = KStdAction::print(this, SLOT(slotFilePrint()), actionCollection());
	fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
	editUndo = KStdAction::undo(this, SLOT(slotEditUndo()), actionCollection());
	editRedo = KStdAction::redo(this, SLOT(slotEditRedo()), actionCollection());
	
	editDelete = new KAction(i18n("&Delete"), 0, this, SLOT(slotEditDelete()), actionCollection(), "edit_delete");
	editCut = KStdAction::cut(this, SLOT(slotEditCut()), actionCollection());
	editCopy = KStdAction::copy(this, SLOT(slotEditCopy()), actionCollection());
	editPaste = KStdAction::paste(this, SLOT(slotEditPaste()), actionCollection());
  editInsertModule = new KAction(i18n("&Insert Module..."), 0, this, SLOT(slotEditInsertModule()), actionCollection(), "edit_insert_module");

		
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());
	viewLogView = new KToggleAction(i18n("&Log View"), CTRL+Key_B, this, SLOT(slotViewToggleLog()), actionCollection(), "view_toggle_log");
	viewTreeView = new KToggleAction(i18n("&Component Supplier"), CTRL+Key_T, this, SLOT(slotViewToggleTree()), actionCollection(), "view_toggle_tree");
	viewTreeFold = new KAction(i18n("&Fold Supplier"), 0, this, SLOT(slotViewFoldTree()), actionCollection(), "view_tree_fold");
	viewTreeUnfold = new KAction(i18n("&Unfold Supplier"), 0, this, SLOT(slotViewUnfoldTree()), actionCollection(), "view_tree_unfold");
	
	viewCostmap = new KToggleAction(i18n("&Display Costmap"), 0, this, SLOT(slotViewToggleCostmap()), actionCollection(), "view_toggle_costmap");
	settingMapSize = new KAction(i18n("Map &Sizes"), 0, this, SLOT(slotSettingMapSize()), actionCollection(), "setting_mapSize");
	settingTiming = new KAction(i18n("&Timing"), 0, this, SLOT(slotSettingTiming()), actionCollection(), "setting_timing");
	settingGrid = new KAction(i18n("&Grid"), 0, this, SLOT(slotSettingGrid()), actionCollection(), "setting_grid");
  settingPackageFiles  = new KAction(i18n("&Package Files"), 0, this, SLOT(slotSettingPackageFiles()), actionCollection(), "setting_packageFiles");
	
	executeReset = new KAction(i18n("&Reset"), 0, this, SLOT(slotExecuteReset()), actionCollection(), "execute_reset");
	executeStart = new KAction(i18n("&Start"), "exec", 0, this, SLOT(slotExecuteStart()), actionCollection(), "execute_start");
	executeStop = new KAction(i18n("S&top"), "stop", 0, this, SLOT(slotExecuteStop()), actionCollection(), "execute_stop");
	executePause = new KToggleAction(i18n("&Pause"), 0, this, SLOT(slotExecutePause()), actionCollection(), "execute_pause");
	
//	testAction = new KAction(i18n("Test &Action"), 0, 0, this, SLOT(slotTestAction()), actionCollection(),"test_action");

	fileNewWindow->setToolTip(i18n("Opens a new application window"));
	fileNew->setToolTip(i18n("Creates a new document"));
	fileOpen->setToolTip(i18n("Opens an existing document"));
	fileOpenRecent->setToolTip(i18n("Opens a recently used file"));
	fileSave->setToolTip(i18n("Saves the actual document"));
	fileSaveAs->setToolTip(i18n("Saves the actual document as..."));
	fileClose->setToolTip(i18n("Closes the actual document"));
	filePrint ->setToolTip(i18n("Prints out the actual document"));
	fileQuit->setToolTip(i18n("Quits the application"));
	editDelete->setToolTip(i18n("Deletes the selected section"));
	editCut->setToolTip(i18n("Cuts the selected section and puts it to the clipboard"));
	editCopy->setToolTip(i18n("Copies the selected section to the clipboard"));
	editPaste->setToolTip(i18n("Pastes the clipboard contents to actual position"));
	editInsertModule->setToolTip(i18n("Insert a module to the current view"));
	viewToolBar->setToolTip(i18n("Enables/disables the toolbar"));
	viewStatusBar->setToolTip(i18n("Enables/disables the statusbar"));

//	testAction->setToolTip(i18n("Executes a test action"));
	
	viewLogView->setToolTip(i18n("Enables/disables the log view"));
	viewLogView->setChecked(true);
	viewTreeView->setToolTip(i18n("Enables/disables the component supplier"));
	viewTreeView->setChecked(true);
	viewTreeFold->setToolTip(i18n("Folds component supplier"));
	viewTreeUnfold->setToolTip(i18n("Unfolds component supplier"));
	viewCostmap->setToolTip(i18n("Enables/disables display of the cost map"));

	executeReset->setToolTip(i18n("Resets simulation"));
	executeStart->setToolTip(i18n("Starts simulation"));
	executeStop->setToolTip(i18n("Stops simulation"));
	executePause->setToolTip(i18n("Pause simulation"));
	executePause->setChecked(false);

	settingMapSize->setToolTip(i18n("Setup Schematic und User Interface size"));
	settingTiming->setToolTip(i18n("Setup simulation timing"));
	settingGrid->setToolTip(i18n("Setup grid"));
	settingPackageFiles->setToolTip(i18n("Select packages"));

	connect(actionCollection(),SIGNAL(actionStatusText(const QString &)),SLOT(slotStatusHelpMsg(const QString &)));
	actionCollection()->setHighlightingEnabled(true);
	// use the absolute path to your ksimusui.rc file for testing purpose in createGUI();
	createGUI();

}


void KSimusApp::initStatusBar()
{
  QString s;
  s = i18n("Time")+" 000.000msec ";
  ///////////////////////////////////////////////////////////////////
  // STATUSBAR
  // TODO: add your own items you need for displaying current application status.
  statusBar()->insertItem(i18n("Ready."), ID_STATUS_MSG,1);
  statusBar()->setItemAlignment(ID_STATUS_MSG, AlignLeft | AlignVCenter);
  statusBar()->insertFixedItem(s, ID_TIME_LABEL, true);
}

void KSimusApp::initDocument()
{
  doc = new KSimusDoc(this);
  connect(doc->getUndo(),SIGNAL(undoAllowed(bool)),SLOT(slotUndoAllowed(bool)));
  connect(doc->getUndo(),SIGNAL(redoAllowed(bool)),SLOT(slotRedoAllowed(bool)));
  doc->newDocument();
	slotSimulationTimeChanged();
}

void KSimusApp::initView()
{ 
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.
	baseWin = new BaseWindow(this, "BaseWindow");
	setCentralWidget(baseWin);
	setCaption(getDocument()->URL().fileName(),false);

	connect(baseWin->getTreeWidget(), SIGNAL(signalHideMe()), this, SLOT(slotViewHideTree()));

	QBoxLayout * lay = new QHBoxLayout(baseWin->getWorkingWidget());
	m_p->tabWidget = new QTabWidget(baseWin->getWorkingWidget());
	lay->addWidget(m_p->tabWidget);

	// Sheet view
	m_p->sheetView = new KSimusView(this, m_p->tabWidget);
	getDocument()->addView(m_p->sheetView);

	connect(m_p->sheetView->getEditor(), SIGNAL(editorModeChanged(int)),
			this, SLOT(slotEditorModeChanged(int)));
	
	connect(m_p->sheetView->getEditor(),SIGNAL(deleteAllowed(bool)),SLOT(slotDeleteAllowed(bool)));
	connect(m_p->sheetView->getEditor(),SIGNAL(cutAllowed(bool)),SLOT(slotCutAllowed(bool)));
	connect(m_p->sheetView->getEditor(),SIGNAL(copyAllowed(bool)),SLOT(slotCopyAllowed(bool)));
	connect(m_p->sheetView->getEditor(),SIGNAL(signalStatusMsg(const QString &)),SLOT(slotStatusMsg(const QString &)));
	connect(m_p->sheetView->getEditor(),SIGNAL(signalStatusHelpMsg(const QString &)),SLOT(slotStatusHelpMsg(const QString &)));

	
	
	m_p->tabWidget->insertTab(m_p->sheetView, i18n("Schematic"), ID_SHEET_VIEW);

	// User view
	m_p->userView = new KSimusView(this, m_p->tabWidget);
	m_p->userView->getEditor()->setEditorView(EV_USERVIEW);
	getDocument()->addView(m_p->userView);

	connect(m_p->userView->getEditor(), SIGNAL(editorModeChanged(int)),
			this, SLOT(slotEditorModeChanged(int)));
	
	connect(m_p->userView->getEditor(),SIGNAL(deleteAllowed(bool)),SLOT(slotDeleteAllowed(bool)));
	connect(m_p->userView->getEditor(),SIGNAL(cutAllowed(bool)),SLOT(slotCutAllowed(bool)));
	connect(m_p->userView->getEditor(),SIGNAL(copyAllowed(bool)),SLOT(slotCopyAllowed(bool)));
	connect(m_p->userView->getEditor(),SIGNAL(signalStatusMsg(const QString &)),SLOT(slotStatusMsg(const QString &)));
	connect(m_p->userView->getEditor(),SIGNAL(signalStatusHelpMsg(const QString &)),SLOT(slotStatusHelpMsg(const QString &)));

	m_p->tabWidget->insertTab(m_p->userView, i18n("User Interface"), ID_USER_VIEW);

	
	// Module Dialog
	m_p->moduleDialog = new ModuleDialog(getDocument()->getContainer(), m_p->tabWidget);

	m_p->tabWidget->insertTab(m_p->moduleDialog, i18n("Module"), ID_MODULE_VIEW);

	
	connect(m_p->tabWidget, SIGNAL(currentChanged(QWidget *)), SLOT(slotViewChanged(QWidget *)));
}

void KSimusApp::openDocumentFile(const KURL& url)
{
  slotStatusMsg(i18n("Opening file..."));

  doc->openDocument( url);
  fileOpenRecent->addURL( url );
  slotStatusMsg(i18n("Ready."));
}


KSimusDoc *KSimusApp::getDocument() const
{
   return doc;
}

KSimEditor * KSimusApp::getActiveEditor() const
{
	return getDocument()->getActiveEditor();
}

/** returns a pointer to the log list widget */	
LogList * KSimusApp::getLogList() const
{
  if(baseWin)
	  return baseWin->getLogWidget();
	return 0;
}

void KSimusApp::saveOptions()
{	
	config->setGroup("General Options");
	config->writeEntry("First Start",false);
	config->writeEntry("Geometry", size());
	config->writeEntry("Show Toolbar", viewToolBar->isChecked());
	config->writeEntry("Show Statusbar",viewStatusBar->isChecked());
	config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
	fileOpenRecent->saveEntries(config,"Recent Files");
	
	config->writeEntry("Display Costmap",viewCostmap->isChecked());
}


void KSimusApp::readOptions()
{
	
	config->setGroup("General Options");
	
	bool firstRead = config->readBoolEntry("First Start",true);
	
	if (firstRead)
	{
		// Some presets
		QString group(config->group());
	
		// Default package dirs
		config->setGroup("Packages");
		KStandardDirs dirs;
		QStringList packList = dirs.findDirs("lib","");
		unsigned int i;
		for (i=0; i<packList.count(); i++)
		{
			packList[i] += "ksimus/";
		}
		
		config->writeEntry("Directories", packList);

		config->setGroup(group);
	}	

	// bar status settings
	bool bViewToolbar = config->readBoolEntry("Show Toolbar", true);
	viewToolBar->setChecked(bViewToolbar);
	slotViewToolBar();

	bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);
	viewStatusBar->setChecked(bViewStatusbar);
	slotViewStatusBar();


	// bar position settings
	KToolBar::BarPosition toolBarPos;
	toolBarPos=(KToolBar::BarPosition) config->readNumEntry("ToolBarPos", KToolBar::Top);
	toolBar("mainToolBar")->setBarPos(toolBarPos);
	
	// initialize the recent file list
	fileOpenRecent->loadEntries(config,"Recent Files");

	QSize size=config->readSizeEntry("Geometry");
	if(!size.isEmpty())
	{
		resize(size);
	}

	viewCostmap->setChecked(config->readBoolEntry("Display Costmap",false));
  doc->setCostMapVisible(viewCostmap->isChecked());
}

void KSimusApp::saveProperties(KConfig *_cfg)
{
  if(doc->URL().fileName()!=i18n("Untitled") && !doc->isModified())
  {
    // saving to tempfile not necessary

  }
  else
  {
    KURL url=doc->URL();	
    _cfg->writeEntry("filename", url.url());
    _cfg->writeEntry("modified", doc->isModified());
    QString tempname = kapp->tempSaveName(url.url());
    QString tempurl= KURL::encode_string(tempname);
    KURL _url(tempurl);
    doc->saveDocument(_url);
  }
}


void KSimusApp::readProperties(KConfig* _cfg)
{
  QString filename = _cfg->readEntry("filename", "");
  KURL url(filename);
  bool modified = _cfg->readBoolEntry("modified", false);
  if(modified)
  {
    bool canRecover;
    QString tempname = kapp->checkRecoverFile(filename, canRecover);
    KURL _url(tempname);
  	
    if(canRecover)
    {
      doc->openDocument(_url);
      doc->setModified();
      setCaption(_url.fileName(),true);
      QFile::remove(tempname);
    }
  }
  else
  {
    if(!filename.isEmpty())
    {
      doc->openDocument(url);
      setCaption(url.fileName(),false);
    }
  }
}		

bool KSimusApp::queryClose()
{
  return doc->saveModified();
}

bool KSimusApp::queryExit()
{
  saveOptions();
  return true;
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void KSimusApp::slotFileNewWindow()
{
  slotStatusMsg(i18n("Opening a new application window..."));
	
  KSimusApp *new_window= new KSimusApp();
  new_window->show();

  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotFileNew()
{
	getActiveEditor()->setEditorMode(EM_SELECT);
  slotStatusMsg(i18n("Creating new document..."));

  if(!doc->saveModified())
  {
     // here saving wasn't successful

  }
  else
  {	
    doc->newDocument();		
    setCaption(doc->URL().fileName(), false);
    fileOpenRecent->setCurrentItem(-1);
  }

  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotFileOpen()
{
  slotStatusMsg(i18n("Opening file..."));
	
  if(!doc->saveModified())
  {
     // here saving wasn't successful

  }
  else
  {	
		KFileDialog * dia = new KFileDialog (":<documentPath>",
        i18n("*.sim|KSimus files (*.sim)\n*|All files"), this, 0, true);

	  dia->setCaption(i18n("Open File..."));
  	dia->exec();

	  KURL url(dia->selectedURL());

  	delete dia;

    if(!url.isEmpty())
    {
      doc->openDocument(url);
      setCaption(url.fileName(), false);
      fileOpenRecent->addURL( url );
    }
  }
  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotFileOpenRecent(const KURL& url)
{
  slotStatusMsg(i18n("Opening file..."));
	
  if(!doc->saveModified())
  {
     // here saving wasn't successful
  }
  else
  {
    doc->openDocument(url);
    setCaption(url.fileName(), false);
  }

  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotFileSave()
{
	if (doc->isNamed())
	{
		slotStatusHelpMsg(i18n("Saving file..."));
	
		doc->saveDocument(doc->URL());
	}
	else
	{
		// Save As Dialog if not named
		slotFileSaveAs();
	}
}

void KSimusApp::slotFileSaveAs()
{
  slotStatusMsg(i18n("Saving file with a new filename..."));

	KFileDialog * dia = new KFileDialog (":<documentPath>",
        i18n("*.sim|KSimus files (*.sim)\n*|All files"), this, 0, true);

  dia->setCaption(i18n("Save as..."));
  dia->exec();

  KURL url(dia->selectedURL());
  QString filter(dia->currentFilter());

  delete dia;

  if(!url.isEmpty())
  {
    QString filename(url.fileName());

    // Append extension, if no is present
    if ((filename.right(4) != ".sim") && (filter == "*.sim"))
    {
    	filename += ".sim";
    	url.setFileName(filename);
    }

    doc->saveDocument(url);
    fileOpenRecent->addURL(url);
    setCaption(url.fileName(),doc->isModified());
  }

  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotFileClose()
{
  slotStatusMsg(i18n("Closing file..."));
	
  close();

  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotFilePrint()
{
  slotStatusMsg(i18n("Printing..."));

  KPrinter printer;
  if (printer.setup(this))
  {
    doc->getActiveView()->print(&printer);
  }

  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotFileQuit()
{
  slotStatusMsg(i18n("Exiting..."));
  saveOptions();
  // close the first window, the list makes the next one the first again.
  // This ensures that queryClose() is called on each window to ask for closing
  KMainWindow* w;
  if(memberList)
  {
    for(w=memberList->first(); w!=0; w=memberList->first())
    {
      // only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
      // the window and the application stay open.
      if(!w->close())
	break;
    }
  }	
  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotEditUndo()
{
  setFocus(); // For KSimLineEdit (Last undo)
  getDocument()->undo();
  if (m_p->currentView == APP_MODULE_VIEW)
  {
		m_p->moduleDialog->setup();
  }
}

void KSimusApp::slotEditRedo()
{
  getDocument()->redo();
  if (m_p->currentView == APP_MODULE_VIEW)
  {
		m_p->moduleDialog->setup();
  }
}

void KSimusApp::slotEditDelete()
{
  slotStatusHelpMsg(i18n("Deleting selection..."));
  getActiveEditor()->deleteSelection();
}

void KSimusApp::slotEditCut()
{
  slotStatusHelpMsg(i18n("Cutting selection..."));
  getActiveEditor()->cutSelection();
  editPaste->setEnabled(true);
  m_pastAllowed = true;
}

void KSimusApp::slotEditCopy()
{
  slotStatusHelpMsg(i18n("Copying selection to clipboard..."));
  getActiveEditor()->copySelection();
  editPaste->setEnabled(true);
  m_pastAllowed = true;
}

void KSimusApp::slotEditPaste()
{
  slotStatusHelpMsg(i18n("Inserting clipboard contents..."));
  getActiveEditor()->setEditorMode(EM_PAST);
}

void KSimusApp::slotEditInsertModule()
{
  slotStatusMsg(i18n("Inserting a module..."));
	QString file;

	file=KFileDialog::getOpenFileName(":<modulePath>",
                                    i18n("*.sim|KSimus files (*.sim)\n*|All files"),
                                    this, i18n("Insert Module..."));
	if(!file.isEmpty())
	{
		QFileInfo fileInfo(file);
		m_p->moduleFile = file;
		slotSelectCI(&ModuleBaseInfo);
	}

  slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotViewToolBar()
{
  slotStatusHelpMsg(i18n("Toggling toolbar..."));
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off
  if(!viewToolBar->isChecked())
  {
    toolBar("mainToolBar")->hide();
  }
  else
  {
    toolBar("mainToolBar")->show();
  }		
}

void KSimusApp::slotViewStatusBar()
{
  slotStatusHelpMsg(i18n("Toggle the statusbar..."));
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off
  if(!viewStatusBar->isChecked())
  {
    statusBar()->hide();
  }
  else
  {
    statusBar()->show();
  }
}

void KSimusApp::slotViewToggleLog()
{
	slotStatusHelpMsg(i18n("Toggle the log view..."));
	///////////////////////////////////////////////////////////////////
	//turn log view on or off
	if(!viewLogView->isChecked())
	{
		baseWin->hideLogWidget();
	}
	else
	{
		baseWin->showLogWidget();
	}
}

void KSimusApp::slotViewHideLog()
{
	slotStatusHelpMsg(i18n("Hide the log view..."));
	///////////////////////////////////////////////////////////////////
	//turn log view off
	if(viewLogView->isChecked())
	{
		baseWin->hideLogWidget();
		viewLogView->setChecked(false);
	}
}

void KSimusApp::slotViewToggleTree()
{
	slotStatusHelpMsg(i18n("Toggle the component supplier..."));
	///////////////////////////////////////////////////////////////////
	//turn component supplier on or off
	if(!viewTreeView->isChecked())
	{
		baseWin->hideTreeWidget();
		viewTreeFold->setEnabled(false);
		viewTreeUnfold->setEnabled(false);
	}
	else
	{
		baseWin->showTreeWidget();
		viewTreeFold->setEnabled(true);
		viewTreeUnfold->setEnabled(true);
	}
}

void KSimusApp::slotViewHideTree()
{
	slotStatusHelpMsg(i18n("Hide the component supplier..."));
	///////////////////////////////////////////////////////////////////
	//turn component supplier off
	if(viewTreeView->isChecked())
	{
		baseWin->hideTreeWidget();
		viewTreeFold->setEnabled(false);
		viewTreeUnfold->setEnabled(false);
		viewTreeView->setChecked(false);
	}
}

void KSimusApp::slotViewFoldTree()
{
	slotStatusHelpMsg(i18n("Fold component supplier..."));
	///////////////////////////////////////////////////////////////////
	// fold component supplier
	baseWin->getTreeWidget()->slotFoldTree();
}

void KSimusApp::slotViewUnfoldTree()
{
	slotStatusHelpMsg(i18n("Unfold component supplier..."));
	///////////////////////////////////////////////////////////////////
	// fold component supplier
	baseWin->getTreeWidget()->slotUnfoldTree();
}

void KSimusApp::slotViewToggleCostmap()
{
//	slotStatusMsg(i18n("Display/Hide the cost map..."));
	getDocument()->setCostMapVisible(viewCostmap->isChecked());
	///////////////////////////////////////////////////////////////////
	//turn log view on or off
	slotStatusMsg(i18n("Ready."));
}



void KSimusApp::slotSettingMapSize()
{
	slotStatusMsg(i18n("Setup map sizes..."));

  KDialogBase * dia;
	dia = new KDialogBase(KDialogBase::TreeList,
					i18n("Map Sizes"),
					KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
 					KDialogBase::Ok,
 					this);
 	QVBox * page;
	MapSizesWidget * wid;
	page = dia->addVBoxPage(i18n("Map Sizes"));
	wid = new MapSizesWidget(getDocument(), page, "Information");
	connect(dia, SIGNAL(okClicked()), wid, SLOT(slotAccept()));
	connect(dia, SIGNAL(defaultClicked()), wid, SLOT(slotDefault()));
	connect(dia, SIGNAL(cancelClicked()), wid, SLOT(slotCancel()));
  dia->exec();
	

	delete dia;
	slotStatusMsg(i18n("Ready."));
}


void KSimusApp::slotSettingTiming()
{
	slotStatusMsg(i18n("Setup simulation timing..."));

  KDialogBase * dia;
	dia = new KDialogBase(KDialogBase::TreeList,
					i18n("Timings"),
					KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
 					KDialogBase::Ok,
 					this);
 	QVBox * page;
	SimulationTimingWidget * wid;
	page = dia->addVBoxPage(i18n("Timing"));
	wid = new SimulationTimingWidget(getDocument()->getTiming(), page, "Information");
	connect(dia, SIGNAL(okClicked()), wid, SLOT(slotAccept()));
	connect(dia, SIGNAL(defaultClicked()), wid, SLOT(slotDefault()));
	connect(dia, SIGNAL(cancelClicked()), wid, SLOT(slotCancel()));
//	connect(wid, SIGNAL(signalChangeData()), dia, SLOT(slotDataChanged()));
  dia->exec();
	

	delete dia;
	slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotSettingGrid()
{
	slotStatusMsg(i18n("Setup grid..."));

  KSimGrid * grid;
  if (getCurrentView() == APP_SHEET_VIEW)
  {
  	grid = getDocument()->getSheetGrid();
  }
  else if (getCurrentView() == APP_USER_VIEW)
  {
  	grid = getDocument()->getUserGrid();
  }
  else
  {
  	KSIMDEBUG("Grid Setting - bad view type");
  	grid = 0;
  }

  if (grid)
  {
	  KSimGridDialog * dia = new KSimGridDialog(grid);
	  dia->exec();
	  getDocument()->slotUpdateAllViews(0);
		delete dia;
	}
	slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotSettingPackageFiles()
{
	slotStatusMsg(i18n("Setup package files..."));

  KSimPackageFileDialog * dia = new KSimPackageFileDialog();
	
	// Load last size
	QString group(config->group());
	config->setGroup("Packages/File Dialog");
	QSize size=config->readSizeEntry("Geometry");
	config->setGroup(group);
	if(!size.isEmpty())
	{
		dia->resize(size);
	}

	// Execute dialog
	dia->exec();
	
	// Save size
	config->setGroup("Packages/File Dialog");
	config->writeEntry("Geometry", dia->size());
	config->setGroup(group);
	
	// Delete dialog
	delete dia;
	slotStatusMsg(i18n("Ready."));
}

void KSimusApp::slotExecuteReset()
{
	slotStatusHelpMsg(i18n("Resets simulation..."));

	getDocument()->simulationReset();
}

void KSimusApp::slotExecuteStart()
{
	slotStatusHelpMsg(i18n("Starts simulation..."));

	getDocument()->simulationStart();

	baseWin->getTreeWidget()->setEnabled(false);
	slotSetupActions();
}
void KSimusApp::slotExecuteStop()
{
	slotStatusHelpMsg(i18n("Stops simulation..."));

	executePause->setChecked(false);
	
	getDocument()->simulationStop();

	slotSetupActions();
	baseWin->getTreeWidget()->setEnabled(true);
}
void KSimusApp::slotExecutePause()
{
	slotStatusMsg(i18n("Pause simulation..."));
	slotSetupActions();
	
	slotStatusMsg(i18n("Ready."));
}

	
void KSimusApp::slotStatusMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message permanently
  statusBar()->clear();
  statusBar()->changeItem(text, ID_STATUS_MSG);
}

void KSimusApp::slotStatusHelpMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  statusBar()->message(text, 2000);
//  KSIMDEBUG(text);
}


void KSimusApp::slotDeleteAllowed(bool ena)
{
	m_deleteAllowed = ena;
	editDelete->setEnabled(ena);
}

void KSimusApp::slotCutAllowed(bool ena)
{
	m_cutAllowed = ena;
	editCut->setEnabled(ena);
}

void KSimusApp::slotCopyAllowed(bool ena)
{
	m_copyAllowed = ena;
	editCopy->setEnabled(ena);
}

void KSimusApp::slotUndoAllowed(bool ena)
{
	m_undoAllowed = ena;
	editUndo->setEnabled(ena);
}

void KSimusApp::slotRedoAllowed(bool ena)
{
	m_redoAllowed = ena;
	editRedo->setEnabled(ena);
}
void KSimusApp::slotSaveAllowed(bool ena)
{
	m_saveAllowed = ena;
	fileSave->setEnabled(ena);
	setCaption(doc->URL().fileName(), ena);
}

void KSimusApp::slotTestAction()
{
	slotStatusMsg(i18n("Test Action..."));
	
	

	slotStatusMsg(i18n("Ready."));
}

/** Receives the component info for inserting component and calls the function slotSelctCI()
	of the active editor.
 */
void KSimusApp::slotSelectCI(const ComponentInfo *ci)
{
	m_p->lastSelectedCI = ci;
	getActiveEditor()->slotSelectCI(ci);
}

void KSimusApp::slotEditorModeChanged(int mode)
{
	switch((EditorModeType)mode)
	{
		case EM_SELECT:
			slotStatusMsg(i18n("Ready."));
			break;
   	
		case EM_INSERT:
			{
				QString msg;
				msg.sprintf(i18n("Insert Component '%1' ...").arg(i18n(m_p->lastSelectedCI->getName().latin1())));
				slotStatusMsg(msg);
			}
			break;
		
		case EM_INSERT_MOVE:
			break;
		
		case EM_PAST:
			slotStatusMsg(i18n("Past Components..."));
			break;
		
		case EM_PAST_MOVE:
			break;
		
		case EM_MOVE_OR_SELECT:
			break;
		
		case EM_MOVE:
			slotStatusMsg(i18n("Move Components..."));
			break;
		
		case EM_MOVE_COPY:
			slotStatusMsg(i18n("Copy Components..."));
			break;
		
		case EM_RECT_SELECT:
		case EM_SINGLE_SELECT:
			slotStatusMsg(i18n("Select Components..."));
			break;
		
		case EM_SPECIAL:
			break;
		
		case EM_COMP_RESIZE_B:
		case EM_COMP_RESIZE_F:
			slotStatusMsg(i18n("Resize Component..."));
			break;
		
		case EM_WIRE:
			slotStatusMsg(i18n("Connect Components..."));
			break;
		
		case EM_RESIZE_MAP:
			slotStatusMsg(i18n("Resize Map..."));
			break;
	}
	
	if (((EditorModeType)mode != EM_INSERT) && ((EditorModeType)mode != EM_INSERT_MOVE))
	{
		baseWin->getTreeWidget()->slotUnselect();
	}
}


void KSimusApp::slotViewChanged(QWidget * wid)
{
	getActiveEditor()->setEditorMode(EM_SELECT);
	if(wid == m_p->sheetView)
	{
		m_p->currentView = APP_SHEET_VIEW;
		m_p->sheetView->show();
		m_p->userView->hide();
		getDocument()->setActiveView(m_p->sheetView);
		emit signalViewChanged(APP_SHEET_VIEW);
	}
	else if(wid == m_p->userView)
	{
		m_p->currentView = APP_USER_VIEW;
		m_p->userView->show();
		m_p->sheetView->hide();
		getDocument()->setActiveView(m_p->userView);
		emit signalViewChanged(APP_USER_VIEW);
	}
	else if(wid == m_p->moduleDialog)
	{
		m_p->currentView = APP_MODULE_VIEW;
		m_p->moduleDialog->setup();
		m_p->userView->hide();
		m_p->sheetView->hide();
		emit signalViewChanged(APP_MODULE_VIEW);
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Widget",wid->name());
		KSIMDEBUG_VAR("Unknown Widget",wid);
	}
	slotSetupActions();
}

void KSimusApp::slotSimulationTimeChanged()
{
  QString s;
  s = i18n("Time") + ": " + getDocument()->getTimeString();
  statusBar()->changeItem(s, ID_TIME_LABEL);
}

const QString & KSimusApp::getModuleFile() const
{
	return m_p->moduleFile;
}


const char * KSimusApp::convertViewType(eAppViewType view)
{
	return currentViewDict.find(view);
}


void KSimusApp::setCurrentView(eAppViewType newView)
{
	if(m_p->tabWidget)
	{
		switch(newView)
		{
			default:
				KSIMDEBUG_VAR("Unknown eAppViewType",newView);
				// no break here !!!
			
			case APP_SHEET_VIEW:
				m_p->tabWidget->showPage(m_p->sheetView);
				break;
				
			case APP_USER_VIEW:
				m_p->tabWidget->showPage(m_p->userView);
				break;
				
			case APP_MODULE_VIEW:
				m_p->tabWidget->showPage(m_p->moduleDialog);
				break;
		}
	}
}

void KSimusApp::setCurrentView(const char * newView)
{
	setCurrentView(currentViewDict.find(newView,APP_SHEET_VIEW));
}

eAppViewType KSimusApp::getCurrentView() const
{
	return m_p->currentView;
}

const char * KSimusApp::getCurrentViewString() const
{
	return convertViewType(getCurrentView());
}

void KSimusApp::slotSetupActions()
{
	bool running = getDocument()->isSimulationRunning();
	bool stopped = !running;
	bool editView = (getCurrentView() != APP_MODULE_VIEW);
	
	fileNewWindow->setEnabled(stopped);
	fileNew->setEnabled(stopped);
	fileOpen->setEnabled(stopped);
	fileOpenRecent->setEnabled(stopped);
	fileSave->setEnabled(stopped && m_saveAllowed);
	fileSaveAs->setEnabled(stopped);
	fileClose->setEnabled(true);
	filePrint->setEnabled(true);
	fileQuit->setEnabled(true);
	
	editUndo->setEnabled(stopped && m_undoAllowed);
	editRedo->setEnabled(stopped && m_redoAllowed);
	editDelete->setEnabled(stopped && editView && m_deleteAllowed);
	editCut->setEnabled(stopped && editView && m_cutAllowed);
	editCopy->setEnabled(stopped && editView && m_copyAllowed);
	editPaste->setEnabled(stopped && editView && m_pastAllowed);
	editInsertModule->setEnabled(stopped && editView);
	
//	viewToolBar->setEnabled(ena);		always enabled
//	viewStatusBar->setEnabled(ena);		always enabled
//	viewLogView->setEnabled(ena);		always enabled
//	viewTreeView->setEnabled(ena);		always enabled
	viewCostmap->setEnabled(stopped);
	settingTiming->setEnabled(stopped);
	settingGrid->setEnabled(stopped && editView);
	
//	executeReset->setEnabled(ena);		always enabled
	executeStart->setEnabled(stopped);
	executeStop->setEnabled(running);
	executePause->setEnabled(false);

//	testAction->setEnabled(true);		always enabled
}
