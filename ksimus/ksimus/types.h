/***************************************************************************
                          types.h  -  description
                             -------------------
    begin                : Fri Jul 21 2000
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

#ifndef TYPES_H
#define TYPES_H

enum eViewType { SHEET_VIEW, USER_VIEW };

enum eAppViewType { APP_SHEET_VIEW, APP_USER_VIEW, APP_MODULE_VIEW };

enum eHitType { INVALID_HIT, NO_HIT, NORMAL_HIT, CONNECTOR_HIT, WIRE_HIT, SPECIAL_HIT,
                COMP_RESIZE_F_HIT, COMP_RESIZE_B_HIT };
	

enum ConnOrientationType { CO_UNDEF, CO_TOP, CO_RIGHT, CO_BOTTOM, CO_LEFT };
enum ConnDirType { CD_UNDEF, CD_INPUT, CD_OUTPUT, CD_TRISTATE };


								



#endif // TYPES_H
