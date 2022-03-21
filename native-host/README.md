# Host dotnet runtime using `hostfxr`

Hosting the dotnet runtime allows native code (C++ typically) to configure
and start the dotnet runtime. Therefore, the native code is now able
to call any managed assembly compiled for the dotnet runtime.
Here, there is a great introducing [tutorial](https://docs.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting) from Microsoft.

## .NET SDK

The only prerequisities is to have the .NET SDK installed/downloaded.
This repo is using [.NET6 SDK](https://dotnet.microsoft.com/en-us/download).
The installer will install the SDK on the user machine.
For more control over the version, the SDK can also be downloaded for a specific architecture.
[Here](https://dotnet.microsoft.com/en-us/download/dotnet/6.0) is the full list for .NET6 for instance.

Examples below are given for .NET SDK version 6.0.3, on win-x64 architecture.

Once a SDK is installed or downloaded, to successfully host .NET runtime from native code, we need some native APIs to interact with:

1. `hostfxr.dll`, which can be found into `sdk_root_path/host/fxr/6.0.3/`.
2. `hostfxr.h` and `coreclr_delegates.h`, which can be found into `sdk_root_path/packs/Microsoft.NETCore.App.Host.win-x64\6.0.3\runtimes\win-x64\native/`
3. [OPTIONAL] If you want to use `nethost` to automatically detects installed SDK root path we need:
   * [`nethost.dll`, `nethost.lib`] (on windows `.lib` is for the linker)
   * `nethost.h`
   that can be found at the same location as `hostfxr.h` and `coreclr_delegates.h`.
   The only function from `nethost` that is usefull to host .NET runtime into native code is `get_hostfxr_path` which automatically search for `hostfxr.dll` (or `.so`) on the host machine.

On linux, replace `hostfxr.dll` by `libhostfxr.so`, and [`nethost.lib`, `nethost.dll`] by `libnesthost.so`.

To initialize and use a runtime from native code, the path of `hostfxr.dll` is required. Or it's already known in advance so `nethost` is not usefull. Or we don't know a path to `hostfxr.dll` and we use `nethost` to know search for it.

In conclusion, depending the project, if the whole SDK is embedded, no need to use `nethost` and a relative path to the SDK into the project can be user. Or, simply ask to the user to install the .NET SDK on his machine, and then use `nethost` to get this path automatically.

FIXME: Need to keep searching exactly how (with `nethost`) a version selection of SDKs can be done if mulitple SDKs are installed. Or if `nethost` takes the most up to date version on the machine.

## Managed assembly to be loaded from hosted runtime

In this example, C# project is used to then be compiled into an assembly `dll` which contains `IL` code and then be loaded from the hosted runtime into native code.

To compile the C# project there is no dependency required, just compile the C# project in a normal fashion.

The only restriction is at a code level. Not all C# methods can be called from hosted runtime in native code. In fact, the function needs to be `static`, and must use types that can be understood by the native code => [marshalling](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/type-marshalling).

It is important that, when the C# project is compiled the option `/p:GenerateRuntimeConfigurationFiles=true` must be specified to generate a configuration file for the runtime. This option can be specified in the `csproj` or directly using the command line.

To then call a managed method, the `hostfxr` api requires four steps:

1. Load the `hostfxr` library to access the runtime API.

2. Initialize the .NET runtime with a configuration file. This configuration file gives `hostfxr` the parameters to correctly initialize the runtime and be able to run our library. The configuration file is generated on the build of the C# project (cf. option `/p:GenerateRuntimeConfigurationFiles=true`) on the `dotnet build` command.

3. Load the method to be called from native code, giving which requires:
   * The library path (the `dll` generated when C# is compiled)
   * The dotnet type which contains the fully resolved type's name with namespaces (`namespace.classname`) and the name of the assembly.
   * The dotnet type method, which is the static method's name we want to call from native code.
   
4. Call the function through the pointer returned at the step 3.


## Usage

To use the example here:
1. Compile with `build.[sh/bat]`
2. Run the `main.[exe/out]` without argument to run with nethost (and then autolocate SDK).
   OR
   Run `main.[exe/out] path/to/sdk/hostfxr.[dll,so]` to run with the specified path to hostfxr.

