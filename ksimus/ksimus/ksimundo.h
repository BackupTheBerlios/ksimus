/***************************************************************************
                          ksimundo.h  -  description
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

#ifndef KSIMUNDO_H
#define KSIMUNDO_H


// C-Includes

// QT-Includes
#include <qobject.h>
#include <qstrlist.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class KSimUndoPrivate;
class KSimusDoc;
class QSize;
class Component;
class CompView;
class ComponentList;
class CompViewList;
class KSimpleConfig;





/**Undo and Redo functionality
  *@author Rasmus Diekenbrock
  */

class KSimUndo : public QObject
{
	Q_OBJECT
protected:

public:
	KSimUndo(KSimusDoc * doc);
	~KSimUndo();
	
	void begin(const QString & description);
	void end();
	/** Pauses the undo tracking
	 *  pause = true  : stop tracking
	 *  pause = false : restart tracking */
	void pause(bool pause);
	/** Returns true, if undo tracking is active */
	bool isActive();
	void reload(const Component * comp);
	void reload(const ComponentList * compList);
	void reload(const CompViewList * cvList);
	void remove(const Component * comp);
	void remove(const ComponentList * compList);
	void remove(const CompViewList * cvList);

	void changeProperty(const Component * comp, const QString & description);
	void changeProperty(const CompView * compView, const QString & description);
	void changeDocumentProperty(const QString & description);
	void reloadDocumentProperty();
	void changeContainerProperty(const QString & description);
	void reloadContainerProperty();
	
	void changeSheetSize(const QSize & size);
	void changeUserSize(const QSize & size);

	
		
	void undo();
	/** Like undo(), but dont record for redo
	 */
	void hiddenUndo();
	void redo();	
	void removeUndoHistory();
	void removeRedoHistory();
	
	void setUndoDepth(int depth);
	int getUndoDepth() const;
	void setRedoDepth(int depth);
	int getRedoDepth() const;

private:
	KSimUndoPrivate * m_p;
	void begin_(const QString & description);
	void beginRedo(const QString & description);
	/** Executes the common parts of undo and redo */
	void doCommon(KSimpleConfig * file);

signals:
	void undoAllowed(bool yes);
	void redoAllowed(bool yes);

};

#endif
