libdir = @libdir@/ksimus

INCLUDES= $(all_includes) -I$(includedir)

METASOURCES = AUTO

lib_LTLIBRARIES =  libksimus-boolean.la

AM_LDFLAGS = -shared -avoid-version -module -no-undefined 
libksimus_boolean_la_LDFLAGS = $(all_libraries)

libksimus_boolean_la_SOURCES =  boolean2in1out.cpp boolean7segment.cpp booleanconstant.cpp booleancounter.cpp boolean.cpp booleanor.cpp booleanxor.cpp booltristate1out.cpp booltristateand.cpp booltristateor.cpp booltristatexor.cpp convertbool2booltristate.cpp convertbooltristate2bool.cpp delay.cpp demultiplexer.cpp dflipflop.cpp extconnbooltristate.cpp flipflopbase.cpp implicitconverterbooltristate2bool.cpp inverter.cpp iodevicevirtualbool.cpp jkflipflop.cpp monoflop.cpp multidlatch.cpp multiplexer.cpp rsflipflop.cpp

noinst_HEADERS =  boolean2in1out.h boolean7segment.h booleanconstant.h booleancounter.h boolean.h booleanor.h booleanxor.h booltristate1out.h booltristateand.h booltristateor.h booltristatexor.h convertbool2booltristate.h convertbooltristate2bool.h delay.h demultiplexer.h dflipflop.h extconnbooltristate.h flipflopbase.h implicitconverterbooltristate2bool.h inverter.h iodevicevirtualbool.h jkflipflop.h monoflop.h multidlatch.h multiplexer.h rsflipflop.h

