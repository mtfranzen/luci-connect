# Installation
To install Luciconnect on Windows or Debian/Ubuntu, choose one of the available [release binaries](https://github.com/mtfranzen/luci-connect/releases).

## Dependencies:
 * [Asio](http://www.think-async.com) - Either by installing the standalone version or using the boost-libraries.

# Build from Source
The project can be compiled on both Linux and Windows using CMake and the respective build system such as *make* or *Visual++*

## Documentation
The documentation can be generated using Doxygen.

## Tests
The tests depend on [GTest](https://github.com/google/googletest). To run the tests after compiling, execute the binary `bin/tests` after startin *Helen* or *Luci*.


## Examples
Compile the samples in the `samples/` folder. The binary  `samples/bin/simple-service` registers a simple service for adding two numbers provided by a client. A client can be  such as the binary `samples/bin/simple-client <number1> <number2>`. Not that an instance of *Helen* or *Luci* must be running.
