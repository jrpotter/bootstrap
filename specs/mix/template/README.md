# Mix Flake Template

This is a template for constructing a working environment for Elixir development
(version 1.15.7, Erlang/OTP 25) with the [mix](https://hexdocs.pm/mix/1.12/Mix.html)
tool. [direnv](https://direnv.net/) can be used to launch a dev shell upon
entering this directory (refer to `.envrc`). Otherwise run via:
```bash
$> nix develop
```

## Language Server

The [elixir-ls](https://github.com/elixir-lsp/elixir-ls) LSP (version 0.17.10)
is included in this flake.

## Dependencies

This project pins Mix dependencies using [mix2nix](https://github.com/ydlr/mix2nix).
After updating your `mix.lock` file, make sure to re-run the following:
```bash
mix2nix > deps.nix
```
As of now, `mix2nix` cannot handle git dependencies found inside the `mix.lock`
file. If you have git dependencies, add them manually or use
[FODs](https://nixos.org/manual/nixpkgs/stable/#packaging-beam-applications).
