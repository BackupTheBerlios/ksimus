####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libio.a

## INCLUDES were found outside kdevelop specific part

libio_a_METASOURCES = AUTO

libio_a_SOURCES = ksimiostorage.cpp ksimiopinselectiondialog.cpp ksimiopinselectionwidget.cpp ksimiocomponent.cpp ksimiojoinboolin.cpp ksimiodevicepropertygeneralwidget.cpp ksimiodevicepropertybasewidget.cpp ksimiodevicepropertydialog.cpp ksimiodevicelist.cpp ksimiodeviceoverviewwidget.cpp ksimiojoin.cpp ksimiopin.cpp ksimiodevice.cpp 


EXTRA_DIST = ksimiodevice.cpp ksimiodevice.h ksimiopin.cpp ksimiopin.h ksimiojoin.cpp ksimiojoin.h ksimiodeviceoverviewwidget.cpp ksimiodeviceoverviewwidget.h ksimiodevicelist.cpp ksimiodevicelist.h ksimiodevicepropertydialog.cpp ksimiodevicepropertydialog.h ksimiodevicepropertybasewidget.cpp ksimiodevicepropertybasewidget.h ksimiodevicepropertygeneralwidget.cpp ksimiodevicepropertygeneralwidget.h ksimiojoinboolin.cpp ksimiojoinboolin.h ksimiocomponent.cpp ksimiocomponent.h ksimiopinselectionwidget.cpp ksimiopinselectionwidget.h ksimiopinselectiondialog.cpp ksimiopinselectiondialog.h ksimiostorage.cpp ksimiostorage.h 

install-data-local:
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimiodevice.h $(pkgincludedir)/ksimiodevice.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimiopin.h $(pkgincludedir)/ksimiopin.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimiojoin.h $(pkgincludedir)/ksimiojoin.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimiodevicepropertydialog.h $(pkgincludedir)/ksimiodevicepropertydialog.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimiodevicepropertybasewidget.h $(pkgincludedir)/ksimiodevicepropertybasewidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimiodevicepropertygeneralwidget.h $(pkgincludedir)/ksimiodevicepropertygeneralwidget.h
	$(mkinstalldirs) $(pkgincludedir)/
	$(INSTALL_DATA) $(srcdir)/ksimiojoinboolin.h $(pkgincludedir)/ksimiojoinboolin.h

uninstall-local:
	-rm -f $(pkgincludedir)/ksimiodevice.h
	-rm -f $(pkgincludedir)/ksimiopin.h
	-rm -f $(pkgincludedir)/ksimiojoin.h
	-rm -f $(pkgincludedir)/ksimiodevicepropertydialog.h
	-rm -f $(pkgincludedir)/ksimiodevicepropertybasewidget.h
	-rm -f $(pkgincludedir)/ksimiodevicepropertygeneralwidget.h
	-rm -f $(pkgincludedir)/ksimiojoinboolin.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES= $(all_includes) -I.. -I../items -I../lib -I../module -I../tools
