####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libksimus-datarecorder.la


## INCLUDES were found outside kdevelop specific part


LDFLAGS =    -shared

libksimus_datarecorder_la_METASOURCES = AUTO


LDADD = 

libksimus_datarecorder_la_SOURCES = textrec.cpp datarecorderchannelfloat.cpp zoomwidget.cpp widgetcontrolbutton.cpp channelpositionwidget.cpp datarecorderchannelboolean.cpp datarecorderwidget.cpp datarecorderview.cpp datarecorderdataview.cpp datarecorderchannelbase.cpp datarecorder.cpp 
libksimus_datarecorder_la_LIBADD = 

libksimus_datarecorder_la_LDFLAGS = $(all_libraries) -avoid-version -module -no-undefined



EXTRA_DIST = datarecorder.cpp datarecorder.h datarecorderchannelbase.cpp datarecorderchannelbase.h datarecorderdataview.cpp datarecorderdataview.h datarecorderview.cpp datarecorderview.h datarecorderwidget.cpp datarecorderwidget.h datarecorderchannelboolean.cpp datarecorderchannelboolean.h channelpositionwidget.cpp channelpositionwidget.h widgetcontrolbutton.cpp widgetcontrolbutton.h zoomwidget.cpp zoomwidget.h datarecorderchannelfloat.cpp datarecorderchannelfloat.h textrec.cpp textrec.h 

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs

libdir = @libdir@/ksimus
INCLUDES= $(all_includes) -I$(includedir)


messages:
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
		 $(XGETTEXT) -C -ki18n -x $(kde_includes)/kde.pot $$LIST -o ../po/ksimus-datarecorder.pot; \
	fi


