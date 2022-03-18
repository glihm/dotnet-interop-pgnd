using Xunit;
using LibSharp;

namespace testing;

public class CallNativeFromCsharp
{
    /// <summary>
    ///   Tests the C++ native library wrapped in a class
    ///   using explicit P/Invoke (DllImport).
    /// </summary>
    [Fact]
    public void Sum()
    {
        int r = NativeCpp.add(2, 2);
        Assert.Equal(4, r);
    }

    // TODO: add several examples for the Marshalling.
}