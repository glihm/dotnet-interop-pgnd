# Dotnet interop playground

Playground to test interop between managed and unmanaged code using dotnet.


## How .NET (managed) can call native libraries
From the cross-platform perspective, the solution is [P/Invoke](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke).
With few lines of code, a C# code can call a function from a native library loaded with `DllImport`.

The important point using `DllImport` is to place the dll/so files correctly. The `DllImport` path is relative to the main program executing, and not the C# dll!


## How native code (unmanaged) can call .NET (managed) functions.
Here, there is two ways:

1. Embedding the .NET runtime into a native application, and the runtime is instanciated
   by this application which then can execute any managed `dll` (`IL`).
   This required a bit of setup adding the required headers for the native application
   to compile, and the runtime must be accessible to actually run the `dll`.
2. Using the recent NativeAOT feature, which compiles the `IL` directly to `native` code.
   Very easy, only one NuGet package to import into the .NET project and a publish. And the runtime is no longer required.

In both cases, there are some limitations, and the most important ones are:
	* Functions in C# must be static.
	* Limited to primitive types (and see [marshalling](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/type-marshalling)).

In this project, both implementations are done for experimental purposes.


## Goals
The goals here are:
	* Understand how interop works
	* Test Marshalling to know how data can be passed back and forth


## Repo hierarchy
- `libnative`: A C/C++ library that is aimed at mocking an existing library with already lot's of work done. We typically want to wrap such library to be used from C#.

- `libsharp`: A C# library that will wrap and use the `libnative` for some computations. This library will:
* use pure C# classes
* use `libnative` using `DllImport`
* be compiled in `IL` (as normally done) to be used into `native-host`
* be compiled using NativeAOT into machine code, to then be loaded as a library into `native-aot`.

- `native-aot`: A C/C++ program that wants to use the C# library from it's native form.
- `native-host`: A C/C++ program that wants to use the C# from an hosted dotnet CLR.

- `testing`: A xUnit project to test the C# library directly and explore marshalling.


## Compilation
Each project that requires compilation has a `build.bat` and `build.sh` file. No other tool is used in order to keep things very simple.
In a real world project, MsBuild using csproj, Scons or Makefile are usually used.
Each project contains a README.md to contextualize the reader.


## Disclaimer
This project is very exploratory and experimental.
Done by a developer very new to the dotnet interop and with a very little experience in C/C++.
So any advices or remarks are very welcomed!

