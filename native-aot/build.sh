#!/bin/bash

# exits when any command fails
set -e

# if first arg is CALLCPP -> must include the sum.dll used by NativeCpp.add.
if [[ ! -n "$1" ]]; then
    echo "No C++ call"
fi

# clean
if [ -d "./build" ]; then
    rm -rf ./build    
fi

if [ -d "../libsharp/bin" ]; then
    rm -rf "../libsharp/bin"
fi

dotnet clean ../libsharp/LibSharp.csproj

# publish the C# library with AOT options thanks to the imported package ILCompiler.
if [[ -n "$1" ]]; then
    dotnet publish --nologo \
	   -r linux-x64 \
	   -c Release \
	   /p:SelfContained=true \
	   /p:NativeLib=Shared \
	   /p:DefineConstants=LINUX \
	   /p:DefineConstants=CALLCPP \
	   -o build \
	   ../libsharp/LibSharp.csproj
else
    dotnet publish --nologo \
	   -r linux-x64 \
	   -c Release \
	   /p:SelfContained=true \
	   /p:NativeLib=Shared \
	   /p:DefineConstants=LINUX \
	   -o build \
	   ../libsharp/LibSharp.csproj
fi

if [[ -n "$1" ]]; then
    pushd ../libnative/
    bash build.sh
    popd
    cp ../libnative/build/libnative.so build/
fi

# clang or gcc depending your choice
clang main.c -D LINUX -ldl -pthread -o main.out
# then runs:
# $ ./main build/LibSharp.so
