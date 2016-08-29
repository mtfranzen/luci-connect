## Building from source
1. Clone the repository including submodules using the `--recursive` tag:
```
git clone --recursive http://129.132.6.17/ADvISE/QUA-Services.git
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

To try it, run Luci or, preferably `helen`. Then run the example `SimpleService.o` to register an example service in Luci that adds two numbers. Note that both these processes run continuously.
Finally, run `SimpleClient.o <arg1> <arg2>` to sum up the two arguments.
