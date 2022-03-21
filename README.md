# Dotnet interop playground

Playground to test interop between managed and unmanaged code using .NET.


## How .NET (managed) can call native libraries
From the cross-platform perspective, the solution is [P/Invoke](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke).
With few lines of code, a C# code can call a function from a native library loaded with `DllImport`.

The important point using `DllImport` is to place the dll/so files correctly. The `DllImport` path is relative to the main program executing, and not the C# dll! (need to be tried but perhaps absolute paths works well... but deploying an app is never done with absolute path as the distant machine configuration is not known in advance). Build systems are here to copy files where they have to be before being deployed.

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
- `managed-pinvoke`: C# code calls C++ library with P/Invoke (explicit).
- `native-aot`: C program calls C# from a native assembly compiled from C# with NativeAOT.
- `native-host`: C++ program calls C# from a hosted runtime using `hostfxr`.

## Compilation
Each project that requires compilation has a `build.bat` and `build.sh` file. No other tool is used in order to keep things very simple.
In a real world project, MsBuild using csproj, Scons or [c]Make are usually used.
Each project contains a README.md to contextualize more specifically is needed.


## Disclaimer
This project is very exploratory and experimental. So any advices or remarks are very welcomed!

