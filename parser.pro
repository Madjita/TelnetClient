TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        parser.cpp


HEADERS += \
    str_switch.h



LIBS += -L$$PWD/ -lWs2_32
LIBS += -L$$PWD/ -lwsock32

