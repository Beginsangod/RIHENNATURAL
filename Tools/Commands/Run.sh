#!/bin/bash

BUILD_DIR="Build"
BIN_DIR="$BUILD_DIR/Bin"
OUTPUT_FILE="RihenNatural"

if [ ! -f "$BIN_DIR/$OUTPUT_FILE" ]; then
    echo "Error: Executable not found. Run 'nken build' first."
    exit 1
fi

echo "Running RihenNatural..."
cd "$BIN_DIR"
./"$OUTPUT_FILE"
cd ../..