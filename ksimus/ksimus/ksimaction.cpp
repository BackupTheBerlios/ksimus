/***************************************************************************
                          ksimaction.cpp  -  description
                             -------------------
    begin                : Sat Sep 1 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#include "ksimaction.h"

KSimAction::KSimAction()
	: m_action(NO)
{
}

KSimAction::KSimAction(eAction action)
	: m_action(action)
{
}

KSimAction::KSimAction(const KSimAction & action)
	: m_action(action.getAction())
{
}

KSimAction::~KSimAction()
{
}

/** Filters by component addons unused actions. */
KSimAction KSimAction::componentAddOnFilter(KSimAction action)
{
	#define USED_ACTION (SIMULATION | DRAWSHEETVIEW | DRAWUSERVIEW | STORAGE | CHECKCIRCUIT | CHECKPROPERTY | INITPOPUPMENU | INITPROPERTYDIALOG)
	return KSimAction((eAction)(action.getAction() & USED_ACTION));
	#undef USED_ACTION
}

/** Filters by component unused actions. */
KSimAction KSimAction::componentFilter(KSimAction action)
{
	#define USED_ACTION (SIMULATION | CHECKCIRCUIT | UPDATEVIEW | CHECKPROPERTY | INITPOPUPMENU | INITPROPERTYDIALOG)
	return KSimAction((eAction)(action.getAction() & USED_ACTION));
	#undef USED_ACTION
}

/** Filters by component unused actions. */
KSimAction KSimAction::connectorFilter(KSimAction action)
{
	#define USED_ACTION (SIMULATION | STORAGE | CHECKCIRCUIT | CHECKPROPERTY | INITPOPUPMENU | INITPROPERTYDIALOG)
	return KSimAction((eAction)(action.getAction() & USED_ACTION));
	#undef USED_ACTION
}


