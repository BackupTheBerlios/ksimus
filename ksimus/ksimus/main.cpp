/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Sat Dec 23 15:44:46 CET 2000
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

#include <stdio.h>

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kurl.h>

#include "ksimus.h"
#include "ksimdebug.h"
#include "config.h"

// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
static const char *description =
	I18N_NOOP("KSimus is an application for simulating networks with boolean and floating point data type.\n"
	          "String and some more data types are planned.");
	
	
static KCmdLineOptions options[] =
{
	{ "+[File]", I18N_NOOP("file to open"), 0 },
	{ "execute", I18N_NOOP("start the execution of the given file"), 0 },
	{ "hidden", I18N_NOOP("execute KSimus in hidden mode (only in combination with option 'execute')"), 0 },
	{ "language ", I18N_NOOP("use the given language (for testing purpose)"), 0 },
	{ 0, 0, 0 }
	// INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

	KAboutData aboutData( "ksimus", I18N_NOOP("KSimus"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 2003, Rasmus Diekenbrock");
	aboutData.addAuthor("Rasmus Diekenbrock",0, "ksimus@gmx.de");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	KApplication app;
 
	if (app.isRestored())
	{
		RESTORE(KSimusApp);
	}
	else
	{
	
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		// Set language
		QCString language = args->getOption("language");
		if (!language.isEmpty())
		{
			KGlobal::locale()->setLanguage(language);
		}
		
		if (args->count())
		{
			if (args->isSet("execute"))
			{
				for (int i = 0; i < args->count(); i++)
				{
					KSimusApp *ksimus = new KSimusApp();
					ksimus->executeDocumentFile(args->url(i));
					if (!args->isSet("hidden"))
					{
						ksimus->show();
					}
					else
					{
						ksimus->showMinimized();
					}
				}
			}
			else
			{
				for (int i = 0; i < args->count(); i++)
				{
					KSimusApp *ksimus = new KSimusApp();
					ksimus->openDocumentFile(args->url(i));
					ksimus->show();
				}
			}
		}
		else
		{
			KSimusApp *ksimus = new KSimusApp();
			ksimus->show();
		}
		
		args->clear();
	}
	
	return app.exec();
}  
