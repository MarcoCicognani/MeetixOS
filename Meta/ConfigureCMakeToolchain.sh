#!/bin/bash

SOURCE_DIR=$(realpath ..)
OUT_PATH="$SOURCE_DIR/Build"
OUT_FILE="$OUT_PATH/CMakeToolchain.txt"

echo "Making Dir '$OUT_PATH'..."
mkdir -p "$OUT_PATH" || exit 1

echo -n "Sed CMake/CMakeToolchain.txt.template..."
sed -e 's|@SOURCE_DIR@|'$SOURCE_DIR'|' CMake/CMakeToolchain.txt.template >"$OUT_FILE" || exit 1
echo "Done"
