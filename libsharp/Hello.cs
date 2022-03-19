using System;
using System.Runtime.InteropServices;

namespace LibSharp;

public static class Hello
{
    private static int s_CallCount = 1;

    [StructLayout(LayoutKind.Sequential)]
    public struct LibArgs
    {
        public IntPtr Message;
        public int Number;
    }

    ///
    ///
    ///
    public static int Hi(IntPtr arg, int argLength)
    {
        if (argLength < Marshal.SizeOf(typeof(LibArgs)))
        {
            return 1;
        }

        LibArgs libArgs = Marshal.PtrToStructure<LibArgs>(arg);
        Console.WriteLine($"Hi! from {nameof(Hi)} [count: {s_CallCount++}]");
        PrintLibArgs(libArgs);
        return 0;
    }


    // Examples to pass some arguments that are non-default signature.
    // public delegate void CustomEntryPointDelegate(LibArgs libArgs);
    // public static void CustomEntryPoint(LibArgs libArgs)
    // {
    //     Console.WriteLine($"Hello, world! from {nameof(CustomEntryPoint)} in {nameof(Lib)}");
    //     PrintLibArgs(libArgs);
    // }

    ///
    ///
    ///
    private static void PrintLibArgs(LibArgs libArgs)
    {
        // Windows works with UNICODE (I didn't know that before).
        string? message = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
            ? Marshal.PtrToStringUni(libArgs.Message)
            : Marshal.PtrToStringUTF8(libArgs.Message);

        Console.WriteLine($"-- message: {message}");
        Console.WriteLine($"-- number: {libArgs.Number}");
    }

}
