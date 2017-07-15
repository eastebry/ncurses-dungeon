# A Raycaster in your terminal

# TODO
* Make intro where you set terminal size and pick a move style
* Text interface
* detect width and height of the terminal
* Levels!

# Ideas
* Text interface that says things
* Door that only opens when you have your back to it
* Wall textures
* Gates that are partially see through
* Moving doors
* Sprites
* Limited field of vision
* Torches

## Screen tearing
* The refreshing bug goes away when using apple's terminal, instead of iterm
* Repeated calls to `clear()` causes some glitchy transition scenes
* Calling `erase()` instead works nicely locally, but leads to some serious screen tearing remotely
* `erase()` might work, but I am thinking the terminal size and remote terminal size need to match up
