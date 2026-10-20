# ScrambledWords

A word game built with Qt 5 (C++). Players unscramble letters to form valid words
from a built-in dictionary.

## Features

- Scrambled-letter word puzzles
- Binary-search dictionary lookup (`dictionary.txt`)
- User accounts with high-score persistence
- Session timer and scoring

## Building

The project uses a Qt `qmake` build (`Jumble.pro`). On macOS:

```bash
qmake -r Jumble.pro
make -j8
```

The resulting application bundle is `jumble.app` on macOS, or the `jumble`
executable on other platforms.

## Visual tests

`tests/visual/` contains screenshot tests (QtTest) for the main menu, log-in,
sign-up and game windows. They render off-screen with a pinned style and the
bundled DejaVu fonts, then compare against the PNGs in `tests/visual/baselines/`.

```bash
mkdir build-tests && cd build-tests
qmake ../tests/visual/visual.pro && make -j8
QT_QPA_PLATFORM=offscreen ./tst_visual
```

On a mismatch the actual and diff images are written to `visual-output/`
(CI uploads them as the `visual-diffs` artifact). After an intentional UI
change, refresh the baselines with `UPDATE_BASELINES=1 ./tst_visual`.
Baselines are generated on Linux; CI runs the tests on Ubuntu.

## Project layout

| File                 | Purpose                                            |
|----------------------|----------------------------------------------------|
| `Jumble.pro`         | qmake project file                                 |
| `main.cpp`           | Application entry point                            |
| `mainmenu.*`         | Main menu window                                   |
| `session.*`          | In-game session window                             |
| `logindialog.*`      | Login dialog                                       |
| `signupdialog.*`     | Sign-up dialog                                     |
| `letter.*`           | Letter-button model                                |
| `timer.*`            | Game timer                                         |
| `wordchecker.*`      | Dictionary word checker (binary search)           |
| `player.*`           | Player data and persistence                        |
| `settings.*`         | Settings placeholder                              |
| `dictionary.txt`     | Fixed-width dictionary data file                   |
| `icons/`, `icons.qrc` | Application icons, compiled into the binary        |
| `tests/visual/`      | Screenshot (visual regression) tests               |

## License

See the project history for original authorship (Glenn Faison).