####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libksimus-floatingpoint.la


## INCLUDES were found outside kdevelop specific part


LDFLAGS =   -shared

libksimus_floatingpoint_la_METASOURCES = AUTO


LDADD = 

libksimus_floatingpoint_la_SOURCES = random.cpp delay.cpp floatmin.cpp floatmax.cpp floattan.cpp floatcos.cpp floatsin.cpp floatlog.cpp floatexp.cpp dataselector.cpp floatlatch.cpp convertboolfloat.cpp convertfloatbool.cpp extconnfloatout.cpp extconnfloatin.cpp simpleconditional.cpp waveformgenerator.cpp floatabs.cpp float1in1out.cpp floatconstinput.cpp floatstylerange1out.cpp floatlineinput.cpp floatremainder.cpp floatstyle1out.cpp floatxin1out.cpp floatsubtractor.cpp floatmultiplier.cpp floatlineoutput.cpp floatinputslider.cpp floatdivider.cpp floatadd.cpp float2in1out.cpp float1out.cpp ksimusfloatingpoint.cpp 
libksimus_floatingpoint_la_LIBADD = 

libksimus_floatingpoint_la_LDFLAGS = $(all_libraries) -avoid-version -module -no-undefined



EXTRA_DIST = ksimusfloatingpoint.cpp ksimusfloatingpoint.h float1out.cpp float1out.h float2in1out.cpp float2in1out.h floatadd.cpp floatadd.h floatdivider.cpp floatdivider.h floatinputslider.cpp floatinputslider.h floatlineoutput.cpp floatlineoutput.h floatmultiplier.cpp floatmultiplier.h floatsubtractor.cpp floatsubtractor.h floatxin1out.cpp floatxin1out.h floatstyle1out.cpp floatstyle1out.h floatremainder.cpp floatremainder.h floatlineinput.cpp floatlineinput.h floatstylerange1out.cpp floatstylerange1out.h floatconstinput.cpp floatconstinput.h float1in1out.cpp float1in1out.h floatabs.cpp floatabs.h waveformgenerator.cpp waveformgenerator.h simpleconditional.cpp simpleconditional.h extconnfloatin.cpp extconnfloatin.h extconnfloatout.cpp extconnfloatout.h convertfloatbool.cpp convertfloatbool.h convertboolfloat.cpp convertboolfloat.h floatlatch.cpp floatlatch.h dataselector.cpp dataselector.h floatexp.cpp floatexp.h floatlog.cpp floatlog.h floatsin.cpp floatsin.h floatcos.cpp floatcos.h floattan.cpp floattan.h floatmax.cpp floatmax.h floatmin.cpp floatmin.h delay.cpp delay.h random.cpp random.h 

####### kdevelop will overwrite this part!!! (end)############

libdir = @libdir@/ksimus
INCLUDES= $(all_includes) -I$(includedir)

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

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
		 $(XGETTEXT) -C -ki18n -x $(kde_includes)/kde.pot $$LIST -o ../po/ksimus-floatingpoint.pot; \
	fi

