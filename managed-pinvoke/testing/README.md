Testing project to run libsharp unit testing.

Playground to test Marshalling.

Before running the test, you need to compile `libnative`.
(`cd ../libnative`, then `build.[sh/bat]`).

If you run the test from:

1. Command line: then use ` dotnet test /p:DefineConstants=LINUX` if you are on linux.
   On windows, remove the `/p:...`.
   
2. VS: then define the build constants from the project's properties interface.
