####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libksimus-integer.la


INCLUDES = $(all_includes)


LDFLAGS =        -shared

libksimus_integer_la_METASOURCES = AUTO


LDADD = 

libksimus_integer_la_SOURCES = convertfloatinteger.cpp convertintegerfloat.cpp convertintegerbool.cpp convertboolinteger.cpp integer2multibool.cpp multibool2integer.cpp integerlatch.cpp integercounter.cpp integerinverterarithmetic.cpp integermin.cpp integermax.cpp integerconditional.cpp extconnintegerout.cpp extconnintegerin.cpp integerxor.cpp integeror.cpp integerand.cpp integerinverter.cpp integerconstinput.cpp integerimplicitconverter.cpp integerabs.cpp integerdelay.cpp integerdataselector.cpp integerremainder.cpp integersubtractor.cpp integerdivider.cpp integer2in1out.cpp integermultiplier.cpp integerlineinput.cpp integerspinbox.cpp integerstylerange1out.cpp integerstyle1out.cpp integerlineoutput.cpp integer1out.cpp integer1in1out.cpp integerxin1out.cpp integeradd.cpp packageinteger.cpp 
libksimus_integer_la_LIBADD = 

libksimus_integer_la_LDFLAGS = $(all_libraries) -avoid-version -module -no-undefined



EXTRA_DIST = packageinteger.h packageinteger.cpp integeradd.cpp integeradd.h integerxin1out.cpp integerxin1out.h integer1in1out.cpp integer1in1out.h integer1out.cpp integer1out.h integerlineinput.h integerlineoutput.cpp integerlineoutput.h integerstyle1out.cpp integerstyle1out.h integerstylerange1out.cpp integerstylerange1out.h integerspinbox.cpp integerspinbox.h integerlineinput.cpp integermultiplier.cpp integermultiplier.h integer2in1out.cpp integer2in1out.h integerdivider.cpp integerdivider.h integersubtractor.cpp integersubtractor.h integerremainder.cpp integerremainder.h integerdataselector.cpp integerdataselector.h integerdelay.cpp integerdelay.h integerabs.cpp integerabs.h integerimplicitconverter.cpp integerimplicitconverter.h integerconstinput.cpp integerconstinput.h integerinverter.cpp integerinverter.h integerand.cpp integerand.h integeror.cpp integeror.h integerxor.cpp integerxor.h extconnintegerin.cpp extconnintegerin.h extconnintegerout.cpp extconnintegerout.h integerconditional.cpp integerconditional.h integermax.cpp integermax.h integermin.cpp integermin.h integerinverterarithmetic.cpp integerinverterarithmetic.h integercounter.cpp integercounter.h integerlatch.cpp integerlatch.h multibool2integer.cpp multibool2integer.h integer2multibool.cpp integer2multibool.h convertboolinteger.cpp convertboolinteger.h convertintegerbool.cpp convertintegerbool.h convertintegerfloat.cpp convertintegerfloat.h convertfloatinteger.cpp convertfloatinteger.h 

####### kdevelop will overwrite this part!!! (end)############
# These paths are KDE specific. Use them:
# kde_appsdir         Where your application's menu entry (.desktop) should go to.
# kde_icondir         Where your icon should go to - better use KDE_ICON.
# kde_sounddir        Where your sounds should go to.
# kde_htmldir         Where your docs should go to. (contains lang subdirs)
# kde_datadir         Where you install application data. (Use a subdir)
# kde_locale          Where translation files should go to. (contains lang subdirs)
# kde_cgidir          Where cgi-bin executables should go to.
# kde_confdir         Where config files should go to (system-wide ones with default values).
# kde_mimedir         Where mimetypes .desktop files should go to.
# kde_servicesdir     Where services .desktop files should go to.
# kde_servicetypesdir Where servicetypes .desktop files should go to.
# kde_toolbardir      Where general toolbar icons should go to (deprecated, use KDE_ICON).
# kde_wallpaperdir    Where general wallpapers should go to.
# kde_templatesdir    Where templates for the "New" menu (Konqueror/KDesktop) should go to.
# kde_bindir          Where executables should go to. Use bin_PROGRAMS or bin_SCRIPTS.
# kde_libdir          Where shared libraries should go to. Use lib_LTLIBRARIES.
# kde_moduledir       Where modules (e.g. parts) should go to. Use kde_module_LTLIBRARIES.
# kde_styledir        Where Qt/KDE widget styles should go to (new in KDE 3).
# kde_designerdir     Where Qt Designer plugins should go to (new in KDE 3).

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
		 $(XGETTEXT) $$LIST -o ../po/ksimusdmm.pot; \
	fi

