/***************************************************************************
                          ksimusdoc.h  -  description
                             -------------------
    begin                : Sat Dec 23 15:44:46 CET 2000
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

#ifndef KSIMUSDOC_H
#define KSIMUSDOC_H

// include files for QT
#include <qobject.h>
#include <qstring.h>
#include <qptrlist.h>

// include files for KDE
#include <kurl.h>

// forward declaration of the KSimus classes
class KSimData;
class KSimusView;
class KSimusApp;
class KSimEditor;
class KSimUndo;
class KSimFile;
class KSimGrid;
class KSimTimeServer;
class CompContainer;
class Component;
class SimulationTiming;
class SimulationExecute;

/**	KSimusDoc provides a document object for a document-view model.
  *
  * The KSimusDoc class provides a document object that can be used in conjunction with the classes KSimusApp and KSimusView
  * to create a document-view model for standard KDE applications based on KApplication and KMainWindow. Thereby, the document object
  * is created by the KSimusApp instance and contains the document structure with the according methods for manipulation of the document
  * data by KSimusView objects. Also, KSimusDoc contains the methods for serialization of the document data from and to files.
  *
  * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team. 	
  * @version KDevelop version 1.2 code generation
  */
class KSimusDoc : public QObject
{
	Q_OBJECT

class Private;

public:
	/** Constructor for the fileclass of the application */
	KSimusDoc(QWidget *parent, const char *name=0);
	/** Destructor for the fileclass of the application */
	~KSimusDoc();

	/** adds a view to the document which represents the document contents. Usually this is your main view. */
	void addView(KSimusView *view);
	/** removes a view from the list of currently connected views */
	void removeView(KSimusView *view);
	/** sets the modified flag for the document after a modifying action on the view connected to the document.*/
	void setModified(bool _m=true);
	/** returns if the document is modified or not. Use this to determine if your document needs saving by the user on closing.*/
	bool isModified() const;
	/** sets the named flag for the document after saving or loading document.*/
	void setNamed(bool _m=true);
	/** returns if the document is named or not. Use this to determine if your document needs the dialog "save as".*/
	bool isNamed() const;
	/** "save modified" - asks the user for saving if the document is modified */
	bool saveModified();
	/** deletes the document's contents */
	void deleteContents();
	/** initializes the document generally */
	bool newDocument();
	/** closes the acutal document */
	void closeDocument();
	/** loads the document by filename and format and emits the updateViews() signal */
	bool openDocument(const KURL& url, const char *format=0);
	/** saves the document under filename and format.*/	
	bool saveDocument(const KURL& url, const char *format=0);
	/** returns the KURL of the document */
	const KURL& URL() const;
	/** sets the URL of the document */
	void setURL(const KURL& url);
	
	/** Returns a pointer to the KSimusApp */
	KSimusApp * getApp() const;
	
	/** Returns the grid information of the sheet view */
	KSimGrid * getSheetGrid() const { return m_sheetGrid; };
	/** Returns the grid information of the user view */
	KSimGrid * getUserGrid() const { return m_userGrid; };
	
	/** Returns the undo object */
	KSimUndo * getUndo() const { return m_undo; };
	/** Returns the file management object */
	KSimFile * getFiles() const {return m_files;};
	/** Returns the file management object */
	CompContainer * getContainer() const {return m_container;};
	/** Returns the active view */
	KSimusView * getActiveView() const;
	/** Returns the active editor */
	KSimEditor * getActiveEditor() const;
	/** Set active view */
	void setActiveView(KSimusView * view);
	/** Returns a list of all views */
	QPtrList<KSimusView> * getViewList() const { return m_pViewList; };
	
	void addComponentToEditor(Component * comp);
	void undo();
	void redo();

	/** Load document property */
	void loadProperty(KSimData & config);
	/** Save document property */
	void saveProperty(KSimData & config) const;
	
	/** True, if cost map is displayed */
	bool isCostMapVisible() const;
	/** Display cost map */
	void setCostMapVisible(bool enable);
	
	/** Resets simulation */
	void simulationReset(void);
	/** Starts simulation */
	void simulationStart(void);
	/** Stops simulation */
	void simulationStop(void);
	/** Pause simulation if "pause" is set, or restart simulation if not set */
	void simulationPause(bool pause);
	/** Returns true, if simulation is active.
		*/
	bool isSimulationRunning() const { return m_simRunning; };
	/** Returns true, if simulation is paused.
		*/
	bool isSimulationPaused() const { return m_simPaused; };
	/** Starts the circuit check. Returns true if no error is found */
	bool simulationCheckCirciut();
	
	/** Returns the simulation time
	*/
	const QString getTimeString() const;
	/** Returns the simulation time
	*/
	const KSimTimeServer & getTimeServer() const;
		
	SimulationTiming & getTiming() { return *m_timing; };
	SimulationExecute & getExecute() { return *m_execute; };

	/** Sets the schematic size. */
	void setSheetSize(const QSize & newSize);
	/** Returns the schematic size. */
	QSize getSheetSize() const;
	/** Sets the user size. */
	void setUserSize(const QSize & newSize);
	/** Returns the user size. */
	QSize getUserSize() const;

	/** Simulates one step */
	void simulateExecute();
	/** Update component views during simulation */
	void simulateUpdate();
	
	
public slots:
	/** calls repaint() on all views connected to the document object and is called by the view by which the document has been changed.
	 * As this view normally repaints itself, it is excluded from the paintEvent.
	 */
	void slotUpdateAllViews(KSimusView *sender);
	
private:
	/** the modified flag of the current document */
	Private * m_p;
	bool m_modified;
	CompContainer * m_container;
	KSimUndo * m_undo;
	KSimFile * m_files;
	KSimusView * m_activeView;
	/** the list of the views currently connected to the document */
	QPtrList<KSimusView> *m_pViewList;
	KSimGrid * m_sheetGrid;
	KSimGrid * m_userGrid;
	SimulationTiming * m_timing;
	SimulationExecute * m_execute;
	bool m_named;
	bool m_simRunning;
	bool m_simPaused;

signals:
	/** The signal is emitted if the document is cleared. */
	void signalNewDoc();
	/** The signal is emitted immediatly before the simulation starts (and before components are resetted). */
	void signalStart();
	/** The signal is emitted immediatly after the simulation stops. */
	void signalStop();
	/** The signal is emitted immediatly after the simulation is paused or restartet. */
	void signalPaused(bool paused);
	
	/** The signal is emmited before all Components (and Wires) are reseted (@ref Component::reset). */
	void signalPreReset();
	/** The signal is emmited after all Components (and Wires) are reseted (@ref Component::reset). */
	void signalPostReset();
	/** The signal equivalent to @ref Component::calculate(). */
	void signalCalculate();
	/** The signal is emitted if the component view should be updated. */
	void signalUpdateView();
	/** Called before each drawing. Connect to this signal
	  * if something todo before drawing, e.g. layout things. */
	void signalPolish();
};

class KSimusDocList : public QPtrList<KSimusDoc> {};

#endif // KSIMUSDOC_H
