####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libdatarecorder.la


INCLUDES = $(all_includes)


LDFLAGS = -shared


LDADD = 


libdatarecorder_la_LIBADD = 


libdatarecorder_la_LDFLAGS = -avoid-version -module -no-undefined

libdatarecorder_la_METASOURCES = AUTO

libdatarecorder_la_SOURCES = datarecorderchannelfloat.cpp zoomwidget.cpp widgetcontrolbutton.cpp channelpositionwidget.cpp datarecorderchannelboolean.cpp datarecorderwidget.cpp datarecorderview.cpp datarecorderdataview.cpp datarecorderchannelbase.cpp datarecorder.cpp 


EXTRA_DIST = lo16-app-datarecorder.png datarecorder.cpp datarecorder.h datarecorderchannelbase.cpp datarecorderchannelbase.h datarecorderdataview.cpp datarecorderdataview.h datarecorderview.cpp datarecorderview.h datarecorderwidget.cpp datarecorderwidget.h datarecorderchannelboolean.cpp datarecorderchannelboolean.h channelpositionwidget.cpp channelpositionwidget.h widgetcontrolbutton.cpp widgetcontrolbutton.h zoomwidget.cpp zoomwidget.h datarecorderchannelfloat.cpp datarecorderchannelfloat.h 

install-data-local:
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-datarecorder.png $(kde_icondir)/locolor/16x16/apps/datarecorder.png

uninstall-local:
	-rm -f $(kde_icondir)/locolor/16x16/apps/datarecorder.png

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
		 $(XGETTEXT) -C -ki18n -x $(kde_includes)/kde.pot $$LIST -o ../po/datarecorder.pot; \
	fi


