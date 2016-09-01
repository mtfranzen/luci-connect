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

3. Run `cmake CMakeLists.txt && make`

## Tests
1. Run `helen` or `luci`
2. Install the following requirements:
 * *libgtest-dev* (you need to compile from source and place the binaries in /usr/lib)
3. Run `./bin/tests`

## Examples

1. Start `helen` or `luci`
2. Build the samples
```
cmake samples/CMakeLists.txt && (cd samples && make)
```
3. Run the sample service `samples/bin/simple-service` to register an example service that adds two numbers delivered by some client via Luci
4. Run the sample client `samples/bin/simple-client <number1> <number2>` in a third terminal to sum up the two arguments.

## Documentation
1. Install the following requirements:
 * *graphviz*
 * *doxygen*
2. Run:
```
doxygen doc/Doxyfile
```
