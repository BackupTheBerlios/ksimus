# set the include path for X, qt and KDE
INCLUDES = -I$(top_srcdir)/ksimus/boolean -I$(top_srcdir)/ksimus/io -I$(top_srcdir)/ksimus/items -I$(top_srcdir)/ksimus/lib -I$(top_srcdir)/ksimus/module -I$(top_srcdir)/ksimus/tools $(all_includes)

# these are the headers for your project
noinst_HEADERS =  basewindow.h compcontainer.h componentaddon.h component.h componentitem.h componentmap.h componentpropertybasewidget.h componentpropertydialog.h componentpropertygeneralwidget.h componentpropertyinfowidget.h componentpropertystylewidget.h componentstyle.h compview.h compviewhbox.h compviewvbox.h compviewwidget.h displaywidget.h globals.h ksimaction.h ksimdata.h ksimdebug.h ksimeditor.h ksimfile.h ksimundo.h ksimusdoc.h ksimus.h ksimusview.h ksimwidget.h library.h resource.h types.h wire.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/ksimus.pot

KDE_ICON = ksimus

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ksimus

# the application source, library search path, and link libraries
ksimus_SOURCES = basewindow.cpp compcontainer.cpp componentaddon.cpp component.cpp componentitem.cpp componentmap.cpp componentpropertybasewidget.cpp componentpropertydialog.cpp componentpropertygeneralwidget.cpp componentpropertyinfowidget.cpp componentpropertystylewidget.cpp componentstyle.cpp compview.cpp compviewhbox.cpp compviewvbox.cpp compviewwidget.cpp displaywidget.cpp globals.cpp ksimaction.cpp ksimdata.cpp ksimeditor.cpp ksimfile.cpp ksimundo.cpp ksimus.cpp ksimusdoc.cpp ksimusview.cpp ksimwidget.cpp library.cpp main.cpp wire.cpp ksimdebug.cpp
ksimus_LDFLAGS = $(KDE_RPATH) $(all_libraries) -rdynamic
ksimus_LDADD = $(top_builddir)/ksimus/module/libmodule.a $(top_builddir)/ksimus/lib/liblib.a $(top_builddir)/ksimus/items/libitems.a $(top_builddir)/ksimus/io/libio.a $(top_builddir)/ksimus/boolean/libboolean.a $(top_builddir)/ksimus/tools/libtools.a -lltdl $(LIB_KFILE) $(LIB_KDEPRINT) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT)

# this is where the desktop file will go
shelldesktopdir = $(kde_appsdir)/Applications
shelldesktop_DATA = ksimus.desktop ksimus-execute.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir = $(kde_datadir)/ksimus
shellrc_DATA = ksimusui.rc

mime_DATA = x-ksimus.desktop
mimedir = $(kde_mimedir)/application

noinst_DATA =  wire.cur wire_mask.cur
SUBDIRS = tools module boolean lib items io
AM_CXXFLAGS =  -fPIC
pkginclude_HEADERS = compcontainer.h componentaddon.h component.h componentitem.h componentpropertybasewidget.h componentpropertydialog.h componentpropertygeneralwidget.h componentpropertyinfowidget.h componentpropertystylewidget.h componentstyle.h compview.h compviewhbox.h compviewvbox.h compviewwidget.h ksimaction.h ksimdata.h ksimdebug.h ksimundo.h resource.h types.h wire.h ksimusdoc.h library.h ksimwidget.h ksimus.h
