# My Raylib game

- 2D top down, player is a square
- map is cod nuketown
- Friendslop
- Shooting
    - guns have a conus that gets smaller the more you hold right click, if it gets too small you lose focus
    - the conus gets a little bigger depending on the recoil each time you shoot
    - recoil
    - invincibility dash
    - arms race

1. player movement
2. basic map loading, find a format

## if you wanna run it

follow https://thescienceofcode.com/raylib-vscode-c-cpp-debug/ 
then do `make; make run`
and make use of the included `.vscode` to debug the program, just make sure you have 
in CLI `make`, `clang-format` (make sure it points to .clang-format)
in VSCode extensions `clangd`, `Clang-Format`, `C/C++ Extension pack`
also copy https://github.com/raysan5/raygui/blob/master/src/raygui.h into C:/raylib/include, and add `C:\raylib\lib` to path if you didnt in the first tutorial.
i think thats it, though im not 100% sure