####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = ksimus
ksimus_SOURCES = componentpropertystylewidget.cpp componentstyle.cpp compviewvbox.cpp compviewhbox.cpp ksimdata.cpp ksimaction.cpp componentaddon.cpp compcontainer.cpp library.cpp componentpropertyinfowidget.cpp componentpropertydialog.cpp componentpropertygeneralwidget.cpp componentpropertybasewidget.cpp compviewwidget.cpp ksimeditor.cpp ksimwidget.cpp wire.cpp ksimundo.cpp ksimfile.cpp globals.cpp displaywidget.cpp compview.cpp componentmap.cpp componentitem.cpp component.cpp basewindow.cpp ksimusview.cpp ksimusdoc.cpp ksimus.cpp main.cpp 
ksimus_LDADD   = ./module/libmodule.a ./boolean/libboolean.a ./tools/libtools.a ./lib/liblib.a ./items/libitems.a -lltdl $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS = tools module boolean lib items 

EXTRA_DIST = main.cpp ksimus.cpp ksimus.h ksimusdoc.cpp ksimusdoc.h ksimusview.cpp ksimusview.h ksimusui.rc ksimus.desktop ksimus.xpm mini-ksimus.xpm basewindow.cpp basewindow.h compcontainer.h component.cpp component.h componentitem.cpp componentitem.h componentmap.cpp componentmap.h compview.cpp compview.h displaywidget.cpp displaywidget.h globals.cpp globals.h ksimfile.cpp ksimfile.h ksimundo.cpp ksimundo.h types.h wire.cpp wire.h resource.h ksimwidget.cpp ksimwidget.h ksimeditor.cpp ksimeditor.h compviewwidget.cpp compviewwidget.h ksimdebug.h componentpropertybasewidget.cpp componentpropertybasewidget.h componentpropertygeneralwidget.cpp componentpropertygeneralwidget.h componentpropertydialog.cpp componentpropertydialog.h componentpropertyinfowidget.cpp componentpropertyinfowidget.h library.cpp library.h compcontainer.cpp componentaddon.cpp componentaddon.h ksimaction.cpp ksimaction.h ksimdata.cpp ksimdata.h compviewhbox.cpp compviewhbox.h compviewvbox.cpp compviewvbox.h componentstyle.cpp componentstyle.h componentpropertystylewidget.cpp componentpropertystylewidget.h 

install-data-local:
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimus.h $(pkgincludedir)/ksimus.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimusdoc.h $(pkgincludedir)/ksimusdoc.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimusview.h $(pkgincludedir)/ksimusview.h
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/ksimus.desktop $(kde_appsdir)/Applications/ksimus.desktop
	$(mkinstalldirs) $(kde_icondir)/medium/locolor/apps/
	$(INSTALL_DATA) $(srcdir)/ksimus.xpm $(kde_icondir)/medium/locolor/apps/ksimus.xpm
	$(mkinstalldirs) $(kde_icondir)/small/locolor/apps/
	$(INSTALL_DATA) $(srcdir)/mini-ksimus.xpm $(kde_icondir)/small/locolor/apps/ksimus.xpm
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/basewindow.h $(pkgincludedir)/basewindow.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/compcontainer.h $(pkgincludedir)/compcontainer.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/component.h $(pkgincludedir)/component.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentitem.h $(pkgincludedir)/componentitem.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentmap.h $(pkgincludedir)/componentmap.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/compview.h $(pkgincludedir)/compview.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/displaywidget.h $(pkgincludedir)/displaywidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/globals.h $(pkgincludedir)/globals.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimfile.h $(pkgincludedir)/ksimfile.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimundo.h $(pkgincludedir)/ksimundo.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/types.h $(pkgincludedir)/types.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/wire.h $(pkgincludedir)/wire.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/resource.h $(pkgincludedir)/resource.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimwidget.h $(pkgincludedir)/ksimwidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimeditor.h $(pkgincludedir)/ksimeditor.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/compviewwidget.h $(pkgincludedir)/compviewwidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimdebug.h $(pkgincludedir)/ksimdebug.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentpropertybasewidget.h $(pkgincludedir)/componentpropertybasewidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentpropertygeneralwidget.h $(pkgincludedir)/componentpropertygeneralwidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentpropertydialog.h $(pkgincludedir)/componentpropertydialog.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentpropertyinfowidget.h $(pkgincludedir)/componentpropertyinfowidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/library.h $(pkgincludedir)/library.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentaddon.h $(pkgincludedir)/componentaddon.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimaction.h $(pkgincludedir)/ksimaction.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimdata.h $(pkgincludedir)/ksimdata.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/compviewhbox.h $(pkgincludedir)/compviewhbox.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/compviewvbox.h $(pkgincludedir)/compviewvbox.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentstyle.h $(pkgincludedir)/componentstyle.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentpropertystylewidget.h $(pkgincludedir)/componentpropertystylewidget.h

uninstall-local:
	-rm -f $(pkgincludedir)/ksimus.h
	-rm -f $(pkgincludedir)/ksimusdoc.h
	-rm -f $(pkgincludedir)/ksimusview.h
	-rm -f $(kde_appsdir)/Applications/ksimus.desktop
	-rm -f $(kde_icondir)/medium/locolor/apps/ksimus.xpm
	-rm -f $(kde_icondir)/small/locolor/apps/ksimus.xpm
	-rm -f $(pkgincludedir)/basewindow.h
	-rm -f $(pkgincludedir)/compcontainer.h
	-rm -f $(pkgincludedir)/component.h
	-rm -f $(pkgincludedir)/componentitem.h
	-rm -f $(pkgincludedir)/componentmap.h
	-rm -f $(pkgincludedir)/compview.h
	-rm -f $(pkgincludedir)/displaywidget.h
	-rm -f $(pkgincludedir)/globals.h
	-rm -f $(pkgincludedir)/ksimfile.h
	-rm -f $(pkgincludedir)/ksimundo.h
	-rm -f $(pkgincludedir)/types.h
	-rm -f $(pkgincludedir)/wire.h
	-rm -f $(pkgincludedir)/resource.h
	-rm -f $(pkgincludedir)/ksimwidget.h
	-rm -f $(pkgincludedir)/ksimeditor.h
	-rm -f $(pkgincludedir)/compviewwidget.h
	-rm -f $(pkgincludedir)/ksimdebug.h
	-rm -f $(pkgincludedir)/componentpropertybasewidget.h
	-rm -f $(pkgincludedir)/componentpropertygeneralwidget.h
	-rm -f $(pkgincludedir)/componentpropertydialog.h
	-rm -f $(pkgincludedir)/componentpropertyinfowidget.h
	-rm -f $(pkgincludedir)/library.h
	-rm -f $(pkgincludedir)/componentaddon.h
	-rm -f $(pkgincludedir)/ksimaction.h
	-rm -f $(pkgincludedir)/ksimdata.h
	-rm -f $(pkgincludedir)/compviewhbox.h
	-rm -f $(pkgincludedir)/compviewvbox.h
	-rm -f $(pkgincludedir)/componentstyle.h
	-rm -f $(pkgincludedir)/componentpropertystylewidget.h

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

# set the include path for X, qt and KDE
INCLUDES= $(all_includes) -Iitems -Ilib -Imodule -Itools

METASOURCES = AUTO

# the library search path. 
ksimus_LDFLAGS = $(all_libraries) $(KDE_RPATH)

rcdir = $(kde_datadir)/ksimus
rc_DATA = ksimusui.rc

messages: rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/ksimus.pot; \
	fi



