/***************************************************************************
                          resource.h  -  description
                             -------------------
    begin                : Don Mär 16 19:35:41 CET 2000
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

#ifndef RESOURCE_H
#define RESOURCE_H

///////////////////////////////////////////////////////////////////
// resource.h  -- contains macros used for commands


///////////////////////////////////////////////////////////////////
// COMMAND VALUES FOR MENUBAR AND TOOLBAR ENTRIES


///////////////////////////////////////////////////////////////////
// File-menu entries
#define ID_FILE_NEW_WINDOW          10010
#define ID_FILE_NEW                 10020
#define ID_FILE_OPEN                10030
#define ID_FILE_OPEN_RECENT         10040
#define ID_FILE_CLOSE               10050

#define ID_FILE_SAVE                10060
#define ID_FILE_SAVE_AS             10070

#define ID_FILE_PRINT               10080

#define ID_FILE_QUIT                10090

///////////////////////////////////////////////////////////////////
// Edit-menu entries
#define ID_EDIT_UNDO                11010
#define ID_EDIT_REDO                11020
#define ID_EDIT_COPY                11030
#define ID_EDIT_CUT                 11040
#define ID_EDIT_PASTE               11050
#define ID_EDIT_INSERT_MODULE       11060

// Only Wire Popup !!
#define ID_EDIT_WIRE_DELETE         11910
#define ID_EDIT_WIRE_TRUNCATE       11920

///////////////////////////////////////////////////////////////////
// View-menu entries
#define ID_VIEW_TOOLBAR             12010
#define ID_VIEW_STATUSBAR           12020
#define ID_VIEW_SHEET	            12030
#define ID_VIEW_USER                12040
#define ID_VIEW_MODULE              12050

#define ID_VIEW_LOGLIST             12080
#define ID_VIEW_COSTMAP             12090
#define ID_VIEW_GRID	            12100

///////////////////////////////////////////////////////////////////
// Execute-menu entries
#define ID_EXEC_STOP	   			13010
#define ID_EXEC_START	   			13020
#define ID_EXEC_SINGLE	   			13030

///////////////////////////////////////////////////////////////////
// Popup entries
#define ID_PROPERTIES				14000

///////////////////////////////////////////////////////////////////
// Component specific entries
#define ID_COMP_SPECIFIC			15000

///////////////////////////////////////////////////////////////////
// Connector specific entries
#define ID_CONN_SPECIFIC			16000


///////////////////////////////////////////////////////////////////
// Help-menu entries
#define ID_HELP_CONTENTS            1002


///////////////////////////////////////////////////////////////////
// Component-menu entries			20000 - 29999

#define ID_COMP_MENU				20010


///////////////////////////////////////////////////////////////////
// General application values
#define ID_STATUS_MSG               1001
#define ID_COMP_INFO_MSG            1002

#define IDS_STATUS_DEFAULT          "Ready."
								   //1234567890123456789012345678901234567890
#define IDS_COMP_INFO_DEFAULT       "                                        "


///////////////////////////////////////////////////////////////////
// Some other common values
#define gridX 8
#define gridY gridX



#endif // RESOURCE_H
