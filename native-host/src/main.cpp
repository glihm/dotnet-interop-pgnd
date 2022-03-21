#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <filesystem>

#include <Dotnet_host.h>

namespace stdfs = std::filesystem;
using string_t = std::basic_string<char_t>;

// Platform imports.
#ifdef WINDOWS
std::filesystem::path * resolve_absolute_path(wchar_t *name);

#define STR(s) L##s
#define COUT std::wcout

#else

std::filesystem::path * resolve_absolute_path(char *name);

#define STR(s) s
#define MAX_PATH PATH_MAX
#define COUT std::cout

#endif


// Defines the structure that is marshalled in managed code.
struct lib_args
{
    const char_t *message;
    int number;
};

/*
 * Entry point.
 */
#ifdef WINDOWS
    int __cdecl wmain(int argc, wchar_t *argv[])
#else
    int main(int argc, char *argv[])
#endif
{
    // Remove warning.
    (void)argc;

    // Get path of the current program to load files with relative paths.
    stdfs::path *prog_path = resolve_absolute_path(argv[0]);

    // Relative files locations.
    stdfs::path config_file = prog_path->parent_path() / "build" / "LibSharpHost.runtimeconfig.json";
    #ifdef WINDOWS
    stdfs::path lib_dll = prog_path->parent_path() / "build" / "LibSharpHost.dll";
    #else
    stdfs::path lib_dll = prog_path->parent_path() / "build" / "LibSharpHost.so";
    #endif

    Dotnet_host *dotnet = new Dotnet_host;

    // Step 1: load hostfxr.
    // OPTION 1:
    // Nethost automatically searches for the installed hostfxr.dll.
    //bool loaded = dotnet->hostfxr_load_from_nethost();

    // OPTION 2:
    // Custom absolute path to the SDK.
    stdfs::path hostfxr_dll = "/home/glihm/Downloads/host/fxr/6.0.3/libhostfxr.so";
    bool loaded = dotnet->hostfxr_load_from_path(hostfxr_dll.c_str());
    if (!loaded)
    {
	COUT << "Couldn't load hostfxr" << std::endl;
	return EXIT_FAILURE;
    }

    // Step 2: load and configure the runtime.
    loaded = dotnet->runtime_load(config_file.c_str());
    if (!loaded)
    {
	COUT << "Couldn't load runtime configuration " << config_file.c_str() << std::endl;
	return EXIT_FAILURE;
    }

    // Step 3: load the managed static method.
    component_entry_point_fn dotnet_hi = dotnet->managed_method_load(
        lib_dll.c_str(),
	STR("LibSharpHost.Greeting, LibSharpHost"),
	STR("Hi"));

    if (dotnet_hi == nullptr)
    {
	COUT << "Can't load managed method" << std::endl;
	return EXIT_FAILURE;
    }

    // Step 4: Run managed code.
    for (int i = 0; i < 3; ++i)
    {
	lib_args args {
	    STR("called from host!"),
	    i
	};

	int r = dotnet_hi(&args, sizeof(args));
	COUT << "return value = " << r << std::endl;
    }

    return EXIT_SUCCESS;
}

/*
 * Resolves and gets the full path of a given executable/program name.
 */
#ifdef WINDOWS
    stdfs::path *
    resolve_absolute_path(wchar_t *name)
#else
    stdfs::path *
    resolve_absolute_path(char *name)
#endif
{
    char_t host_path[MAX_PATH];
    
#ifdef WINDOWS
    auto size = ::GetFullPathNameW(
	name,
	sizeof(host_path) / sizeof(char_t),
	host_path,
	nullptr);

    assert(size != 0);
#else
    auto resolved = realpath(name, host_path);
    assert(resolved != nullptr);
#endif

    return new stdfs::path(host_path);
}
