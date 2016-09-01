## Building from source on Linux
1. Clone the repository including submodules using the `--recursive` tag:
```
git clone --recursive https://github.com/mtfranzen/luci-connect.git
```
2. Install the following requirements:
 * *cmake*
 * *make*
 * *libasio-dev*
 * *g++*

3. Run:
```
cmake && make
```

## Tests
1. Install the following requirements:
 * *libgtest-dev* (you need to compile from source and place the binaries in /usr/lib)
 * *haskell-stack*
 * *llvm*
2. Run:
```
./bin/tests
```

## Documentation
1. Install the following requirements:
 * *graphviz*
 * *doxygen*
2. Run:
```
doxygen doc/Doxyfile
```

## Examples

1. Start `helen` or `luci` and run `cmake CMakeLists.txt && make` in the directory `samples/`.
2. Run the sample service `bin/simple-service` to register an example service that adds two numbers delivered by some client via Luci
3. Run the sample client `bin/simple-client <number1> <number2>` in a third terminal to sum up the two arguments.
