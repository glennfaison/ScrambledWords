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

The resulting application bundle is `src/ui/jumble.app` on macOS, or the `jumble`
executable on other platforms.

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
| `icons/`             | Application icons                                  |

## License

See the project history for original authorship (Glenn Faison).