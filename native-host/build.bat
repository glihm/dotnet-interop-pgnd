@echo off

:: clean
IF EXIST build RMDIR /S /Q build
mkdir build

dotnet clean libsharp-host/LibSharpHost.csproj

dotnet build^
       -c Release^
       -o build^
       /p:DefineConstants=WINDOWS^
       /p:GenerateRuntimeConfigurationFiles=true^
       libsharp-host/LibSharpHost.csproj

pushd build
cl.exe ..\src\Dotnet_host.cpp^
       ..\src\main.cpp^
       /D WINDOWS^
       /std:c++17^
       /W4^
       /I ..\inc^
       /EHsc^
       /Od^
       /GS^
       /sdl^
       /Fe"..\main.exe"^
       /link ..\lib\nethost.lib
popd
:: add /Zi for debug files.

:: dll must be placed at the same level of the executable!
:: the lib folder is just here to old a persistent copy as build is erased on clean.
Copy lib\nethost.dll .
