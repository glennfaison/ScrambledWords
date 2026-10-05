# ScrambledWords (Jumble)

A single-player word game. The player is shown 10 random letters and has
100 seconds to form as many valid SCRABBLE words as possible. Words are
scored by their letters' Scrabble values. High scores are stored per player.

## Project layout

```
Jumble.pro          top-level qmake project (builds src/ui and tests)
src/
  core/             game logic, no Qt widgets
    letter.h/.cpp        letter selection + Scrabble value table
    wordchecker.h/.cpp   dictionary lookup + word scoring
    player.h/.cpp        player data (name + score history)
    scorestore.h/.cpp    persistent JSON storage under QStandardPaths
    timer.h/.cpp         per-second countdown QThread
  ui/               Qt Widgets
    main.cpp             entry point (command-line parsing)
    mainmenu.h/.cpp/.ui  main menu with account list + high scores
    session.h/.cpp/.ui   the game screen
    logindialog.*        log-in dialog
    signupdialog.*       sign-up dialog
    ui.pro               sub-project file (must be named ui.pro for qmake subdirs)
tests/
  tests.pro        QtTest project
  testletter.cpp   letter selection + value table
  testplayer.cpp   player score bookkeeping
  testscorestore.cpp  JSON storage + backup/restore
  testwordchecker.cpp  dictionary lookup + scoring
icons.qrc          embedded icon resources
icons/             the actual icon files
dictionary.txt     fixed 30-byte null-padded word list (sorted)
```

## Build

Requires Qt 5 (5.15+ recommended) and C++17.

```
qmake -r Jumble.pro
make -j8
```

This produces:
- `src/ui/jumble`       the game
- `tests/jumble_tests`  the unit test suite

`dictionary.txt` must sit next to the executable for word checking to work.

## Run tests

```
tests/jumble_tests
```

All 24 tests should pass.

## Notes

- High scores live under `QStandardPaths::AppDataLocation`
  (e.g. `~/.local/share/ScrambledWords/highscores.json` on Linux) with a
  `.bak` copy next to it.
- Letter values use the original game's table (K=2, Q=8, matching the
  original code). The alphabet is weighted like a Scrabble tile bag.
- The word list is stored as fixed 30-byte null-padded records in
  `dictionary.txt`. `WordChecker::isWordInDictionary` loads it into a
  `QSet` for O(1) lookup instead of binary-searching the raw file.
