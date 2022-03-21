using System;
using System.Runtime.InteropServices;

namespace LibSharp;

/// <summary>
///   A wrapper class that uses the native compiled C++ library.
///   
///   DllImport is the way to do explicit P/Invoke.
///   https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke
/// </summary>
public static class NativeCpp
{
    /// <summary>
    ///   The DllImport path must be known at compile time and it's relative
    ///   to the appication executable when run.
    ///   
    ///   In the case of this project, LibSharp is not executing the code directly
    ///   as it's a library, the xUnit project does.
    ///   
    ///   A recommended way is to only put the dll name, and then with the build system,
    ///   copying the dll file at the correct location. Like this the dll location is always the same.
    ///   
    ///   In this example, as we may want to run the tests from VS test explorer, we need a relative
    ///   path from the xUnit project (and not LibSharp).
    ///   The xUnit project is built into testing/bin.
    /// </summary>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <returns></returns>
    #if LINUX
    [DllImport("../../libnative/build/libnative.so", EntryPoint = "add")]
    #else
    [DllImport("../../libnative/build/libnative.dll", EntryPoint = "add")]
    #endif
    public static extern Int32 add(Int32 a, Int32 b);


    // TODO: add more functions into the CPP library and import them to illustrate
    // the marshalling and how it works.
    // https://docs.microsoft.com/en-us/dotnet/standard/native-interop/type-marshalling
}

