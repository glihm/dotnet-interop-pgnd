using System;
using System.Runtime.InteropServices;

namespace LibSharp;

public class Sums
{
    [UnmanagedCallersOnly(EntryPoint = "add")]
    public static int Add(int a, int b)
    {
#if CALLCPP
        // Try to include into AOT compiled IL, a call to unmanaged code.
        // In fact, as NativeCpp.add is using DllImport on the native library, the dll
        // of this native library MUST be copied to the output directory.
        Console.WriteLine("FROM C++");
        return NativeCpp.add(a, b);
#else
        // Normal way, no dependency on sum.dll of libnative.
        return a + b;
#endif
    }

    [UnmanagedCallersOnly(EntryPoint = "sumstring")]
    public static IntPtr sumstring(IntPtr first, IntPtr second)
    {
        // Parse strings from the passed pointers 
        string? my1String = Marshal.PtrToStringAnsi(first);
        string? my2String = Marshal.PtrToStringAnsi(second);

        // Concatenate strings 
        string sum = my1String + my2String;

        // Assign pointer of the concatenated string to sumPointer
        IntPtr sumPointer = Marshal.StringToHGlobalAnsi(sum);

        // Return pointer
        return sumPointer;
    }
}
