## Dependencies:
 * [Asio](http://www.think-async.com) - Either by installing the standalone version (`libasio-dev`) or using the boost-libraries.

# Install Binaries

# Install from Source
To build and install libluciconnect from source, clone the repository  using the **recursive** tag:
 ```git clone --recursive git@github.com:mtfranzen/luci-connect.git```

## Linux
1. Install [CMake](https://cmake.org/)
2. Build the project using CMake and Make (**tested with g++**)
  ```cmake . && make install```

## Windows
1. Install [Boost](http://www.boost.org/)
2. Install [CMake](https://cmake.org/)
3. Build the project using CMake and MSVC (**tested with Visual Studio 2015**), make sure to specify the correct foldes
  ```cmake -DBOOST_ROOT="C:/local/boost_1_63_0/" -DBOOST_LIBRARYDIR="C:/local/boost_1_63_0/lib64-msvc-14.0" -DBoost_COMPILER="-vc140" -G "Visual Studio 14 2015 Win64"```
4. Open the generated solution in VS
5. Compile the Package `INSTALL` or `PACKAGE` with subsequently running the generated installer

## Documentation
The documentation can be generated using Doxygen.

## Tests
The tests depend on [GTest](https://github.com/google/googletest). To run the tests after compiling, execute the binary `bin/tests` after starting *Helen* or *Luci*.

## Examples
Compile the samples in the `samples/` folder. The binary  `samples/bin/simple-service` registers a simple service for adding two numbers provided by a client. A client can be  such as the binary `samples/bin/simple-client <number1> <number2>`. Not that an instance of *Helen* or *Luci* must be running.
