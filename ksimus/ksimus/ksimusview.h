/***************************************************************************
                          ksimusview.h  -  description
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

#ifndef KSIMUSVIEW_H
#define KSIMUSVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

// include files for Qt
#include <qscrollview.h>

class KSimusDoc;
class KSimusApp;
class KSimEditor;

/** The KSimusView class provides the view widget for the KSimusApp instance.	
 * The View instance inherits QWidget as a base class and represents the view object of a KTMainWindow. As KSimusView is part of the
 * docuement-view model, it needs a reference to the document object connected with it by the KSimusApp class to manipulate and display
 * the document structure provided by the KSimusDoc class.
 * 	
 * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
 * @version KDevelop version 0.4 code generation
 */
class KSimusView : public QScrollView
{
  Q_OBJECT
  public:
    /** Constructor for the main view */
    KSimusView(KSimusApp * app, QWidget *parent = 0, const char *name=0);
    /** Destructor for the main view */
    ~KSimusView();

    /** returns a pointer to the document connected to the view instance. Mind that this method requires a KSimusApp instance as a parent
     * widget to get to the window document pointer by calling the KSimusApp::getDocument() method.
     *
     * @see KSimusApp#getDocument
     */
    KSimusDoc *getDocument() const;
	/** returns a pointer to the application */
    KSimusApp *getApp() const;
	/** returns a pointer to the editor */
    KSimEditor *getEditor() const;

    /** contains the implementation for printing functionality */
    void print(QPrinter *pPrinter);
	
  private:
  	KSimusApp * m_app;
  	KSimEditor * m_editor;
	
};

#endif // KSIMUSVIEW_H
