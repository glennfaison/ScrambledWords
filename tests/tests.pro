QT      += core testlib widgets

TARGET   = jumble_tests
TEMPLATE = app
CONFIG  += c++17 console
CONFIG  -= app_bundle

INCLUDEPATH += ..

SOURCES += \
    ../src/core/letter.cpp \
    ../src/core/player.cpp \
    ../src/core/scorestore.cpp \
    ../src/core/timer.cpp \
    ../src/core/wordchecker.cpp \
    main.cpp \
    testletter.cpp \
    testplayer.cpp \
    testscorestore.cpp \
    testwordchecker.cpp \
    testmainmenu.cpp \
    testsession.cpp

HEADERS += \
    ../src/core/letter.h \
    ../src/core/player.h \
    ../src/core/scorestore.h \
    ../src/core/timer.h \
    ../src/core/wordchecker.h \
    testletter.h \
    testplayer.h \
    testscorestore.h \
    testwordchecker.h \
    testmainmenu.h \
    testsession.h

# The AGL framework was removed from the macOS SDK in Xcode 15+.
# Qt 5.15's macx-clang mkspec still adds `-framework AGL` to the
# linker command line, which fails on modern SDKs. Strip the flag
# from the generated Makefile after qmake runs.
macx {
    system("sed -i '' 's/-framework AGL//g' Makefile")
}
