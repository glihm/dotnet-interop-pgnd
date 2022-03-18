@echo off
if not exist "build" mkdir build
pushd build

cl.exe ../*.cpp^
  /D WINDOWS^
  /Od /GS /EHsc /sdl /Zi^
  /LD
  /Fe libnative.dll

popd
