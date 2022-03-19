@echo off

:: clean
IF EXIST build RMDIR /S /Q build
IF EXIST main.exe RM /Q main.exe
mkdir build

IF EXIST ..\libsharp\bin RMDIR /S /Q ..\libsharp\bin

dotnet clean ../libsharp/LibSharp.csproj

dotnet build^
       -c Release^
       -o build^
       /p:DefineConstants=WINDOWS^
       /p:GenerateRuntimeConfigurationFiles=true^
       ../libsharp/LibSharp.csproj

pushd build
cl.exe ..\src\Hostfxr_ctx.cpp^
       ..\src\main.cpp^
       /D WINDOWS^
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

:: dll must be placed at the same level of the executable.
Copy lib\nethost.dll .

:: then execute the main as follow
:: > build/main.exe
