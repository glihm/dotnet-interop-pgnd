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
    ///   to the appication folder when deployed.
    ///   
    ///   Usually, the DLLs are placed into the build directory, that's the easiest
    ///   way to ensure it will be present as the build system must copy it.
    /// </summary>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <returns></returns>
#if DEBUG
    // Runs when used from xUnit interface.
    [DllImport("../../../../libnative/build/sum.dll", EntryPoint = "add")]
#else
    // Runs when compiled from command line and for aot with configuration Release.
    [DllImport("build/sum.dll", EntryPoint = "add")]
#endif
    public static extern Int32 add(Int32 a, Int32 b);


    // TODO: add more functions into the CPP library and import them to illustrate
    // the marshalling and how it works.
    // https://docs.microsoft.com/en-us/dotnet/standard/native-interop/type-marshalling
}

