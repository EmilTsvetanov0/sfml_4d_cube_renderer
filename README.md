Sorry for writing everything in a single .cpp file but I don't really know how to fix this, so enjoy what you got
Compiling commands:
g++ -c main.cpp -I[path to SFML-2.6.1\include]
g++ main.o -o main -L[path to SFML-2.6.1\lib] -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32

Example:
g++ -c main.cpp -IC:\Users\tsvet\SFML-2.6.1\include
g++ main.o -o main -LC:\Users\tsvet\SFML-2.6.1\lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32
