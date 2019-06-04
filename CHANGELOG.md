# Changelog
## Complete
- move tetrimino and tetrimino display
- rotate tetrimino and tetrimino display
- make tetrimino land and fix in position properly without bugging out
- make more tetriminos
- display all tetriminos properly
- clearing / tetris-ing i.e. removing objects
- olographic display / ghost piece of where the current tetrimino might land after instant drop
- implement instant drop functionality
- fix hard drop display bugging out i.e. force display position to match logical position
- holding pieces
- displaying a held piece
- add tetrimino bag that lines up the next piece that will be dropped
- make tetrimino bag have a random selection of pieces
- make tetrimino bag show the next 2/3/4 pieces to be dropped
- fix weird stuck at top bug? -> line clears pulled out-of-array data (garbage) into the array which was "detected" as a blocking piece for newly spawned tetriminos
- better delays in moving and dropping a tetrimino
- basic points system
- bonus points for clearing simultaneous rows and/or consecutively
- fix transparency not working because tetrimino_display and object are 2 different things
- refactored .h files into .h and .cpp files
- (hardcoded "ghost" mode using shaders to only reveal objects when shined on by the spotlight i.e. faced by the camera)
- implement wallkicks/twists with j/l/s/t/z using the same offsets, i blocks using different offsets, and o blocks using no offsets 
- points for t-spins and t-spin mini (incl. double and triple variants)
- proper combo incrementing
- proper back-to-back bonus points
- fix bug where placing a tetrimino that overlaps with the ceiling of the game board will cause a non-removal tetrimino forever in that column
- added light_mode uniform to the fragment shader to support switching between ghost/spotlight shader and standard shader without having to recompile
- smoothed out the transparency of objects drawn by the fragment shader and also modified the spotlights intensity and attenuation to fit better

## Incomplete
- speedup if held down button DAS (delayed auto shift)
- animations for clearing stuff
- slowly move tetriminoes in directions / animations?
- better UI everywhere else
- better / incremental levels and points system that gets harder the more you clear and/or the longer you stay alive

### extra:
- extra modes i.e. ghost / spotlight
- cheese/garbage mode
- 1v1 pvp mode (add 1 layer / garbage below antoher person for clearing)
- full 3d mode -> 2 or more layers, and give extra axis for rotations? will need to resolve unseeable gaps in the tetris

