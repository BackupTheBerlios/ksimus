####### kdevelop will overwrite this part!!! (begin)##########

INCLUDES = $(all_includes)

libmodule_a_METASOURCES = AUTO

noinst_LIBRARIES = libmodule.a

libmodule_a_SOURCES = modulepropertywidget.cpp typeselection.cpp moduleeditor.cpp moduledialog.cpp moduledata.cpp module.cpp 


EXTRA_DIST = module.cpp module.h moduledata.cpp moduledata.h moduledialog.cpp moduledialog.h moduleeditor.cpp moduleeditor.h typeselection.cpp typeselection.h modulepropertywidget.cpp modulepropertywidget.h 

install-data-local:
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/module.h $(pkgincludedir)/module.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/moduledata.h $(pkgincludedir)/moduledata.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/moduledialog.h $(pkgincludedir)/moduledialog.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/moduleeditor.h $(pkgincludedir)/moduleeditor.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/typeselection.h $(pkgincludedir)/typeselection.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/modulepropertywidget.h $(pkgincludedir)/modulepropertywidget.h

uninstall-local:
	-rm -f $(pkgincludedir)/module.h
	-rm -f $(pkgincludedir)/moduledata.h
	-rm -f $(pkgincludedir)/moduledialog.h
	-rm -f $(pkgincludedir)/moduleeditor.h
	-rm -f $(pkgincludedir)/typeselection.h
	-rm -f $(pkgincludedir)/modulepropertywidget.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools


