/***************************************************************************
                          ksimundo.cpp  -  description
                             -------------------
    begin                : Sat Jun 3 2000
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

// QT-Includes
#include <qdir.h>
#include <qfile.h>
#include <qsize.h>

// KDE-Includes
#include <ksimpleconfig.h>
#include <klocale.h>

// Project-Includes
#include "resource.h"
#include "component.h"
#include "compview.h"
#include "compcontainer.h"
#include "ksimundo.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimfile.h"
#include "ksimdebug.h"
#include "ksimeditor.h"
#include "moduledata.h"

// Forward declaration



static const char * sUndoGrp   = "/Undo Property/";
static const char * sRemove    = "RemoveList";
static const char * sDesc      = "Description";
static const char * sNoDesc    = "No description";
static const char * sModi      = "Modified";
static const char * sSelect    = "Select";
static const char * sUserSize  = "UserSize";
static const char * sSheetSize = "SheetSize";
static const char * sLastView    = "Last View";

#define SHEET_VIEW	0
#define USER_VIEW	1


#define parentDoc	((KSimusDoc*)parent())
#define container	(((KSimusDoc*)parent())->getContainer())

class KSimUndoPrivate
{
public:	
	KSimUndoPrivate()
		:	file(0),
			undoDepth(500),
			redoDepth(500),
			undoFileCounter(0),
			redoFileCounter(0),
			paused(false)
	{
		undoFileList.setAutoDelete(true);
		redoFileList.setAutoDelete(true);
	};

	~KSimUndoPrivate() {};
	
	
	KSimpleConfig * file;
	unsigned int undoDepth;
	unsigned int redoDepth;
	unsigned int undoFileCounter;
	unsigned int redoFileCounter;
	QStrList undoFileList;
	QStrList redoFileList;
	ComponentList reloadList;
	ComponentList removeList;
	bool paused;
};


KSimUndo::KSimUndo(KSimusDoc * doc)
	:	QObject(doc)
{
	m_p = new KSimUndoPrivate();
}

KSimUndo::~KSimUndo()
{
	removeUndoHistory();
	removeRedoHistory();
	
	delete m_p;
}

void KSimUndo::begin(const QString & description)
{
	removeRedoHistory();
	emit redoAllowed(false);
	begin_(description);
}
	
void KSimUndo::begin_(const QString & description)
{
	m_p->paused = false;
	emit undoAllowed(true);
	// Create new undo file name
	QDir dir = parentDoc->getFiles()->getSessionDir();
	QString * filename = new QString;
	filename->sprintf("/undo%04u.sim", m_p->undoFileCounter++);
	*filename = dir.absPath() + *filename;
	
	// Append file name to list
	m_p->undoFileList.append (*filename);
	
	// Remove old undo files
	while (m_p->undoFileList.count() > m_p->undoDepth)
	{
		if (!QFile::remove(m_p->undoFileList.first()))
		{
			KSIMDEBUG("Remove old files failed");
		}
		m_p->undoFileList.removeFirst();
	}
	
	// Write last undo file
	if (m_p->file)
	{
		KSIMDEBUG("Last undo file not saved");
		delete m_p->file;
	}
		
	// Create new undo file
	m_p->file = new KSimpleConfig(*filename);
	m_p->file->setGroup(sUndoGrp);
	m_p->file->writeEntry(sDesc, description);
	m_p->file->writeEntry(sModi, parentDoc->isModified());

	// Save selected components (The IDs!)
	const CompViewList * cvList = &parentDoc->getActiveEditor()->selected;
	QStrList numList;
	numList.setAutoDelete(true);
	FOR_EACH_COMPVIEW(it, *cvList)
	{
			QString * num = new QString();
			num->setNum(it.current()->getComponent()->getSerialNumber());
			numList.append(*num);
	}
	m_p->file->writeEntry(sSelect, numList);
	
	m_p->file->writeEntry(sLastView,parentDoc->getApp()->getCurrentViewString());
	m_p->file->setGroup("/");
}

void KSimUndo::beginRedo(const QString & description)
{
	m_p->paused = false;
	// Create new redo file name
	QDir dir = parentDoc->getFiles()->getSessionDir();
	QString * filename = new QString;
	filename->sprintf("/redo%04u.sim", m_p->redoFileCounter++);
	*filename = dir.absPath() + *filename;
	
	// Append redo file name to list
	m_p->redoFileList.append (*filename);
	
	// Remove old redo files
	while (m_p->redoFileList.count() > m_p->redoDepth)
	{
		if (!QFile::remove(m_p->redoFileList.first()))
		{
			KSIMDEBUG("Remove old files failed");
		}
		m_p->redoFileList.removeFirst();
	}
	
	// Write last undo/redo file
	if (m_p->file)
	{
		KSIMDEBUG("Last undo file not saved");
		delete m_p->file;
	}
		
	// Create new redo file
	m_p->file = new KSimpleConfig(*filename);
	m_p->file->setGroup(sUndoGrp);
	m_p->file->writeEntry(sDesc, description);
	
	// Save selected components (The IDs!)
	const CompViewList * cvList = &parentDoc->getActiveEditor()->selected;
	QStrList numList;
	numList.setAutoDelete(true);
	FOR_EACH_COMPVIEW(it, *cvList)
	{
			QString * num = new QString();
			num->setNum(it.current()->getComponent()->getSerialNumber());
			numList.append(*num);
	}
	m_p->file->writeEntry(sSelect, numList);
	
	m_p->file->writeEntry(sLastView,parentDoc->getApp()->getCurrentViewString());
	m_p->file->setGroup("/");
}

void KSimUndo::end()
{
	if (m_p->file)
	{
		// Save remove components
		QStrList numList;
		numList.setAutoDelete(true);
		while(m_p->removeList.count())
		{
			QString * num = new QString();
			num->setNum(m_p->removeList.first()->getSerialNumber());
			numList.append(*num);
			m_p->removeList.removeFirst();
		}
		m_p->file->setGroup(sUndoGrp);
		m_p->file->writeEntry(sRemove, numList);
		
		// Clear list
		m_p->reloadList.clear();
			
		// write file
		delete m_p->file;
		m_p->file = 0;
	}
	else
	{
		KSIMDEBUG("undo file not exist");
	}
}

void KSimUndo::pause(bool pause)
{
	if (pause)
	{
		if (!m_p->paused)
		{
			m_p->paused = true;
		}
		else
		{
			KSIMDEBUG("Undo tracking is already paused");
		}
	}
	else
	{
		if (m_p->paused)
		{
			m_p->paused = false;
		}
		else
		{
			KSIMDEBUG("Undo tracking is already running");
		}
	}
}
	
bool KSimUndo::isActive()
{
	return ((m_p->file != 0) & (!m_p->paused));
}

void KSimUndo::reload(const Component * comp)
{
	ComponentList dummyList;
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		if (comp && (m_p->reloadList.findRef(comp) == -1))
		{
			m_p->reloadList.append(comp);
			dummyList.append(comp);
			// Save component
			parentDoc->getContainer()->saveComponents(*m_p->file, &dummyList);
		}
	}
}

void KSimUndo::reload(const ComponentList * compList)
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		FOR_EACH_COMP(it, *compList)
		{
			reload(it.current());
		}
	}
}

void KSimUndo::reload(const CompViewList * cvList)
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		FOR_EACH_COMPVIEW(it, *cvList)
		{
			reload(it.current()->getComponent());
		}
	}
}

void KSimUndo::remove(const Component * comp)
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		if (comp && (m_p->removeList.findRef(comp) == -1))
		{
			m_p->removeList.append(comp);
		}
	}
}

void KSimUndo::remove(const ComponentList * compList)
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		FOR_EACH_COMP(it, *compList)
		{
			remove(it.current());
		}
	}
}

void KSimUndo::remove(const CompViewList * cvList)
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		FOR_EACH_COMPVIEW(it, *cvList)
		{
			remove(it.current()->getComponent());
		}
	}
}

void KSimUndo::undo()
{
	if (m_p->undoFileList.count() == 0)
	{
		KSIMDEBUG("Nothing to do");
	}
	else
	{
		// Open undo file
		KSimpleConfig * file = new KSimpleConfig(m_p->undoFileList.last());
	
		file->setGroup(sUndoGrp);
		QString desc = file->readEntry(sDesc, sNoDesc);
		parentDoc->getApp()->slotStatusHelpMsg(i18n("Undo: %1").arg(desc));
		beginRedo(desc);

		/** Executes the common parts of undo and redo */
		doCommon(file);
		
		end();

		delete file;
		
		if (!QFile::remove(m_p->undoFileList.last()))
		{
			KSIMDEBUG("Remove old file failed");
		}
		m_p->undoFileList.removeLast();

		if (m_p->undoFileList.count() == 0)
		{
			emit undoAllowed(false);
		}
		emit redoAllowed(true);
	}
}

void KSimUndo::hiddenUndo()
{
	if (m_p->undoFileList.count() == 0)
	{
		KSIMDEBUG("Nothing to do");
	}
	else
	{
		// Open undo file
		KSimpleConfig * file = new KSimpleConfig(m_p->undoFileList.last());
	
		file->setGroup(sUndoGrp);
		
		/** Executes the common parts of undo and redo */
		doCommon(file);
		
		delete file;
		
		if (!QFile::remove(m_p->undoFileList.last()))
		{
			KSIMDEBUG("Remove old file failed");
		}
		m_p->undoFileList.removeLast();

		if (m_p->undoFileList.count() == 0)
		{
			emit undoAllowed(false);
		}
		emit redoAllowed(false);
	}
}


void KSimUndo::redo()	
{
	if (m_p->redoFileList.count() == 0)
	{
		KSIMDEBUG("Nothing to do");
	}
	else
	{
		// Open redo file
		KSimpleConfig * file = new KSimpleConfig(m_p->redoFileList.last());
	
		file->setGroup(sUndoGrp);
		
		QString desc(file->readEntry(sDesc, sNoDesc));
		parentDoc->getApp()->slotStatusHelpMsg(i18n("Redo: %1").arg(desc));

		begin_(desc);

		/** Executes the common parts of undo and redo */
		doCommon(file);
		
		end();
		
		delete file;
		
		if (!QFile::remove(m_p->redoFileList.last()))
		{
			KSIMDEBUG("Remove old file failed");
		}
		m_p->redoFileList.removeLast();
        	
		if (m_p->redoFileList.count() == 0)
		{
			emit redoAllowed(false);
		}
	}
}
	
/** Executes the common parts of undo and redo */
void KSimUndo::doCommon(KSimpleConfig * file)
{
	QStrList numList;
	QSize newSheetSize, newUserSize, size;
	parentDoc->getActiveEditor()->unselectAll();
	
	file->setGroup("/");
	parentDoc->loadProperty(*file);
	container->loadProperty(*file);
	
	/*  Resize sheet size
		The new size contains the old and the new size
		This is requiered by replacement of components
		Do nothing, if size unchenged */
	file->setGroup(sUndoGrp);
	size = QSize();
	newSheetSize = file->readSizeEntry(sSheetSize, &size);
	if (newSheetSize.isValid())
	{
		QSize oldsize = container->getSheetSize();
		size.setHeight(QMAX(newSheetSize.height(),oldsize.height()));
		size.setWidth (QMAX(newSheetSize.width() ,oldsize.width() ));
		container->setSheetSize(size);
	}
	/*  Do same with user size */
	size = QSize();
	newUserSize = file->readSizeEntry(sUserSize, &size);
	if (newUserSize.isValid())
	{
		QSize oldsize = container->getUserSize();
		size.setHeight(QMAX(newUserSize.height(),oldsize.height()));
		size.setWidth (QMAX(newUserSize.width() ,oldsize.width() ));
		container->setUserSize(size);
	}
	
	file->readListEntry(sRemove, numList);
	while (numList.count())
	{
		QString num(numList.getLast());
		bool ok = false;
		unsigned int id;
		id = num.toUInt(&ok);
		if (ok)
		{
			FOR_EACH_COMP(it, *container->getComponentList())
			{
				if (it.current()->getSerialNumber() == id)
				{
					container->delComponent(it.current());
					break;
				}
			}
		}
		else
		{
			KSIMDEBUG("not a uint");
		}
		numList.removeLast();
	}
	
	
	file->setGroup("/");
	container->loadComponents(*file);
		
	file->setGroup(sUndoGrp);
	
	parentDoc->setModified(file->readBoolEntry(sModi, true));
		
		
	/*  Resize sheet size
		Do nothing, if size unchenged */
	if (newSheetSize.isValid())
	{
		container->setSheetSize(newSheetSize);
	}
	/*  Do same with user size */
	if (newUserSize.isValid())
	{
		container->setUserSize(newUserSize);
	}
	
	parentDoc->getApp()->setCurrentView(file->readEntry(sLastView));
/*	parentDoc->getActiveEditor()->setEditorView(
			(EditorViewType)(file->readNumEntry(sView)));		*/
	
	file->readListEntry(sSelect, numList);
	while (numList.count())
	{
		QString num(numList.getLast());
		bool ok = false;
		unsigned int id;
		id = num.toUInt(&ok);
		if (ok)
		{
			FOR_EACH_COMP(it, *container->getComponentList())
			{
				if (it.current()->getSerialNumber() == id)
				{
					parentDoc->getActiveEditor()->select(it.current(),true);
					break;
				}
			}
		}
		else
		{
			KSIMDEBUG("not a uint");
		}
		numList.removeLast();
	}

	file->setGroup("/");
}

void KSimUndo::removeUndoHistory()
{
	// Remove all undo files
	while (m_p->undoFileList.count())
	{
		if (!QFile::remove(m_p->undoFileList.first()))
		{
			KSIMDEBUG_VAR("Remove files failed", m_p->undoFileList.first());
		}
		m_p->undoFileList.removeFirst();
	}
	m_p->undoFileCounter = 0;
	emit undoAllowed(false);
}

void KSimUndo::removeRedoHistory()
{
	// Remove all undo files
	while (m_p->redoFileList.count())
	{
		if (!QFile::remove(m_p->redoFileList.first()))
		{
			KSIMDEBUG_VAR("Remove files failed", m_p->undoFileList.first());
		}
		m_p->redoFileList.removeFirst();
	}
	m_p->redoFileCounter = 0;
	emit redoAllowed(false);
}

void KSimUndo::changeProperty(const Component * comp, const QString & description)
{
	begin(description);
	reload(comp);
	end();
}
void KSimUndo::changeProperty(const CompView * compView, const QString & description)
{
	changeProperty(compView->getComponent(), description);
}

void KSimUndo::changeDocumentProperty(const QString & description)
{
	begin(description);
	reloadDocumentProperty();
	end();
}

void KSimUndo::reloadDocumentProperty()
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		parentDoc->saveProperty(*m_p->file);
	}
}
void KSimUndo::changeContainerProperty(const QString & description)
{
	begin(description);
	reloadContainerProperty();
	end();
}
void KSimUndo::reloadContainerProperty()
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		container->saveProperty(*m_p->file);
	}
}

void KSimUndo::changeSheetSize(const QSize & size)
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		QString group = m_p->file->group();
		m_p->file->setGroup(sUndoGrp);

		// Save only once per undo session
		QSize test = QSize();
		test = m_p->file->readSizeEntry(sSheetSize, &test);
		if(!test.isValid())
		{
			m_p->file->writeEntry(sSheetSize, size);
		}
		m_p->file->setGroup(group);
	}
}

void KSimUndo::changeUserSize(const QSize & size)
{
	// Do only if function called between KSimUndo::begin() and KSimUndo::end()
	if (isActive())
	{
		QString group = m_p->file->group();
		m_p->file->setGroup(sUndoGrp);

		// Save only once per undo session
		QSize test = QSize();
		test = m_p->file->readSizeEntry(sUserSize, &test);
		if(!test.isValid())
		{
			m_p->file->writeEntry(sUserSize, size);
		}
		m_p->file->setGroup(group);
	}
}

void KSimUndo::setUndoDepth(int depth)
{
	m_p->undoDepth = depth;
}
int KSimUndo::getUndoDepth() const
{
	return m_p->undoDepth;
}
void KSimUndo::setRedoDepth(int depth)
{
	m_p->redoDepth = depth;
}

int KSimUndo::getRedoDepth() const
{
	return m_p->redoDepth;
}
