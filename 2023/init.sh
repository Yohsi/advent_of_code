#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "expected 1 arg"
    exit 1
fi

day=$1

touch "inputs/day${day}"
touch "samples/day${day}"
cp src/template.rs "src/day${day}.rs"
echo "
[[bin]]
name = \"day${day}\"
path = \"src/day${day}.rs\"
" >> Cargo.toml