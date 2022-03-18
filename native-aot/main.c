// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.


// Platform specific macros.
#ifdef WINDOWS
#include "windows.h"
#define symLoad GetProcAddress
#define HANDLE HINSTANCE
#else
#include "dlfcn.h"
#include <unistd.h>
#define symLoad dlsym
#define HANDLE void*
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef F_OK
#define F_OK    0
#endif

// .h
HANDLE load_library(const char *path);
int csharp_aot_sum_int(HANDLE lib, char *funcName, int a, int b);
char *csharp_aot_sum_str(HANDLE lib, char *funcName, char *a, char *b);

/*
 * Entry point.
 * To work, the native code that wants to call the C#-natively compiled code,
 * we must know the path of the library (dll on windows for instance).
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
	puts("Error: missing library path.\nusage: ./main.[exe,out] <path/to/the/nativeaot.dll>\n");
	return -1;
    }

    char *lib_path = argv[1];
    printf("using lib path: %s\n", lib_path);

    // Check if the library file exists.
    if (access(lib_path, F_OK) == -1)
    {
        puts("Couldn't find library at the specified path");
        return 0;
    }

    // Get a handle of the library to actually reference it.
    HANDLE lib = load_library(lib_path);

    // Sum two integers
    int sum = csharp_aot_sum_int(lib, "add", 2, 8);
    printf("The sum is %d \n", sum);
    assert(sum == 10);

    // Concatenate two strings
    char *sumstring = csharp_aot_sum_str(lib, "sumstring", "ok", "ko");
    printf("The concatenated string is %s \n", sumstring);
    assert(strcmp(sumstring, "okko") == 0);

    // Free string
    free(sumstring);

    return 0;
}

/*
 * Loads a library from it's dll/so file's path.
 */
HANDLE load_library(const char *path)
{
    #ifdef WINDOWS
        return LoadLibraryA(path);
    #else
        return dlopen(path, RTLD_LAZY);
    #endif
}

/*
 * Calls sum function defined in C# shared library.
 */
int csharp_aot_sum_int(HANDLE handle, char *funcName, int firstInt, int secondInt)
{
    typedef int(*myFunc)(int,int);
    myFunc MyImport = (myFunc)symLoad(handle, funcName);

    int result = MyImport(firstInt, secondInt);

    // CoreRT libraries do not support unloading
    // See https://github.com/dotnet/corert/issues/7887
    return result;
}

/*
 * Calls sum function defined in C# shared library.
 */
char *csharp_aot_sum_str(HANDLE handle, char *funcName, char *firstString, char *secondString)
{
    // Declare a typedef
    typedef char *(*myFunc)(char*,char*);

    // Import Symbol named funcName
    myFunc MyImport = (myFunc)symLoad(handle, funcName);

    // The C# function will return a pointer
    char *result = MyImport(firstString, secondString);

    // CoreRT libraries do not support unloading
    // See https://github.com/dotnet/corert/issues/7887
    return result;
}
