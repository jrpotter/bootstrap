#!/usr/bin/env bash

if [ -z "$VERSION" ]; then
  VERSION="default"
fi

cp "$VERSION/flake.nix" "$OUT"
