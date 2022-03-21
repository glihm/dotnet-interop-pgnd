@echo off

:: clean
IF EXIST build RMDIR /S /Q build
mkdir build

pushd build
cl.exe ../*.cpp^
  /D WINDOWS^
  /Od /GS /EHsc /sdl /Zi^
  /LD^
  /Fe"libnative.dll"
popd
