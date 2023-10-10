# ncurses-dungeon
### A raycast 3d dungeon crawler in your terminal
![l-FTLuWA](https://github.com/eastebry/ncurses-dungeon/assets/1396340/dd5a8ab9-bf1a-4506-9cf3-2004a21ad3c3)

## About
This project contains a raycasting engine and three levels of a terminal-based dungeon crawler. This was built for the BsidesSF 2018 CTF.

[Here](https://medium.com/@microaeris/bsidessf-2018-ctf-fd23a265eb42) is a writeup that describes the solution to the first two levels.

## Implementation Notes
#### Screen tearing
* The refreshing bug goes away when using apple's terminal, instead of iterm
* Repeated calls to `clear()` causes some glitchy transition scenes
* Calling `erase()` instead works nicely locally, but leads to some serious screen tearing remotely
* `erase()` might work, but I am thinking the terminal size and remote terminal size need to match up
