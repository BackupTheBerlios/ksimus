libdir = @libdir@/ksimus

INCLUDES= $(all_includes) -I$(includedir)

METASOURCES = AUTO

lib_LTLIBRARIES =  libksimus-datarecorder.la

AM_LDFLAGS = -shared -avoid-version -module -no-undefined 
libksimus_datarecorder_la_LDFLAGS = $(all_libraries)

libksimus_datarecorder_la_SOURCES =  channelpositionwidget.cpp datarecorderchannelbase.cpp datarecorderchannelboolean.cpp datarecorderchannelfloat.cpp datarecorder.cpp datarecorderdataview.cpp datarecorderview.cpp datarecorderwidget.cpp textrec.cpp widgetcontrolbutton.cpp zoomwidget.cpp

noinst_HEADERS =  channelpositionwidget.h datarecorderchannelbase.h datarecorderchannelboolean.h datarecorderchannelfloat.h datarecorderdataview.h datarecorder.h datarecorderview.h datarecorderwidget.h textrec.h widgetcontrolbutton.h zoomwidget.h
