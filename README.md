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
2. Run:
```make test```

## Documentation
1. Install the following requirements:
 * *graphviz*
 * *doxygen*
2. Run:
```make docs```
