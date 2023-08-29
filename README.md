# cmake-progress
Progress bars for cmake builds

This is useful for **emerge** builds which can sometimes compile thousands of files using cmake 👀

## Installation
This project has no dependencies, except you want to build the test-suite too.
```shell
cmake -B build
cmake --build build --target install
```
If you only want to build the program, without installing it, replace `cmake_progress` by `install` in the previous command.

If you want to run the test-suite too, you'll need to have [criterion](https://criterion.readthedocs.io/en/master/intro.html) installed.
```shell
cmake -B build
cmake --build build --target cmake_progress_test
```
To run the test suite, do `./build/cmake_progress_test`.

## Usage

The program reads the stdin and outputs it to the standard output, except for cmake output. The cmake output is replaced by a progress bar (hence the name of this program) :
```shell
sudo emerge -a dev-libs/libgit2 | cmake_progress
```
