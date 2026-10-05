QT      += core testlib

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
    testwordchecker.cpp

HEADERS += \
    ../src/core/letter.h \
    ../src/core/player.h \
    ../src/core/scorestore.h \
    ../src/core/timer.h \
    ../src/core/wordchecker.h \
    testletter.h \
    testplayer.h \
    testscorestore.h \
    testwordchecker.h
