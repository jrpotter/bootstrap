# Clang Flake Template

This is a template for constructing a working environment for C/C++ development
with the [clang](https://clang.llvm.org/) (version 16.0.6) toolchain. [direnv](https://direnv.net/)
can be used to launch a dev shell upon entering this directory (refer to
`.envrc`). Otherwise run via:
```bash
$ nix develop
```

## Building

We use [CMake](https://cmake.org/) (version 3.27.7) to build the project. When
first starting the project, we recommend running the following commands:
```bash
$ mkdir -p build/{Debug,Release}
$ (cd build/Debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..)
$ (cd build/Release && cmake -DCMAKE_BUILD_TYPE=Release ../..)
```
This will create a CMake cache file in each subdirectory with the build types
set. Now you can build a `Debug` or `Release` variant by navigating to the
corresponding subdirectory and running:
```bash
$ cmake --build .
```

## Language Server

The [clangd](https://clangd.llvm.org/) LSP (version 14.0.6) is included in this
flake. The [codelldb](https://github.com/vadimcn/codelldb) VSCode plugin is also
included to interface with the LSP. Note this plugin, despite its name, is
compatible with other editors (e.g. neovim). To configure, refer to your
editor's documentation.

To use the LSP across files, a
[compilation database](https://clang.llvm.org/docs/JSONCompilationDatabase.html)
must be generated. The `CMakeLists.txt` file already enables this in the Debug
configuration type. A top-level `compile_commands.json` symbolic link already
exists and points to this generated database.

## Documentation

Documentation is generated using [Doxygen](https://www.doxygen.nl/index.html)
(version 1.9.7) through CMake. (Re)generate documentation by navigating to
the desired build configuration directory and running:
```bash
$ cmake --build .
$ make docs
```

## Testing

We use [CTest](https://cmake.org/cmake/help/latest/module/CTest.html) (version
3.27.7) for unit testing. To run the tests, navigate to `build/Debug` and type
the following:
```bash
$ cmake --build .
$ make test
```

## Formatting

Formatting depends on the [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
(version 14.0.6) tool. Refer to `.clang-format` for default formatting options.
A `pre-commit` hook is included in `.githooks` that can be used to format all
`*.c(pp)?` and `*.h(pp)?` files prior to commit. Install via:
```bash
$ git config --local core.hooksPath .githooks/
```
If running [direnv](https://direnv.net/), this hook is installed automatically
when entering the directory.
