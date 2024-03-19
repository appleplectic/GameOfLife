# Game Of Life
A small C++ and SFML implementation of Conway's Game of Life, made by Levin.

## Usage

Download the latest release from the releases page and run the executable (unzip if necessary). On Linux, some of the dependencies listed below may be necessary.

## Building from Source

### On Linux
First install the necessary dependencies - below is an example on Ubuntu.

```bash
sudo apt-get install -y cmake build-essential git libx11-dev mesa-common-dev libudev-dev libopenal-dev libvorbis-dev libflac-dev libxrandr-dev libxcursor-dev libfreetype-dev libfreetype6 libfreetype6-dev
```

Clone, build & make:
```bash
git clone https://github.com/appleplectic/GameOfLife.git
cd GameOfLife
mkdir build
cd build
cmake ..
cmake --build .
```

The resulting binary should be available in the `bin/` directory.

### On Windows

A sutiable build system (MinGW/MSys2/etc.) must be installed.

Clone, build & make:
```bash
git clone https://github.com/appleplectic/GameOfLife.git
cd GameOfLife
mkdir build
cd build
cmake ..
cmake --build .
```

The resulting binary should be available in the `bin/` directory.
