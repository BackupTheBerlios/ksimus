####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libksimus-boolean.la


## INCLUDES were found outside kdevelop specific part


LDFLAGS =    -shared

libksimus_boolean_la_METASOURCES = AUTO


LDADD = 

libksimus_boolean_la_SOURCES = implicitconverterbooltristate2bool.cpp extconnbooltristate.cpp convertbooltristate2bool.cpp convertbool2booltristate.cpp inverter.cpp delay.cpp demultiplexer.cpp multiplexer.cpp multidlatch.cpp boolean7segment.cpp booleanconstant.cpp booleancounter.cpp monoflop.cpp dflipflop.cpp jkflipflop.cpp flipflopbase.cpp rsflipflop.cpp boolean2in1out.cpp booleanor.cpp booleanxor.cpp boolean.cpp 
libksimus_boolean_la_LIBADD = 

libksimus_boolean_la_LDFLAGS = $(all_libraries) -avoid-version -module -no-undefined



EXTRA_DIST = boolean.cpp boolean.h booleanxor.cpp booleanxor.h booleanor.cpp booleanor.h boolean2in1out.cpp boolean2in1out.h rsflipflop.cpp rsflipflop.h flipflopbase.cpp flipflopbase.h jkflipflop.cpp jkflipflop.h dflipflop.cpp dflipflop.h monoflop.cpp monoflop.h booleancounter.cpp booleancounter.h booleanconstant.cpp booleanconstant.h boolean7segment.cpp boolean7segment.h multidlatch.cpp multidlatch.h multiplexer.cpp multiplexer.h demultiplexer.cpp demultiplexer.h delay.cpp delay.h inverter.cpp inverter.h convertbool2booltristate.cpp convertbool2booltristate.h convertbooltristate2bool.cpp convertbooltristate2bool.h extconnbooltristate.cpp extconnbooltristate.h implicitconverterbooltristate2bool.cpp implicitconverterbooltristate2bool.h 

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
INCLUDES= $(all_includes) -I$(includedir)



# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
		 $(XGETTEXT) -C -ki18n -x $(kde_includes)/kde.pot $$LIST -o ../po/ksimus-boolean.pot; \
	fi




