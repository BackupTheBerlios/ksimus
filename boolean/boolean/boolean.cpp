/***************************************************************************
                          boolean.cpp  -  description
                             -------------------
    begin                : Son Mai  6 22:47:09 CEST 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
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
#include "boolean.h"
#include "booleanor.h"
#include "booleanxor.h"
#include "rsflipflop.h"

// Forward declaration

// Project-Includes

const ComponentInfoList distributeComponents = { &KSimLibBoolean::BooleanOrInfo,
                                                 &KSimLibBoolean::BooleanNorInfo,
                                                 &KSimLibBoolean::BooleanXorInfo,
                                                 &KSimLibBoolean::BooleanXnorInfo,
                                                 &KSimLibBoolean::RSFlipFlopInfo,
                                                 0 };


