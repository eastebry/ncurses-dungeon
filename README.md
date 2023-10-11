# ncurses-dungeon
### A raycast 3d dungeon crawler in your terminal
![l-FTLuWA](https://github.com/eastebry/ncurses-dungeon/assets/1396340/dd5a8ab9-bf1a-4506-9cf3-2004a21ad3c3)

## About
This project contains a raycasting engine and three levels of a terminal-based dungeon crawler. This was built for the BsidesSF 2018 CTF. It was intentionally  written in C in order to include challenges memory corruption challenges.

### Level 1
Level 1 was worth a small number of points, and requires using `strings`, plus a light bit of reverse engineering. Users wander around the dungeon until they discover a "Feeble Pickaxe", which can be used to destroy certain walls, but will break if used on an unbreakable wall.

<details>
    <summary>Solution</summary>
    Running `strings` on the binary will reveal multiple text clues, along with one veyr long string.

    ```
    > strings -n30 level1
    ********************X  ab c dd&     *  XX*** **  d&     i  XXee --*  ---ii--*  XXee             *  XX------- XXXX  X*  XX     **pX X    *  XX  &  ** X&&&&&&&  XX  &              -XX  -----&& *****   XX        X     &X  XX---&  * XX&&&j&   X-   &      &       X-------------------XXff    gggXX-**   -XX----------   *   -XX--    &  X-------*XX  &  &&    &    XXXX  &&--&&----------XXzX&&----   - -    X*E******************
Immediately after entering, you hear a thunderous boom
The wall ceiling behind you collapse into a pile of rubble
You barely avoid the falling rock, but you are now trapped
The air here is thick and damp.
You feel a light breeze. This dungeon is large and cavernous.
There is a skeleton lying on the ground. It has clearly been here for while.
The bones of its fingers are curled around a piece of parchment.
...
The note reads: I've been trapped in this maze for weeks.
This dungeon is a labyrenth. I've expored every passageway ten times,
but still can't find where I entered. I wish I had a map.
The note reads: I ran into a another group of explorers today.
They told me that some of the stone walls are more brittle than others
and could be broken. No help to me. I can't tell any of these apart.
You examine the walls for cracks, but find nothing. This rock seems solid
...
You swing the pickaxe. It strikes nothing.
You swing the pickaxe against the rock. The rock crumbles apart
You swing the pickaxe, and it breaks
    ```

The text clues indicate that you must use the pickaxe to break certain walls. The long string is a map encoded in a single line, which can be split into multple to look like this:
```
********************
X  ab c dd&     *  X
X*** **  d&     i  X
Xee --*  ---ii--*  X
Xee             *  X
X------- XXXX  X*  X
X     **pX X    *  X
X  &  ** X&&&&&&&  X
X  &              -X
X  -----&& *****   X
X        X     &X  X
X---&  * XX&&&j&   X
-   &      &       X
-------------------X
Xff    gggXX-**   -X
X----------   *   -X
X--    &  X-------*X
X  &  &&    &    XXX
X  &&--&&----------X
XzX&&----   - -    X
*E******************
```

Reversing the binary will reveal that the characters `X * - &` are walls, and the letter-characters are events. The seemingly unreachable `E` character is the end of the level. There appears to be no way to reach the `E character`.

Returning to the binary, you will find that there is another very long, non-ascii string that is the exact same length as the map string. Reversing the code paths that lead to the `You swing the pickaxe against the rock. The rock crumbles apart` message, you will see that each character of this string is `xored` with each character of the map string and `0x99` to determine if the wall can be broken. This means that you can `xor` each character of the non-ascii string with `0x99` to reveal a hidden map! Wherever there are difference between the first and the second maps, there is a breakable wall.

[Here](https://medium.com/@microaeris/bsidessf-2018-ctf-fd23a265eb42) is a writeup that describes the solution to the first two levels.

### Build the challenges
```make
./level1
```

### Implementation Notes

#### Screen tearing
* The refreshing bug goes away when using apple's terminal, instead of iterm
* Repeated calls to `clear()` causes some glitchy transition scenes
* Calling `erase()` instead works nicely locally, but leads to some serious screen tearing remotely
* `erase()` might work, but I am thinking the terminal size and remote terminal size need to match up
