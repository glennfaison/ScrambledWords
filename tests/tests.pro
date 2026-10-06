# Test project for ScrambledWords.
#
# The tests are intentionally kept separate from the main Jumble.pro so they
# can be built and run independently of the GUI (see the CI workflow added in
# this branch). They exercise the pure-logic entry points that the game
# depends on: dictionary lookup, scoring, word validation and duplicate
# tracking.

QT       += testlib core widgets
TEMPLATE = app

TARGET = tst_scrambledwords

SOURCES += main.cpp \
           tst_wordchecker.cpp \
           tst_session.cpp

HEADERS += tst_wordchecker.h \
           tst_session.h

# The WordChecker opens "dictionary.txt" from the process working directory.
# Tests therefore run from the repository root where the real dictionary
# ships. Run with:
#     ./tst_scrambledwords -txt