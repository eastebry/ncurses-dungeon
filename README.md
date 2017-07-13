# A Raycaster in your terminal

# TODO
* Make intro where you set terminal size and pick a move style
* Levels!
* Fix the screen tearing and refreshing bug

## Screen tearing
* The refreshing bug goes away when using apple's terminal, instead of iterm
* Repeated calls to `clear()` causes some glitchy transition scenes
* Calling `erase()` instead works nicely locally, but leads to some serious screen tearing remotely
* `erase()` might work, but I am thinking the terminal size and remote terminal size need to match up
