####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libboolean.la


INCLUDES = $(all_includes)


LDFLAGS = -shared


LDADD = 


libboolean_la_LIBADD = 


libboolean_la_LDFLAGS = -avoid-version -module -no-undefined

libboolean_la_METASOURCES = AUTO

libboolean_la_SOURCES = booleanconstant.cpp booleancounter.cpp monoflop.cpp dflipflop.cpp jkflipflop.cpp flipflopbase.cpp rsflipflop.cpp boolean2in1out.cpp booleanor.cpp booleanxor.cpp boolean.cpp 


EXTRA_DIST = boolean.cpp boolean.h lo16-app-boolean.png booleanxor.cpp booleanxor.h booleanor.cpp booleanor.h boolean2in1out.cpp boolean2in1out.h rsflipflop.cpp rsflipflop.h flipflopbase.cpp flipflopbase.h jkflipflop.cpp jkflipflop.h dflipflop.cpp dflipflop.h monoflop.cpp monoflop.h booleancounter.cpp booleancounter.h booleanconstant.cpp booleanconstant.h 

install-data-local:
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-boolean.png $(kde_icondir)/locolor/16x16/apps/boolean.png

uninstall-local:
	-rm -f $(kde_icondir)/locolor/16x16/apps/boolean.png

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

libdir = @libdir@/ksimus



# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
		 $(XGETTEXT) -C -ki18n -x $(kde_includes)/kde.pot $$LIST -o ../po/boolean.pot; \
	fi




