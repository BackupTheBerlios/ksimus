####### kdevelop will overwrite this part!!! (begin)##########

INCLUDES = $(all_includes)

libboolean_a_METASOURCES = AUTO

noinst_LIBRARIES = libboolean.a

libboolean_a_SOURCES = clockgenerator.cpp booleanled.cpp booleanbutton.cpp booleanand.cpp 


EXTRA_DIST = booleanand.cpp booleanand.h booleanbutton.cpp booleanbutton.h booleanled.cpp booleanled.h clockgenerator.cpp clockgenerator.h 

install-data-local:
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/booleanand.h $(kde_includes)/ksimus/booleanand.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/booleanbutton.h $(kde_includes)/ksimus/booleanbutton.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/booleanled.h $(kde_includes)/ksimus/booleanled.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/clockgenerator.h $(kde_includes)/ksimus/clockgenerator.h

uninstall-local:
	-rm -f $(kde_includes)/ksimus/booleanand.h
	-rm -f $(kde_includes)/ksimus/booleanbutton.h
	-rm -f $(kde_includes)/ksimus/booleanled.h
	-rm -f $(kde_includes)/ksimus/clockgenerator.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools


