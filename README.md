# How to try this program on your local machine

## Requirements
- **CMake (version >= 3.29) or Make**
- **C++ (version >= 20)**

## Build and Run instructions
### Using Make:
- `make all` - build and run
- `make configure` - generating cmake files in build folder
- `make build` - build
- `make run` - run
- `make clean` - clean build folder
- `make rebuild` - clean, build and run

### Using CMake:
- If you don't have `Make` but have `CMake` run the following command in the root folder of the project:
```
mkdir build && cd build && cmake .. && cmake --build .
```
- Then run the program: `start cube_rotation.exe`

## Dependencies
- [SFML 2.6.1](https://www.sfml-dev.org/) (licensed under zlib/png license)
