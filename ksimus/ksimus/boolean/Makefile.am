####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libboolean.a

## INCLUDES were found outside kdevelop specific part

libboolean_a_METASOURCES = AUTO

libboolean_a_SOURCES = controlpause.cpp controlstop.cpp controlclose.cpp simtimeinput.cpp booleanxin1out.cpp boolean1out.cpp clockgenerator.cpp booleanled.cpp booleanbutton.cpp booleanand.cpp 


EXTRA_DIST = booleanand.cpp booleanand.h booleanbutton.cpp booleanbutton.h booleanled.cpp booleanled.h clockgenerator.cpp clockgenerator.h boolean1out.cpp boolean1out.h booleanxin1out.cpp booleanxin1out.h simtimeinput.cpp simtimeinput.h controlclose.cpp controlclose.h controlstop.cpp controlstop.h controlpause.cpp controlpause.h 

install-data-local:
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/booleanand.h $(pkgincludedir)/booleanand.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/booleanbutton.h $(pkgincludedir)/booleanbutton.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/booleanled.h $(pkgincludedir)/booleanled.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/clockgenerator.h $(pkgincludedir)/clockgenerator.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/boolean1out.h $(pkgincludedir)/boolean1out.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/booleanxin1out.h $(pkgincludedir)/booleanxin1out.h

uninstall-local:
	-rm -f $(pkgincludedir)/booleanand.h
	-rm -f $(pkgincludedir)/booleanbutton.h
	-rm -f $(pkgincludedir)/booleanled.h
	-rm -f $(pkgincludedir)/clockgenerator.h
	-rm -f $(pkgincludedir)/boolean1out.h
	-rm -f $(pkgincludedir)/booleanxin1out.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools


