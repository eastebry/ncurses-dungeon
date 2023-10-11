# ncurses-dungeon
### A raycast 3d dungeon crawler in your terminal
![l-FTLuWA](https://github.com/eastebry/ncurses-dungeon/assets/1396340/dd5a8ab9-bf1a-4506-9cf3-2004a21ad3c3)

## About
This project contains a raycasting engine and three levels of a terminal-based dungeon crawler. This was built for the BsidesSF 2018 CTF. It was intentionally  written in C in order to include memory corruption challenges.

I am done with this project for now, but I would love if other CTF challenge authors use the code I have here to create interesting challenges for their CTF's!

## Writeups
[Here](https://medium.com/@microaeris/bsidessf-2018-ctf-fd23a265eb42) is a writeup that describes the solution to the first two levels.

## Level 1 - Find the Hidden Map
Level 1 was worth a small number of points, and requires using `strings`, plus a light bit of reverse engineering. Users wander around the dungeon until they discover a "Feeble Pickaxe", which can be used to destroy certain walls, but will break if used on an unbreakable wall.

This challenge was solved by multiple teams, although in retrospect it was a bit too difficult for a beginner challenge.

<details>
    <summary>Solution</summary>
    
Running `strings` on the binary will reveal multiple text clues, along with one very long string.
    
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
Reversing the binary will reveal that the characters `X * - &` are walls, and the letter-characters are events. The seemingly unreachable `E` character is the end of the level, and reveal the flag. There appears to be no way to reach the `E character`.
    
Returning to the binary, you will find that there is another very long, non-ascii string that is the exact same length as the map string. Reversing the code paths that lead to the `You swing the pickaxe against the rock. The rock crumbles apart` message, you will see that each character of this longer string is `xored` with each character of the map string and `0x99` to determine if the wall can be broken. This means that you can `xor` each character of the non-ascii string with `0x99` to reveal a hidden map! Wherever there are difference between the first and the second maps, there is a breakable wall.

</details>

## Level 2 - Find the Right Switch Combination
Level 2 was a combination of a reversing puzzle and programming puzzle. Players wander around the maze until they discover a "trap" and a set of switches, which must be pressed in a certain combination in order to reveal the exit of the dungeon. Only one team solved this challenge, so in retrospect, it was probably much too difficult.

<details>
    <summary>Solution</summary>

Using strings on the binary again, players reveal a map of the dungeon with an interesting set of events (remember, letters = events) in the middle:
```
**********E*********
*      XX zXX      *
*       XXXX       *
* ----   rr    ----*
*       &   &      *
** s******m******  *
*    ---ssass-X    *
*    --XsbcdsXX    *
*    ---sefgs-X    *
*    --XshijsXX    *
*ssss---sssss-X    *
* *******X*X*X**** *
* ----   *   ----- *
*      --*--       *
* ----   *   ----- *
* X&X&   *   X&X&X *
*      X&*&X       *
*&X&X&       X&X&X&*
*&&&&&&&&&&&&&&&&&&&
```
Reversing futher (very useful to use use a decompiler here), reveals that these events represents switches on the floor that must be pushed in a combination that satisfies this function:

```
bool unlock() {
    unsigned char w = switches[0];
    if (w == 0)
    return false;
    for (int i=1; i < sizeof(switches); i+=1){
        w ^= (w << 5);
        w ^= (w >> 3);
        w ^= (w << 7);
        if (switches[i] != w % 9)
        return false;
    }
    return true;
}
```
Essentially, the first switch (`switches[0]`) is a seed, that requires each of the other switches to be pressed a certain number of times.

There are an infinite number of solutions, but there is one catch: not all combinations of button presses are possible. Because switches are immediately adjacent, moving off one causes the player to press another.

This essentially boils down to a breadth-first search, starting by pressing a the first button a certain number of times, than using a BFS algorithm to determine if it is possible to press the other switches the correct number of times. Finding the correct combination reveals a hidden exit, which give player the flag.

</details>

## Level 3 - The Sands of Time
Level three was what I envisioned when I dreamt up this challenge. It is a memory corruption challenge that requires you to leverage gameplay elements in order to build an exploit. Players wander around the map, avoiding hidden spikes, until they find an item called "the sands of time", an item that allows you to wind back time and undo previous actions in the maze. (Hint: this item is terribly broken).

Unfortunately no one managed to solve this challenge, so it was clearly much to difficult. I thought the concept of the challenge was very interesting, but clearly it needed to be simplified.

<details>
    <summary>Solution</summary>
    
A key difference between this challenge and the prevoius challenges is that both the map and the flag are stored on the stack. This is an important detail for later.

Wandering around the maze (and avoiding the instant-death spikes), the player will find a item called "The Sands of Time". Using this item allows them to undo the previous step they look in the maze.

After playing around with The Sands of Time (and possibly doing some reverse engineering), the player will discover that it has one serious bug. The game keeps a list of each action the player takes, and unwinds the last action every time The Sands of Time are used. However, it doesn't actually "undo" the last action, instead it just performs the last action in reverse. If the player walks forward, using the Sands of Time will cause them to walk backwards. The game does not do any bounds checks when The Sands of Time are used.

This means the player can use the Sands of Time to escape the map! If the player repeatedly walks forward against a wall, they will remain stationary (collision checks are performed). At this point, they can use the Sands of Time to "undo" each of these walk-forward actions, causing them to walk backwards until they eventuall walk backwards out of the map.

The visuals are rendered to the screen based on the characters in the map, which is stored in the stack. When the player walks outside the map, they are literally walking through the stack, seeing whatever else is stored in memory there. Remember: the flag is also stored on the stack, so if the player skips backwards enough, they will be able to read the flag letter by letter.

Here's a video that shows the whole process (edited for brevity), first using the sands of time to walk outside the maze, then walking through the flag. Note the letters `F L A G` on the left wall, as the player skips backwards. They are literally walking through the flag!

https://github.com/eastebry/ncurses-dungeon/assets/1396340/26ae0708-5a31-42fd-8dbc-fe1608073d46

</details>


### Build and Run the Challenges
```
make
./level1
./level2
./level3
```

### Implementation Notes

#### Screen tearing
* The refreshing bug goes away when using apple's terminal, instead of iterm
* Repeated calls to `clear()` causes some glitchy transition scenes
* Calling `erase()` instead works nicely locally, but leads to some serious screen tearing remotely
* `erase()` might work, but I am thinking the terminal size and remote terminal size need to match up
