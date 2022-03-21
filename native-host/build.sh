#!/bin/bash

# exits when any command fails
set -e

#clean
if [ -d "./build" ]; then
    rm -rf ./build
fi

dotnet clean libsharp-host/LibSharpHost.csproj

dotnet build \
       -c Release \
       -o build \
       /p:DefineConstants=LINUX \
       /p:GenerateRuntimeConfigurationFiles=true \
       libsharp-host/LibSharpHost.csproj

cp lib/libnethost.so .

clang src/Dotnet_host.cpp \
      src/main.cpp \
      -Iinc \
      -L"." \
      -DLINUX \
      -std=c++17 \
      -o main.out \
      -g \
      -lstdc++ \
      -ldl \
      -lnethost \
      -lpthread \
      -Wl,-rpath,'$ORIGIN',--disable-new-dtags


