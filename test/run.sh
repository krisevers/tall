#!/bin/bash

OBJ_DIR="obj"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

if [ ! -d "$OBJ_DIR" ]; then
    mkdir "$OBJ_DIR"
fi

# Optional test number argument
if [ -n "$1" ]; then
    pattern="$(printf '%02d' "$1")_*.cc"
else
    pattern="*.cc"
fi

# run tests
for src in $pattern; do
    [ -e "$src" ] || continue
    exe="$OBJ_DIR/${src%.cc}"
    g++ --std=c++23 -g "$src" -o "$exe"
done

all_passed=true

for exe in "$OBJ_DIR"/*; do
    if [ -n "$1" ]; then
        # Only run the selected test
        [[ "$exe" == "$OBJ_DIR/$(printf '%02d' "$1")_"* ]] || continue
    fi
    if [ -x "$exe" ]; then
        "$exe"
        if [ $? -ne 0 ]; then
            echo -e "Test $exe ${RED}FAILED${NC}."
            all_passed=false
        else
            echo -e "Test $exe ${GREEN}PASSED${NC}."
        fi
    fi
done

if $all_passed; then
    echo -e "${GREEN}All tests passed.${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
