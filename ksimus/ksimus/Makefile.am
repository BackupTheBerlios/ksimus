####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = ksimus
ksimus_SOURCES = ksimdata.cpp ksimaction.cpp componentaddon.cpp compcontainer.cpp library.cpp componentpropertyinfowidget.cpp componentpropertydialog.cpp componentpropertygeneralwidget.cpp componentpropertybasewidget.cpp compviewwidget.cpp ksimeditor.cpp ksimwidget.cpp wire.cpp ksimundo.cpp ksimfile.cpp globals.cpp displaywidget.cpp compview.cpp componentmap.cpp componentitem.cpp component.cpp basewindow.cpp ksimusview.cpp ksimusdoc.cpp ksimus.cpp main.cpp 
ksimus_LDADD   = ./module/libmodule.a ./boolean/libboolean.a ./tools/libtools.a ./lib/liblib.a ./items/libitems.a -ldl $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS = tools module boolean lib items 

EXTRA_DIST = main.cpp ksimus.cpp ksimus.h ksimusdoc.cpp ksimusdoc.h ksimusview.cpp ksimusview.h ksimusui.rc ksimus.desktop ksimus.xpm mini-ksimus.xpm basewindow.cpp basewindow.h compcontainer.h component.cpp component.h componentitem.cpp componentitem.h componentmap.cpp componentmap.h compview.cpp compview.h displaywidget.cpp displaywidget.h globals.cpp globals.h ksimfile.cpp ksimfile.h ksimundo.cpp ksimundo.h types.h wire.cpp wire.h resource.h ksimwidget.cpp ksimwidget.h ksimeditor.cpp ksimeditor.h compviewwidget.cpp compviewwidget.h ksimdebug.h componentpropertybasewidget.cpp componentpropertybasewidget.h componentpropertygeneralwidget.cpp componentpropertygeneralwidget.h componentpropertydialog.cpp componentpropertydialog.h componentpropertyinfowidget.cpp componentpropertyinfowidget.h library.cpp library.h compcontainer.cpp componentaddon.cpp componentaddon.h ksimaction.cpp ksimaction.h ksimdata.cpp ksimdata.h 

install-data-local:
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimus.h $(kde_includes)/ksimus/ksimus.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimusdoc.h $(kde_includes)/ksimus/ksimusdoc.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimusview.h $(kde_includes)/ksimus/ksimusview.h
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/ksimus.desktop $(kde_appsdir)/Applications/ksimus.desktop
	$(mkinstalldirs) $(kde_icondir)/medium/locolor/apps/
	$(INSTALL_DATA) $(srcdir)/ksimus.xpm $(kde_icondir)/medium/locolor/apps/ksimus.xpm
	$(mkinstalldirs) $(kde_icondir)/small/locolor/apps/
	$(INSTALL_DATA) $(srcdir)/mini-ksimus.xpm $(kde_icondir)/small/locolor/apps/ksimus.xpm
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/basewindow.h $(kde_includes)/ksimus/basewindow.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/compcontainer.h $(kde_includes)/ksimus/compcontainer.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/component.h $(kde_includes)/ksimus/component.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/componentitem.h $(kde_includes)/ksimus/componentitem.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/componentmap.h $(kde_includes)/ksimus/componentmap.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/compview.h $(kde_includes)/ksimus/compview.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/displaywidget.h $(kde_includes)/ksimus/displaywidget.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/globals.h $(kde_includes)/ksimus/globals.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimfile.h $(kde_includes)/ksimus/ksimfile.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimundo.h $(kde_includes)/ksimus/ksimundo.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/types.h $(kde_includes)/ksimus/types.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/wire.h $(kde_includes)/ksimus/wire.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/resource.h $(kde_includes)/ksimus/resource.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimwidget.h $(kde_includes)/ksimus/ksimwidget.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimeditor.h $(kde_includes)/ksimus/ksimeditor.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/compviewwidget.h $(kde_includes)/ksimus/compviewwidget.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimdebug.h $(kde_includes)/ksimus/ksimdebug.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/componentpropertybasewidget.h $(kde_includes)/ksimus/componentpropertybasewidget.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/componentpropertygeneralwidget.h $(kde_includes)/ksimus/componentpropertygeneralwidget.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/componentpropertydialog.h $(kde_includes)/ksimus/componentpropertydialog.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/componentpropertyinfowidget.h $(kde_includes)/ksimus/componentpropertyinfowidget.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/library.h $(kde_includes)/ksimus/library.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/componentaddon.h $(kde_includes)/ksimus/componentaddon.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimaction.h $(kde_includes)/ksimus/ksimaction.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/ksimdata.h $(kde_includes)/ksimus/ksimdata.h

uninstall-local:
	-rm -f $(kde_includes)/ksimus/ksimus.h
	-rm -f $(kde_includes)/ksimus/ksimusdoc.h
	-rm -f $(kde_includes)/ksimus/ksimusview.h
	-rm -f $(kde_appsdir)/Applications/ksimus.desktop
	-rm -f $(kde_icondir)/medium/locolor/apps/ksimus.xpm
	-rm -f $(kde_icondir)/small/locolor/apps/ksimus.xpm
	-rm -f $(kde_includes)/ksimus/basewindow.h
	-rm -f $(kde_includes)/ksimus/compcontainer.h
	-rm -f $(kde_includes)/ksimus/component.h
	-rm -f $(kde_includes)/ksimus/componentitem.h
	-rm -f $(kde_includes)/ksimus/componentmap.h
	-rm -f $(kde_includes)/ksimus/compview.h
	-rm -f $(kde_includes)/ksimus/displaywidget.h
	-rm -f $(kde_includes)/ksimus/globals.h
	-rm -f $(kde_includes)/ksimus/ksimfile.h
	-rm -f $(kde_includes)/ksimus/ksimundo.h
	-rm -f $(kde_includes)/ksimus/types.h
	-rm -f $(kde_includes)/ksimus/wire.h
	-rm -f $(kde_includes)/ksimus/resource.h
	-rm -f $(kde_includes)/ksimus/ksimwidget.h
	-rm -f $(kde_includes)/ksimus/ksimeditor.h
	-rm -f $(kde_includes)/ksimus/compviewwidget.h
	-rm -f $(kde_includes)/ksimus/ksimdebug.h
	-rm -f $(kde_includes)/ksimus/componentpropertybasewidget.h
	-rm -f $(kde_includes)/ksimus/componentpropertygeneralwidget.h
	-rm -f $(kde_includes)/ksimus/componentpropertydialog.h
	-rm -f $(kde_includes)/ksimus/componentpropertyinfowidget.h
	-rm -f $(kde_includes)/ksimus/library.h
	-rm -f $(kde_includes)/ksimus/componentaddon.h
	-rm -f $(kde_includes)/ksimus/ksimaction.h
	-rm -f $(kde_includes)/ksimus/ksimdata.h

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



