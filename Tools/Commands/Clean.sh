#!/bin/bash

BUILD_DIR="./Build"

echo "Cleaning build files..."

if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
    echo "Removed $BUILD_DIR"
fi

echo "Clean completed."