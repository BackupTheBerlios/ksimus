libdir = @libdir@/ksimus

INCLUDES= $(all_includes) -I$(includedir)

METASOURCES = AUTO

lib_LTLIBRARIES =  libksimus-floatingpoint.la

AM_LDFLAGS = -shared -avoid-version -module -no-undefined 
libksimus_floatingpoint_la_LDFLAGS = $(all_libraries)

libksimus_floatingpoint_la_SOURCES =  convertboolfloat.cpp convertfloatbool.cpp dataselector.cpp delay.cpp extconnfloatin.cpp extconnfloatout.cpp float1in1out.cpp float1out.cpp float2in1out.cpp floatabs.cpp floatadd.cpp floatconstinput.cpp floatcos.cpp floatdivider.cpp floatexp.cpp floatinputslider.cpp floatlatch.cpp floatlineinput.cpp floatlineoutput.cpp floatlog.cpp floatmax.cpp floatmin.cpp floatmultiplier.cpp floatremainder.cpp floatsin.cpp floatstyle1out.cpp floatstylerange1out.cpp floatsubtractor.cpp floattan.cpp floatxin1out.cpp ksimusfloatingpoint.cpp random.cpp simpleconditional.cpp waveformgenerator.cpp

noinst_HEADERS =  convertboolfloat.h convertfloatbool.h dataselector.h delay.h extconnfloatin.h extconnfloatout.h float1in1out.h float1out.h float2in1out.h floatabs.h floatadd.h floatconstinput.h floatcos.h floatdivider.h floatexp.h floatinputslider.h floatlatch.h floatlineinput.h floatlineoutput.h floatlog.h floatmax.h floatmin.h floatmultiplier.h floatremainder.h floatsin.h floatstyle1out.h floatstylerange1out.h floatsubtractor.h floattan.h floatxin1out.h ksimusfloatingpoint.h random.h simpleconditional.h waveformgenerator.h
