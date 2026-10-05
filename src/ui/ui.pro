QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = jumble
TEMPLATE = app
CONFIG  += c++17
CONFIG  -= debug_and_release

QMAKE_CXXFLAGS *= -Wall

# Data files must sit next to the executable so the app finds them.
win32: QMAKE_POST_LINK +=  copy /Y $$PWD\\..\\..\\dictionary.txt $$OUT_PWD\\

INCLUDEPATH += ..

SOURCES += \
    ../core/letter.cpp \
    ../core/player.cpp \
    ../core/scorestore.cpp \
    ../core/timer.cpp \
    ../core/wordchecker.cpp \
    logindialog.cpp \
    main.cpp \
    mainmenu.cpp \
    session.cpp \
    signupdialog.cpp

HEADERS += \
    ../core/letter.h \
    ../core/player.h \
    ../core/scorestore.h \
    ../core/timer.h \
    ../core/wordchecker.h \
    logindialog.h \
    mainmenu.h \
    session.h \
    signupdialog.h

FORMS   += \
    logindialog.ui \
    mainmenu.ui \
    session.ui \
    signupdialog.ui

RESOURCES += \
    ../../icons.qrc

# The AGL framework was removed from the macOS SDK in Xcode 15+.
# Qt 5.15's macx-clang mkspec still adds `-framework AGL` to the
# linker command line, which fails on modern SDKs. Strip the flag
# from the generated Makefile after qmake runs.
macx {
    system("sed -i '' 's/-framework AGL//g' Makefile")
}
