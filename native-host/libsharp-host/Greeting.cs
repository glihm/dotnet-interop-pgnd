// Adapted from dotnet/runtime code samples.
using System;
using System.Runtime.InteropServices;

namespace LibSharpHost;

/// <summary>
/// 
/// </summary>
public static class Greeting
{
    //
    private static int s_CallCount = 1;

    /// <summary>
    /// 
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct LibArgs
    {
        public IntPtr Message;
        public int Number;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="arg"></param>
    /// <param name="argLength"></param>
    /// <returns></returns>
    public static int Hi(IntPtr arg, int argLength)
    {
        if (argLength < Marshal.SizeOf(typeof(LibArgs)))
        {
            return 1;
        }

        LibArgs libArgs = Marshal.PtrToStructure<LibArgs>(arg);
        PrintLibArgs(libArgs);
        return 0;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="libArgs"></param>
    private static void PrintLibArgs(LibArgs libArgs)
    {
        // Windows works with UNICODE (I didn't know that before).
        string? message = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
            ? Marshal.PtrToStringUni(libArgs.Message)
            : Marshal.PtrToStringUTF8(libArgs.Message);

        Console.WriteLine($"[C# code] --> {message} {libArgs.Number}");
    }
}