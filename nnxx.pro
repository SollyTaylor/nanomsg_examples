QT += core
QT -= gui

CONFIG += c++11

TARGET = nnxx
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    nnxx/error.cpp \
    nnxx/message.cpp \
    nnxx/message_control.cpp \
    nnxx/message_istream.cpp \
    nnxx/message_iterator.cpp \
    nnxx/message_ostream.cpp \
    nnxx/message_streambuf.cpp \
    nnxx/nn.cpp \
    nnxx/poll.cpp \
    nnxx/pubsub.cpp \
    nnxx/reqrep.cpp \
    nnxx/socket.cpp \
    nnxx/survey.cpp \
    nnxx/tcp.cpp \
    nnxx/timeout.cpp \
    nanomsg/ext/nnxx_ext.c

HEADERS += \
    nanomsg/ext/nnxx_ext.h \
    nnxx/bus.h \
    nnxx/chrono.h \
    nnxx/error.h \
    nnxx/inproc.h \
    nnxx/ipc.h \
    nnxx/message.h \
    nnxx/message_control.h \
    nnxx/message_istream.h \
    nnxx/message_istream.hpp \
    nnxx/message_iterator.h \
    nnxx/message_ostream.h \
    nnxx/message_ostream.hpp \
    nnxx/message_streambuf.h \
    nnxx/message_streambuf.hpp \
    nnxx/nn.h \
    nnxx/pair.h \
    nnxx/pipeline.h \
    nnxx/poll.h \
    nnxx/pubsub.h \
    nnxx/reqrep.h \
    nnxx/socket.h \
    nnxx/socket.hpp \
    nnxx/survey.h \
    nnxx/tcp.h \
    nnxx/timeout.h \
    nnxx/unittest.h \
    nnxx/message \
    nnxx/socket \
    nnxx/testing


INCLUDEPATH += \
    build/include

LIBS += \
    -L$$PWD/build/lib/ -lnanomsg

PRE_TARGETDEPS += \
    $$PWD/build/lib/libnanomsg.a
