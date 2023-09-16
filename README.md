# Biquadris: 2 player competitive Tetris (with graphics display)

### Instructions:

`down`: Move the current block down by 1 position

`drop`: Drop the current block, switch player

`left`: Move the current block left by 1 position

`right`: Move the current block right by 1 position

`clockwise`: Rotate the current block clockwise by 90 degrees

`counterclockwise`: Rotate the current block counterclockwise by 90 degrees

`levelup`: Increases the difficulty level of the game by one

`leveldown`: Decreases the difficulty level of the game by one.

Note:
- Only as much of a command as is necessary to distinguish it from other commands needs to be entered. For example, `lef` is enough to distinguish the `left` command from the `levelup` command, the system understands either `lef` or `left` as meaning `left`.
- Commands can take a multiplier prefix, indicating that command should be executed some number of times. For example `3ri` means move to the right by three cells. If, for example, it is only possible to move to the right by two cells, then the block will move to the right by two cells only.
- To use graphics display, make sure to turn on X forwarding and have an X Window System Server running (e.g. Xming for Windows, XQuartz for Mac).

### Command line arguments:

`-text`: Runs the program in text-only mode. No graphics are displayed. The default behaviour (no -text) is to show both text and graphics.

`-seed xxx`: Sets the random number generator’s seed to `xxx`, set the seed to get a unique random sequence.

`-scriptfile1 xxx`: Uses `xxx` instead of `sequence1.txt` as a source of blocks for level 0, for player 1.

`-scriptfile2 xxx`: Uses `xxx` instead of `sequence2.txt` as a source of blocks for level 0, for player 2.

`-startlevel n`: Starts the game in level `n`. The game starts in level 0 if this option is not supplied.
