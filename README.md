# Installation
To install luciconnect, clone the repository including submodules using the `--recursive` tag:
```
git clone --recursive https://github.com/mtfranzen/luci-connect.git
```

## Linux
Install the following requirements and run `cmake CMakeLists.txt && sudo make install`
 * *cmake*
 * *make*
 * *libasio-dev*
 * *g++*

## Windows
The easiest way to get things running on Windows is to install
  * Visual C++
  * CMake
  * [Boost](https://sourceforge.net/projects/boost/files/boost-binaries/)

Afterwards, run CMake and choose the cloned repository as source directory.

# Documentation
1. Install the following requirements:
 * *graphviz*
 * *doxygen*
2. Run:
```
doxygen doc/Doxyfile```

# Examples
1. Start `helen` or `luci`
2. Build the samples
```
cmake samples/CMakeLists.txt && (cd samples && make)
```
3. Run the sample service `samples/bin/simple-service` to register an example service that adds two numbers delivered by some client via Luci
4. Run the sample client `samples/bin/simple-client <number1> <number2>` in a third terminal to sum up the two arguments.

# Tests
1. Run `helen` or `luci`
2. Install the following requirements:
 * *libgtest-dev*
3. Run `./bin/tests`
