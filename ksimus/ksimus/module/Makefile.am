####### kdevelop will overwrite this part!!! (begin)##########

INCLUDES = $(all_includes)

libmodule_a_METASOURCES = AUTO

noinst_LIBRARIES = libmodule.a

libmodule_a_SOURCES = modulepropertywidget.cpp typeselection.cpp moduleeditor.cpp moduledialog.cpp moduledata.cpp module.cpp 


EXTRA_DIST = module.cpp module.h moduledata.cpp moduledata.h moduledialog.cpp moduledialog.h moduleeditor.cpp moduleeditor.h typeselection.cpp typeselection.h modulepropertywidget.cpp modulepropertywidget.h 

install-data-local:
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/module.h $(kde_includes)/ksimus/module.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/moduledata.h $(kde_includes)/ksimus/moduledata.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/moduledialog.h $(kde_includes)/ksimus/moduledialog.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/moduleeditor.h $(kde_includes)/ksimus/moduleeditor.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/typeselection.h $(kde_includes)/ksimus/typeselection.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/modulepropertywidget.h $(kde_includes)/ksimus/modulepropertywidget.h

uninstall-local:
	-rm -f $(kde_includes)/ksimus/module.h
	-rm -f $(kde_includes)/ksimus/moduledata.h
	-rm -f $(kde_includes)/ksimus/moduledialog.h
	-rm -f $(kde_includes)/ksimus/moduleeditor.h
	-rm -f $(kde_includes)/ksimus/typeselection.h
	-rm -f $(kde_includes)/ksimus/modulepropertywidget.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools


