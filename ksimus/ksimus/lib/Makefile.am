####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = liblib.a

## INCLUDES were found outside kdevelop specific part

liblib_a_METASOURCES = AUTO

liblib_a_SOURCES = implicitconverterlibraryitem.cpp implicitconverterlibrary.cpp implicitconverterinfo.cpp packageinfo.cpp ksimpackage.cpp moduleinfo.cpp wirepropertylibraryitem.cpp wirepropertylibrary.cpp wirepropertyinfo.cpp connectorlibraryitem.cpp connectorlibrary.cpp connectorinfo.cpp componentpopupmenu.cpp componentlistviewitem.cpp componentlistview.cpp componentlibraryitem.cpp componentlibrary.cpp componentinfo.cpp componentdirectory.cpp baselibraryitem.cpp baselibrary.cpp baseinfo.cpp 


EXTRA_DIST = baseinfo.cpp baseinfo.h baselibrary.cpp baselibrary.h baselibraryitem.cpp baselibraryitem.h componentdirectory.cpp componentdirectory.h componentinfo.cpp componentinfo.h componentlibrary.cpp componentlibrary.h componentlibraryitem.cpp componentlibraryitem.h componentlistview.cpp componentlistview.h componentlistviewitem.cpp componentlistviewitem.h componentpopupmenu.cpp componentpopupmenu.h connectorinfo.cpp connectorinfo.h connectorlibrary.cpp connectorlibrary.h connectorlibraryitem.cpp connectorlibraryitem.h wirepropertyinfo.cpp wirepropertyinfo.h wirepropertylibrary.cpp wirepropertylibrary.h wirepropertylibraryitem.cpp wirepropertylibraryitem.h moduleinfo.cpp moduleinfo.h ksimpackage.cpp ksimpackage.h packageinfo.cpp packageinfo.h implicitconverterinfo.cpp implicitconverterinfo.h implicitconverterlibrary.cpp implicitconverterlibrary.h implicitconverterlibraryitem.cpp implicitconverterlibraryitem.h 

install-data-local:
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/baseinfo.h $(pkgincludedir)/baseinfo.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/baselibrary.h $(pkgincludedir)/baselibrary.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentdirectory.h $(pkgincludedir)/componentdirectory.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/componentinfo.h $(pkgincludedir)/componentinfo.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/connectorinfo.h $(pkgincludedir)/connectorinfo.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/connectorlibrary.h $(pkgincludedir)/connectorlibrary.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/wirepropertyinfo.h $(pkgincludedir)/wirepropertyinfo.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/moduleinfo.h $(pkgincludedir)/moduleinfo.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimpackage.h $(pkgincludedir)/ksimpackage.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/packageinfo.h $(pkgincludedir)/packageinfo.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/implicitconverterinfo.h $(pkgincludedir)/implicitconverterinfo.h

uninstall-local:
	-rm -f $(pkgincludedir)/baseinfo.h
	-rm -f $(pkgincludedir)/baselibrary.h
	-rm -f $(pkgincludedir)/componentdirectory.h
	-rm -f $(pkgincludedir)/componentinfo.h
	-rm -f $(pkgincludedir)/connectorinfo.h
	-rm -f $(pkgincludedir)/connectorlibrary.h
	-rm -f $(pkgincludedir)/wirepropertyinfo.h
	-rm -f $(pkgincludedir)/moduleinfo.h
	-rm -f $(pkgincludedir)/ksimpackage.h
	-rm -f $(pkgincludedir)/packageinfo.h
	-rm -f $(pkgincludedir)/implicitconverterinfo.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools


