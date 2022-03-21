using System;
using System.Runtime.InteropServices;

namespace LibSharpAOT;

/// <summary>
/// 
/// </summary>
public class Sums
{
    /// <summary>
    ///   Sums a and b.
    /// </summary>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <returns></returns>
    [UnmanagedCallersOnly(EntryPoint = "add")]
    public static int Add(int a, int b)
    {
        return a + b;
    }

    /// <summary>
    ///   Concatenates given strings.
    /// </summary>
    /// <param name="first"></param>
    /// <param name="second"></param>
    /// <returns></returns>
    [UnmanagedCallersOnly(EntryPoint = "sum_string")]
    public static IntPtr SumString(IntPtr first, IntPtr second)
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
