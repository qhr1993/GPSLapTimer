QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

include(3rdparty/src/qextserialport.pri)
target.path = /home/pi/fyp_test/



TARGET = fyproj_test
TEMPLATE = app


FORMS += \
    mainwindow.ui \
    plotwidget.ui \
    plotaccel.ui \
    plotkmltrack.ui \
    plotkmldata.ui

HEADERS += \
    mainwindow.h \
    plotwidget.h \
    qcustomplot.h \
    kmldata.h \
    kmlhandler.h \
    kmlmaploader.h \
    plotaccel.h \
    gpsloggingthread.h \
    wiringPi/drcSerial.h \
    wiringPi/max5322.h \
    wiringPi/max31855.h \
    wiringPi/mcp23s08.h \
    wiringPi/mcp23s17.h \
    wiringPi/mcp23x08.h \
    wiringPi/mcp23x0817.h \
    wiringPi/mcp3002.h \
    wiringPi/mcp3004.h \
    wiringPi/mcp3422.h \
    wiringPi/mcp4802.h \
    wiringPi/mcp23008.h \
    wiringPi/mcp23016.h \
    wiringPi/mcp23016reg.h \
    wiringPi/mcp23017.h \
    wiringPi/pcf8574.h \
    wiringPi/pcf8591.h \
    wiringPi/sn3218.h \
    wiringPi/softPwm.h \
    wiringPi/softServo.h \
    wiringPi/softTone.h \
    wiringPi/sr595.h \
    wiringPi/wiringPi.h \
    wiringPi/wiringPiI2C.h \
    wiringPi/wiringPiSPI.h \
    wiringPi/wiringSerial.h \
    wiringPi/wiringShift.h \
    wiringPi/wpiExtensions.h \
    laptimerthread.h \
    plotkmltrack.h \
    plotkmldata.h \
    cpushbutton.h \
    ccombobox.h \
    cwidget.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    plotwidget.cpp \
    qcustomplot.cpp \
    kmldata.cpp \
    kmlhandler.cpp \
    kmlmaploader.cpp \
    plotaccel.cpp \
    gpsloggingthread.cpp \
    wiringPi/drcSerial.c \
    wiringPi/max5322.c \
    wiringPi/max31855.c \
    wiringPi/mcp23s08.c \
    wiringPi/mcp23s17.c \
    wiringPi/mcp3002.c \
    wiringPi/mcp3004.c \
    wiringPi/mcp3422.c \
    wiringPi/mcp4802.c \
    wiringPi/mcp23008.c \
    wiringPi/mcp23016.c \
    wiringPi/mcp23017.c \
    wiringPi/pcf8574.c \
    wiringPi/pcf8591.c \
    wiringPi/piHiPri.c \
    wiringPi/piThread.c \
    wiringPi/sn3218.c \
    wiringPi/softPwm.c \
    wiringPi/softServo.c \
    wiringPi/softTone.c \
    wiringPi/sr595.c \
    wiringPi/wiringPi.c \
    wiringPi/wiringPiI2C.c \
    wiringPi/wiringPiSPI.c \
    wiringPi/wiringSerial.c \
    wiringPi/wiringShift.c \
    wiringPi/wpiExtensions.c \
    laptimerthread.cpp \
    plotkmltrack.cpp \
    plotkmldata.cpp \
    cpushbutton.cpp \
    ccombobox.cpp \
    cwidget.cpp

DISTFILES += \
    wiringPi/drcSerial.o \
    wiringPi/max5322.o \
    wiringPi/max31855.o \
    wiringPi/mcp23s08.o \
    wiringPi/mcp23s17.o \
    wiringPi/mcp3002.o \
    wiringPi/mcp3004.o \
    wiringPi/mcp3422.o \
    wiringPi/mcp4802.o \
    wiringPi/mcp23008.o \
    wiringPi/mcp23016.o \
    wiringPi/mcp23017.o \
    wiringPi/pcf8574.o \
    wiringPi/pcf8591.o \
    wiringPi/piHiPri.o \
    wiringPi/piThread.o \
    wiringPi/sn3218.o \
    wiringPi/softPwm.o \
    wiringPi/softTone.o \
    wiringPi/sr595.o \
    wiringPi/wiringPi.o \
    wiringPi/wiringPiI2C.o \
    wiringPi/wiringPiSPI.o \
    wiringPi/wiringSerial.o \
    wiringPi/wiringShift.o \
    wiringPi/wpiExtensions.o

