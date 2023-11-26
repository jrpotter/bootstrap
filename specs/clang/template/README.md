# Clang Flake Template

This is a template for constructing a working environment for C development with
the [clang](https://clang.llvm.org/) (version 16.0.6) toolchain. [direnv](https://direnv.net/)
can be used to launch a dev shell upon entering this directory (refer to
`.envrc`). Otherwise run via:
```bash
$> nix develop
```

## Language Server

The [clangd](https://clangd.llvm.org/) LSP (version 14.0.6) is included in this
flake. The [codelldb](https://github.com/vadimcn/codelldb) VSCode plugin is also
included to interface with the LSP. Note this plugin, despite its name, is
compatible with other editors (e.g. neovim). To configure, refer to your
editor's documentation. To use the LSP across files, a [compilation database](https://clang.llvm.org/docs/JSONCompilationDatabase.html)
must be generated. For this reason, [bear](https://github.com/rizsotto/Bear)
(version 3.1.3) is included in this flake. Use as follows to generate a
`compile_commands.json` file:
```bash
$> bear -- <your-build-command>
```

## Documentation

Documentation is generated using [Doxygen](https://www.doxygen.nl/index.html)
(version 1.9.7). (Re)generate documentation by editing the `Doxyfile` and
running:
```bash
$> doxygen
```

## Formatting

Formatting depends on the [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
(version 14.0.6) tool. Refer to `.clang-format` for default formatting options.
A `pre-commit` hook is included in `.githooks` that can be used to format all
`*.c` and `*.h` files prior to commit. Install via:
```bash
$> git config --local core.hooksPath .githooks/
```
If running [direnv](https://direnv.net/), this hook is installed automatically
when entering the directory.
