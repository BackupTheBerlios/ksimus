/***************************************************************************
                          ksimicon.h  -  description
                             -------------------
    begin                : Wed May 28 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef KSIMICON_H
#define KSIMICON_H

// C-Includes

// QT-Includes

// KDE-Includes
#include <kiconloader.h>
#include <kicontheme.h>

// Project-Includes


/**An icon loader utility class with default icons.
  *@author Rasmus Diekenbrock
  */

class KSimIcon
{
public: 

public:
	static QPixmap load(const char * name, KIcon::Group group, KIcon::States state = KIcon::DefaultState);

private:
	typedef struct tDefaultIcons
	{
		const char * name;
		KIcon::Group group;
		KIcon::States state;
		const char * * xpm;
	};

	static tDefaultIcons defaultIcons[];
};

#endif
