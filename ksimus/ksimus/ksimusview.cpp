/***************************************************************************
                          ksimusview.cpp  -  description
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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>

// application specific includes
#include "ksimusview.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimeditor.h"

KSimusView::KSimusView(KSimusApp * app, QWidget *parent, const char *name)
	:	QScrollView(parent, name),
		m_app(app)
{
  	// Hintergrundfarbe
//	setBackgroundMode(PaletteBackground);
//  setBackgroundMode(PaletteBase);
    m_editor = new KSimEditor(viewport(), name);
}

KSimusView::~KSimusView()
{
	delete m_editor;
}

KSimusDoc *KSimusView::getDocument() const
{
  return m_app->getDocument();
}

/** returns a pointer to the application */
KSimusApp *KSimusView::getApp() const
{
  return m_app;
}

/** returns a pointer to the editor */
KSimEditor *KSimusView::getEditor() const
{
  return m_editor;
}


void KSimusView::print(QPrinter *pPrinter)
{
  QPainter printpainter;
  printpainter.begin(pPrinter);
	
  // TODO: add your printing code here

  printpainter.end();
}
