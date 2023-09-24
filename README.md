# SimpleDgsSort

## Prerequisites

Notes: Debian Based linux's include: Debian, Ubuntu, and Mint

1. Use Linux or Mac.
    - Seriously, who does this stuff with Windoze?
2. Ensure you have your basic build tools installed:
    - Linux (Debian Based): `sudo apt install build-essential`
    - Mac: Install XCode from the App Store
3. Ensure zlib is installed:
    - Linux (Debian Based): `sudo apt install zlib1g-dev`
    - Mac (with HomeBrew): `brew install zlib`
4. Ensure CERN's ROOT Framework is installed:
    - Linux (most flavors): Go through annoying ROOT build and install procedure
    - Mac (with HomeBrew): `brew install root`
5. Ensure CMake is Installed:
    - Linux(Debian Based): `sudo apt install cmake`
    - Mac (with HomeBrew): `brew install cmake`

## Build

1. Open a terminal and navigate to a directory for code projects
2. Clone this Repository (`git clone git@github.com:jmatta1/SimpleDgsSort.git` or `git clone https://github.com/jmatta1/SimpleDgsSort.git`)
3. Navigate into the directory created `cd SimpleDgsSort`
4. Enter the command `make -j <num> release` where <num> is the number of cores to use for building.

