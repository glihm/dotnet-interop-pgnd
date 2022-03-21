#!/usr/bin/bash

set -e

if [ ! -d "./build" ]; then
    mkdir build
fi

clang++ *.cpp \
	-D LINUX \
	-shared \
	-Wall \
	-Wextra \
	-g \
	-o build/libnative.so
