# Fractals

This code uses SFML to render Mandelbrot fractals using multiple threads. This project was a test for multi-threading programming
in C++11. The code is very ugly and probably wrong in many places. I plan to rewrite it in the future and to provide basic
functionnalities such as zooming and panning.

# How to build
You need a recent version of Visual Studio to compile this code. I plan to rewrite this using CMake, but I didn't have time yet.
You also need a precompiled version of SFML 2.4. You will also need to add system, window and graphics DLLs in your PATH
environment variable or directly in the project. Don't forget to include both the debug and the release versions of the DLL.