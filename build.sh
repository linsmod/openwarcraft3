#!/bin/bash
# Build script for openwarcraft3

usage() {
    echo "Usage: $0 [--debug]"
    echo "Options:"
    echo "  --debug    Build debug version"
    exit 1
}

# Parse arguments
DEBUG=false
while [[ $# -gt 0 ]]; do
    case "$1" in
        --debug)
            DEBUG=true
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

cd ~/openwarcraft3
./tools/bin/premake5 gmake2

if $DEBUG; then
    echo "Building debug version..."
    cd build/gmake2 && make config=debug_x86_64
else
    echo "Building release version..."
    cd build/gmake2 && make config=release_x86_64
fi

cd ~/openwarcraft3