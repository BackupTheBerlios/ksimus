####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libksimusfloatingpoint.la


INCLUDES = $(all_includes)


LDFLAGS = -shared


LDADD = 


libksimusfloatingpoint_la_LIBADD = 


libksimusfloatingpoint_la_LDFLAGS = -avoid-version -module -no-undefined

libksimusfloatingpoint_la_METASOURCES = AUTO

libksimusfloatingpoint_la_SOURCES = floatstylerange1out.cpp floatlineinput.cpp floatremainder.cpp floatstyle1out.cpp floatxin1out.cpp floatsubtractor.cpp floatmultiplier.cpp floatlineoutput.cpp floatinputslider.cpp floatdivider.cpp floatadd.cpp float2in1out.cpp float1out.cpp ksimusfloatingpoint.cpp 


EXTRA_DIST = ksimusfloatingpoint.cpp ksimusfloatingpoint.h float1out.cpp float1out.h float2in1out.cpp float2in1out.h floatadd.cpp floatadd.h floatdivider.cpp floatdivider.h floatinputslider.cpp floatinputslider.h floatlineoutput.cpp floatlineoutput.h floatmultiplier.cpp floatmultiplier.h floatsubtractor.cpp floatsubtractor.h floatxin1out.cpp floatxin1out.h floatstyle1out.cpp floatstyle1out.h floatremainder.cpp floatremainder.h floatlineinput.cpp floatlineinput.h floatstylerange1out.cpp floatstylerange1out.h 

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
		 $(XGETTEXT) -C -ki18n -x $(kde_includes)/kde.pot $$LIST -o ../po/ksimusfloatingpoint.pot; \
	fi

