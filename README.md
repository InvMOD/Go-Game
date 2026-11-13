# Go-Game

## Build with raylib

In VS Code, press `Ctrl+Shift+B` and select **Build Go-Game with raylib**. This task uses the raylib-compatible compiler at `C:\raylib\w64devkit\bin\g++.exe` and links the installed archive at `C:\raylib\raylib\src\libraylib.a` directly.

Do not build this project with `C:\Program Files\CodeBlocks\MinGW\bin\g++.exe` and the supplied raylib archives: they were built with an incompatible MinGW runtime and fail to link (`undefined reference to stat64i32`). Use the VS Code task so the compiler and raylib library match.

Compile:
Board test 1: g++ -std=c++17 board_test.cpp board.cpp ../lib/libraylib.a -I../lib -lopengl32 -lgdi32 -lwinmm -o board_test.exe
