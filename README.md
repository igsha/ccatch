# ccatch
Unit Testing Framework for pure C inspired by [Catch](https://github.com/philsquared/Catch).

Long way to test functionality:
```sh
$ mkdir build
$ cd build
build/ $ cmake ..
build/ $ cmake --build . -- -j
build/ $ cmake --build . -- test
```
Short way to test functionality:
```sh
$ gcc -Iinclude/ -o ccatch_test tests/*.c
$ ./ccatch_test
```
