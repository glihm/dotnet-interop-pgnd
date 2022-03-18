# libnative

Emulates a C/C++ library to be used by C#.
Many reasons to use a C/C++ library in C# like:
	1. performances
	2. large code base that huge to reimplented in pure C#
	3. based on hiring requirements / availability


## Compilation:
### Windows
This example target x64 machines. Once VS is installed with [C++ packs](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170), open a `x64 Native Tools Command Prompt` following this [guide](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170#developer_command_prompt_shortcuts) if you don't know what this is.

Run build.bat to compile as a library (.dll + .lib).
The flag `/D WINDOWS` is important as the code is conditionally pre-processed depending defined macros from compile command line.
[Compiler options](https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-170).

### Linux

