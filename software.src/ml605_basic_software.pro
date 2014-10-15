#-------------------------------------------------
#
# Project created by QtCreator 2012-10-30T15:09:45
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = ml605_basic_software
TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
} else {
    DESTDIR = build/release
}

OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui

# a fix for OSX Mountain Lion
unix:*-g++*: QMAKE_CXXFLAGS += -fpermissive

INCLUDEPATH += ../mrf/ \
	../mrf/mrfdata/ \
	../mrf/tools/ \
	../mrf/misc/


SOURCES += main.cpp\
    mainwindow.cpp \
	../mrf/mrftal.cpp \
	../mrf/mrftal_rbtpx3.cpp \
	../mrf/mrftal_rbtpx.cpp \
	../mrf/mrftal_rbudp.cpp \
	../mrf/mrfgal.cpp \
	../mrf/mrfgal_udp.cpp \
	../mrf/mrfcal.cpp \
	../mrf/mrfcal_topix3.cpp \
	../mrf/mrfcal_topix.cpp \
	../mrf/misc/mrftal_rb_sis.cpp \
	../mrf/misc/mrftal_rbbase.cpp \
	../mrf/misc/mrftal_rbbase_v6.cpp \
	../mrf/misc/mrftal_agilent33250a.cpp \
	../mrf/misc/mrfgal_sis1100.cpp \
	../mrf/misc/mrfgal_serialport.cpp \
	../mrf/misc/mrfgal_characterdevice.cpp \
	../mrf/misc/mrfcal_afei3.cpp \
	../mrf/mrfdata/mrfdataregaccess.cpp \
	../mrf/mrfdata/mrfdataadvbase.cpp \
	../mrf/mrfdata/mrfdataadv2d.cpp \
	../mrf/mrfdata/mrfdataadv1daddress.cpp \
	../mrf/mrfdata/mrfdataadv1d.cpp \
	../mrf/mrfdata/mrfdata.cpp \
	../mrf/mrfdata/mrfdata_tpxpixel.cpp \
	../mrf/mrfdata/mrfdata_tpxmasterreg.cpp \
	../mrf/mrfdata/mrfdata_tpxflags.cpp \
	../mrf/mrfdata/mrfdata_tpxdata.cpp \
	../mrf/mrfdata/mrfdata_tpx3pixel.cpp \
	../mrf/mrfdata/mrfdata_tpx3flags.cpp \
	../mrf/mrfdata/mrfdata_tpx3data.cpp \
	../mrf/mrfdata/mrfdata_tpx3config.cpp \
	../mrf/mrfdata/mrfdata_tpx3command.cpp \
	../mrf/mrfdata/mrfdata_mmcmconf.cpp \
	../mrf/mrfdata/mrfdata_ltc2620.cpp \
	../mrf/mrfdata/mrfdata_ltc2604.cpp \
	../mrf/mrfdata/mrfdata_ltc.cpp \
	../mrf/mrfdata/mrfdata_lcd.cpp \
	../mrf/mrfdata/mrfdata_dcmconf.cpp \
	../mrf/mrfdata/mrfdata_chainltc.cpp \
	../mrf/mrfdata/mrfdata_chain2ltc2604.cpp \
	../mrf/mrfdata/mrfdata_afei3hits.cpp \
	../mrf/mrfdata/mrfdata_afei3.cpp \
	../mrf/mrfdata/mrf_confitemaddress.cpp \
	../mrf/mrfdata/mrf_confitem.cpp \
	../mrf/tools/stringseparator.cpp \
	../mrf/tools/qmrftools.cpp \
	../mrf/tools/mrftools.cpp \
	../mrf/tools/mrfstrerror.cpp \
	../mrf/tools/bytehelper.cpp \
	../mrf/tools/mrfmath.cpp \
    register.cpp \  
    tabConnection.cpp \
    tabRegister.cpp

HEADERS  += mainwindow.h \
	../mrf/mrftal.h \
	../mrf/mrftal_rbtpx3.h \
	../mrf/mrftal_rbtpx.h \
	../mrf/mrftal_rbudp.h \
	../mrf/mrfgal.h \
	../mrf/mrfgal_udp.h \
	../mrf/mrfcal.h \
	../mrf/mrfcal_topix3.h \
	../mrf/mrfcal_topix.h \
	../mrf/mrfbase.h \
	../mrf/misc/mrftal_agilent33250a.h \
	../mrf/misc/mrftal_rb_sis.h \
	../mrf/misc/mrftal_rbbase.h \
	../mrf/misc/mrftal_rbbase_v6.h \
	../mrf/misc/mrfgal_sis1100.h \
	../mrf/misc/mrfgal_serialport.h \
	../mrf/misc/mrfgal_characterdevice.h \
	../mrf/misc/mrfcal_afei3.h \
	../mrf/mrfdata/mrfdataregaccess.h \
	../mrf/mrfdata/mrfdataadvbase.h \
	../mrf/mrfdata/mrfdataadv2d.h \
	../mrf/mrfdata/mrfdataadv1daddress.h \
	../mrf/mrfdata/mrfdataadv1d.h \
	../mrf/mrfdata/mrfdata.h \
	../mrf/mrfdata/mrfdata_tpxpixel.h \
	../mrf/mrfdata/mrfdata_tpxmasterreg.h \
	../mrf/mrfdata/mrfdata_tpxflags.h \
	../mrf/mrfdata/mrfdata_tpxdata.h \
	../mrf/mrfdata/mrfdata_tpx3pixel.h \
	../mrf/mrfdata/mrfdata_tpx3flags.h \
	../mrf/mrfdata/mrfdata_tpx3data.h \
	../mrf/mrfdata/mrfdata_tpx3config.h \
	../mrf/mrfdata/mrfdata_tpx3command.h \
	../mrf/mrfdata/mrfdata_tpx3ccr2.h \
	../mrf/mrfdata/mrfdata_tpx3ccr1.h \
	../mrf/mrfdata/mrfdata_tpx3ccr0.h \
	../mrf/mrfdata/mrfdata_mmcmconf.h \
	../mrf/mrfdata/mrfdata_ltc2620.h \
	../mrf/mrfdata/mrfdata_ltc2604.h \
	../mrf/mrfdata/mrfdata_ltc.h \
	../mrf/mrfdata/mrfdata_lcd.h \
	../mrf/mrfdata/mrfdata_dcmconf.h \
	../mrf/mrfdata/mrfdata_chainltc.h \
	../mrf/mrfdata/mrfdata_chain2ltc2604.h \
	../mrf/mrfdata/mrfdata_afei3hits.h \
	../mrf/mrfdata/mrfdata_afei3.h \
	../mrf/mrfdata/mrf_confitemaddress.h \
	../mrf/mrfdata/mrf_confitem.h \
	../mrf/tools/stringseparator.h \
	../mrf/tools/qmrftools.h \
	../mrf/tools/mrftools.h \
	../mrf/tools/mrfstrerror.h \
	../mrf/tools/bytehelper.h \
	../mrf/tools/mrfmath.h \
	../mrf/tools/sis1100_var.h \
    register.h   

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    Makefile
