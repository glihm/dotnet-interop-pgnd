@echo off

:: clean
IF EXIST build RMDIR /S /Q build
mkdir build

:: publish the C# library with AOT options thanks to the imported package ILCompiler.
dotnet publish --nologo^
       	       -r win-x64^
	       -c Release^
	       /p:DefineConstants=WINDOWS^
	       /p:SelfContained=true^
	       /p:NativeLib=Shared^
	       -o build^
	       libsharp-aot/LibSharpAOT.csproj
	       
:: Build the main.
pushd build
cl.exe ../src/dl.c^
       ../src/libsharp_aot.c^
       ../src/main.c^
       /D WINDOWS^
       /I ../inc^
       /std:c11^
       /Fe"../main.exe"^
       /W4
popd
