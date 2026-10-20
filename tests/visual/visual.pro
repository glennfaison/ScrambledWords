# Visual regression tests for the Jumble UI.
#
# Builds the application's widgets (everything except main.cpp) together with a
# QtTest harness that renders each window off-screen and compares it with the
# PNG baselines in tests/visual/baselines/.

QT       += core gui widgets testlib
CONFIG   += testcase console c++11
CONFIG   -= app_bundle

TARGET   = tst_visual
TEMPLATE = app

APP_DIR = $$PWD/../..
INCLUDEPATH += $$APP_DIR
DEPENDPATH  += $$APP_DIR

DEFINES += VISUAL_TESTS_SOURCE_DIR=\\\"$$PWD\\\"

SOURCES += tst_visual.cpp \
    $$APP_DIR/session.cpp \
    $$APP_DIR/letter.cpp \
    $$APP_DIR/timer.cpp \
    $$APP_DIR/wordchecker.cpp \
    $$APP_DIR/player.cpp \
    $$APP_DIR/mainmenu.cpp \
    $$APP_DIR/logindialog.cpp \
    $$APP_DIR/signupdialog.cpp \
    $$APP_DIR/settings.cpp

HEADERS += \
    $$APP_DIR/session.h \
    $$APP_DIR/letter.h \
    $$APP_DIR/timer.h \
    $$APP_DIR/wordchecker.h \
    $$APP_DIR/player.h \
    $$APP_DIR/mainmenu.h \
    $$APP_DIR/logindialog.h \
    $$APP_DIR/signupdialog.h \
    $$APP_DIR/settings.h

FORMS += \
    $$APP_DIR/session.ui \
    $$APP_DIR/mainmenu.ui \
    $$APP_DIR/logindialog.ui \
    $$APP_DIR/signupdialog.ui

RESOURCES += $$APP_DIR/icons.qrc
