/***************************************************************************
                          globals.h  -  description
                             -------------------
    begin                : Fri Sep 22 2000
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

// KDE-Includes

// Project-Includes

// Forward declaration
class KSimusDocList;
class KSimusAppList;
class KSimEditorList;

// Global Variables
extern KSimusDocList * g_docList;
extern KSimusAppList * g_appList;
extern KSimEditorList * g_editorList;


// Global Functions

/** The function returns a prim number that's greater than number.  */
int getGreaterPrim(int number);


