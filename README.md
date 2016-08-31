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
```cmake && make```

## Tests
1. Install the following requirements:
 * *libgtest-dev* (you need to compile from source and place the binaries in /usr/lib)
 * *haskell-stack*
 * *llvm*
2. Run:
```./bin/tests```

## Documentation
1. Install the following requirements:
 * *graphviz*
 * *doxygen*
2. Run:
```doxygen doc/Doxyfile```

## Examples

(TODO)

To try it, run Luci or, preferably `helen`. Then run the example `SimpleService.o` to register an example service that adds two numbers delivered by some client via Luci. Finally, run `SimpleClient.o <arg1> <arg2>` in a third terminal to sum up the two arguments.
