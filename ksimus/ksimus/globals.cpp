/***************************************************************************
                          globals.cpp  -  description
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

#include <qstring.h>
#include <qstrlist.h>

#include "globals.h"
#include "ksimdebug.h"

KSimusDocList * g_docList = 0;
KSimusAppList * g_appList = 0;
KSimEditorList * g_editorList = 0;





int split (QStrList * strList, const char * text, char delimiter)
{
	int index, counter = 0;
	QString base(text),line;
	
	// Split (multi line) text to single line strings
	do
	{
		index = base.findRev(delimiter);
		line = base.right(base.length() - index - 1);
		strList->insert(0, line);
		base.truncate(index);
		counter++;
	}
	while (-1 != index);

	return counter;
}

int getGreaterPrim(int number)
{
	const int somePrims[] = {	17,  47, 97, 167, 277,  379, 479, 577, 677, 773, 877, 977, 1069, 1181,
								1277, 1373, 1481, 1579, 1669, 1777, 1877, 1979, 2081, 2179, 2281, 2377,
								2477, 2579, 2677, 2777, 2879, 2971, 3079, 3181, 3271, 3373, 3469, 3581,
								3677, 3779, 3877, 3967, 4079, 4177, 4273, 4373, 4481, 4583, 4673, 4783,
								4877, 4973, 5077, 5179, 5279, 5381, 5477, 5573, 5683, 5779, 5879, 5981,
								6079, 6173, 6277, 6379, 6473, 6577, 6679, 6779, 6871, 6977, 7079, 7177,
								7283, 7369, 7477, 7577, 7681, 7789, 7877, 7963, 8081, 8179, 8273, 8377,
								8467, 8573, 8677, 8779, 8887, 8971, 9067, 9173, 9277, 9377, 9479, 9587,
								9677, 9781, 9883, 9973, 0
							};
							
	int idx;
	
	for (idx = 0; somePrims[idx]; idx++)
	{
		if (somePrims[idx] > number)
			break;
	}
	
	if (somePrims[idx] == 0)
	{
		KSIMDEBUG_VAR("out of range",number);
		idx --;	// get greatest prim
	}
	
	return somePrims[idx];
}


//#########################################################################
//#########################################################################

//#include "componentlayout.h"

void ksimusDummyFunction()
{
//#########################################################################
//#########################################################################
//
// Include here classes which are unused by ksimus but used by the packages
// The function will never be called !!!
//
//#########################################################################
//#########################################################################
//	new ComponentLayout((Component *)0);
}	





