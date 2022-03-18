@echo off

:: if first arg is CALLCPP -> must include the sum.dll used by NativeCpp.add.
IF "%1" == "" (
   echo No C++ call
)

echo "%1"

:: clean
IF EXIST build RMDIR /S /Q build

IF EXIST ..\libsharp\bin RMDIR /S /Q ..\libsharp\bin
dotnet clean ../libsharp/LibSharp.csproj

:: publish the C# library with AOT options thanks to the import package ILCompiler.
IF "%1" == "cppcall" (
dotnet publish --nologo -r win-x64 -c Release /p:DefineConstants=CALLCPP /p:SelfContained=true /p:NativeLib=Shared -o build ../libsharp/LibSharp.csproj
) ELSE (
dotnet publish --nologo -r win-x64 -c Release /p:SelfContained=true /p:NativeLib=Shared -o build ../libsharp/LibSharp.csproj
)

:: ------> ONLY IF NativeCpp is called into LibSharp <--------
IF "%1" == "cppcall" (
   pushd ..\libnative
   build.bat
   popd
  :: as P/Invoke requires the relative path, and we set it to "sum.dll",
  :: we need to copy this file at the same level of the program ENTRY POINT.
  :: So in this case, the main.exe... and not the LibSharp.dll.
  :: The build system has the responsability to organize those files.
  :: (all dlls in the same directory, etc...)
  :: It will works because in the C# DllImport into libsharp/WrapCppLib.cs,
  :: when published in release, the dll is referenced into build/.
   Copy ..\libnative\build\sum.dll build\sum.dll
)

:: Build the main.
cl.exe main.c -D WINDOWS
:: then execute the main as follow
:: > main.exe sharp-aot-build\LibSharp.dll
