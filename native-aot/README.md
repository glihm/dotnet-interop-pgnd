# NativeAOT

NativeAOT allows IL code to be compiled into machine code for a specified architecture.
This can be seen as the trade-off between those two statements:
> "I want native code with performances and easy to import into native doe project, without importing the dotnet runtime entirely"

and

> "I am aware that I am loosing the whole dotnet ecosystem portability"


This feature still in preview, the repo is [here](https://github.com/dotnet/runtimelab/tree/feature/NativeAOT) and detailed readme [here](https://github.com/dotnet/runtimelab/blob/feature/NativeAOT/docs/using-nativeaot/README.md).

The code samples comes from here, they are just adapted and extended for comprehension purposes.

## Compilation
Very easy to add NativeAOT to a project. Just add the NuGet package to your `csproj`.
```XAML
<ItemGroup>
  <PackageReference Include="Microsoft.DotNet.ILCompiler" Version="7.0.0-*" />
</ItemGroup>
```

Then, the project must be `publish`ed as following (for instance for windows x64):
`dotnet publish -r win-x64 -c Release`

If the compiled application is a library, add the option `/p:NativeLib=Shared`.

And.. that's it. This will output the natively compiled code for the platform/runtime (-r option) specified in the command line.

Important note from the dotnet CLI:
> Optimizing assemblies for size, which may change the behavior of the app. Be sure to test after publishing. See: https://aka.ms/dotnet-illink

In this example, the `build.[bat/sh]` is always recompiling the c program and the `libsharp`. This is not required if for instance only one of the two is modified, you just need to recompile the one you want, in a independent fashion.

For linux, `clang` must be installed as it is used by the dotnet IL compiler.
Libraries `libz-dev` and `libkrb5-dev` are required.


## Usage
Now we have a native library compiled from C# code, we need to load it from native code.
Windows uses `LoadLibraryA` and Linux `dlopen`. Giving the path of the library, it can be load to then return an handle on it. This handle will allow further access to the methods of the library.

To call a function of the C# natively compiled library (nativeAOT), we always need to:
1. Provide the handle on the library we loaded
2. Provide the function's name
3. Provide the function's arguments where [marshalling](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/type-marshalling) comes in.

For this example, the library path is given as argument to the main program.

To build the exampe using a call to C++ from the C#, add CALLCPP as argument for build scripts.
This was a test to check if C# code that is using `DllImport` is able to be compiled using NativeAOT.
The important point when using other native library, the DLLs MUST be placed correctly to be loaded by `DllImport` which uses a relative path.

On windows, open a [x64 Native Tools Command Prompt](https://docs.microsoft.com/en-us/cpp/build/how-to-enable-a-64-bit-visual-cpp-toolset-on-the-command-line?view=msvc-170) and run:

`> build.bat [CALLCPP]`
`> main.exe build\LibSharp.dll`

On linux, in a terminal run:
`$ bash build.sh [CALLCPP]`
`$ main.out build/LibSharp.so`
