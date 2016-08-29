## Building from source
1. Clone the repository including submodules using the `--recursive` tag:
```
git clone --recursive https://github.com/mtfranzen/luci-connect.git
```
2. Install the following requirements:
 * *make*
 * *libasio-dev*
 * *g++*
3. Run:
```make build```

## Tests
1. Install the following requirements:
 * *openjdk-9-jdk*
 * *maven*
 * *haskell-stack*
 * *llvm*
2. Run:
```make test```

## Documentation
1. Install the following requirements:
 * *graphviz*
 * *doxygen*
2. Run:
```make docs```

## Examples

To compile the examples to `/samples/bin/`, run
```make samples```

To try it, run Luci or, preferably `helen`. Then run the example `SimpleService.o` to register an example service that adds two numbers delivered by some client via Luci. Finally, run `SimpleClient.o <arg1> <arg2>` in a third terminal to sum up the two arguments.
