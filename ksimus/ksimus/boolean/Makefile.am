####### kdevelop will overwrite this part!!! (begin)##########

INCLUDES = $(all_includes)

libboolean_a_METASOURCES = AUTO

noinst_LIBRARIES = libboolean.a

libboolean_a_SOURCES = booleanxin1out.cpp boolean1out.cpp clockgenerator.cpp booleanled.cpp booleanbutton.cpp booleanand.cpp 


EXTRA_DIST = booleanand.cpp booleanand.h booleanbutton.cpp booleanbutton.h booleanled.cpp booleanled.h clockgenerator.cpp clockgenerator.h boolean1out.cpp boolean1out.h booleanxin1out.cpp booleanxin1out.h 

install-data-local:
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/booleanand.h $(kde_includes)/ksimus/booleanand.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/booleanbutton.h $(kde_includes)/ksimus/booleanbutton.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/booleanled.h $(kde_includes)/ksimus/booleanled.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/clockgenerator.h $(kde_includes)/ksimus/clockgenerator.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/boolean1out.h $(kde_includes)/ksimus/boolean1out.h
	$(mkinstalldirs) $(kde_includes)/ksimus/
	$(INSTALL_DATA) $(srcdir)/booleanxin1out.h $(kde_includes)/ksimus/booleanxin1out.h

uninstall-local:
	-rm -f $(kde_includes)/ksimus/booleanand.h
	-rm -f $(kde_includes)/ksimus/booleanbutton.h
	-rm -f $(kde_includes)/ksimus/booleanled.h
	-rm -f $(kde_includes)/ksimus/clockgenerator.h
	-rm -f $(kde_includes)/ksimus/boolean1out.h
	-rm -f $(kde_includes)/ksimus/booleanxin1out.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools


