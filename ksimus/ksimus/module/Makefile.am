####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libmodule.a

## INCLUDES were found outside kdevelop specific part

libmodule_a_METASOURCES = AUTO

libmodule_a_SOURCES = modulepropertywidget.cpp typeselection.cpp moduleeditor.cpp moduledialog.cpp moduledata.cpp module.cpp 


EXTRA_DIST = module.cpp module.h moduledata.cpp moduledata.h moduledialog.cpp moduledialog.h moduleeditor.cpp moduleeditor.h typeselection.cpp typeselection.h modulepropertywidget.cpp modulepropertywidget.h 

install-data-local:
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/module.h $(pkgincludedir)/module.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/moduledata.h $(pkgincludedir)/moduledata.h

uninstall-local:
	-rm -f $(pkgincludedir)/module.h
	-rm -f $(pkgincludedir)/moduledata.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools


