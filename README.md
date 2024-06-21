# How to try this program on your local machine

## Requirements

You need:
- **Downloaded SFML-2.6.1**
- **g++ compiler**

## To run the code you should execute both of these compiling commands in the following order:
- **1) g++ -c main.cpp -I[path to SFML-2.6.1\include]**
- **2) g++ main.o -o main -L[path to SFML-2.6.1\lib] -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32**
- Then just start the main.exe file

## Example:
- **g++ -c main.cpp -IC:\Users\tsvet\SFML-2.6.1\include**
- **g++ main.o -o main -LC:\Users\tsvet\SFML-2.6.1\lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32**
