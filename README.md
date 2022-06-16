# noro
the new unix/linux text editor

## download
```
git clone --recurse-submodules https://github.com/MESYETI/noro
```

## compile
### cmake
```
cmake .
cmake --build .
```

### manually
```
g++ src/*.cc -lncurses -Wall -Wextra -Werror -pedantic -o noro
```

## known bugs
none B)

## screenshots
<img src="/pic/noro2.png">

## keys
| Key        | Action                     |
| ---------- | -------------------------- |
| Ctrl + Q   | Quit                       |
| Ctrl + E   | Maximise/unmaximise window |
| Ctrl + S   | Save file                  |
| Ctrl + D   | Save as                    |
| Ctrl + O   | Open                       |
| Ctrl + F   | Find                       |
| Ctrl + N   | New tab                    |
| Ctrl + W   | Close tab                  |
| Page down  | next tab                   |
| Page up    | previous tab               |

## features
### textbox history
use the up/down arrow keys to move up and down through your textbox history

textbox history is limited to 50 entries, you can change this in src/constants.hh
