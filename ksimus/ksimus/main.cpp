/***************************************************************************
                          main.cpp  -  description
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "ksimus.h"

// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
static const char *description =
	I18N_NOOP("KSimus is an application for simulating networks with boolean data type.\n"
						"Analog, string and some more data types are planned. Currently there exists only a few boolean\n"
						"components, but because of the modular character of KSimus extensions are easy to develop.");
	
	
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

	KAboutData aboutData( "ksimus", I18N_NOOP("KSimus"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 2001, Rasmus Diekenbrock");
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
    KSimusApp *ksimus = new KSimusApp();
    ksimus->show();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		if (args->count())
		{
          ksimus->openDocumentFile(args->arg(0));
		}
/*		else
		{
		  ksimus->openDocumentFile();
		}*/
		args->clear();
  }

  return app.exec();
}  
