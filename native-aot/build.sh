#!/bin/bash

# exits when any command fails
set -e

# clean
if [ -d "./build" ]; then
    rm -rf ./build    
fi

# publish the C# library with AOT options thanks to the imported package ILCompiler.
if [[ -n "$1" ]]; then
    dotnet publish --nologo \
	   -r linux-x64 \
	   -c Release \
	   /p:SelfContained=true \
	   /p:NativeLib=Shared \
	   /p:DefineConstants=LINUX \
	   -o build \
	   ../libsharp/LibSharp.csproj

# clang or gcc depending your choice
clang src/dl.c \
      src/libsharp_aot.c \
      src/main.c \
      -D LINUX \
      -ldl \
      -pthread \
      -o main.out
