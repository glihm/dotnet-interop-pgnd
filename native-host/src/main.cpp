#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>

#include <Hostfxr_ctx.h>

using string_t = std::basic_string<char_t>;

#ifdef WINDOWS

string_t resolve_absolute_path(wchar_t *prog_name);
#define STR(s) L ## s
#define CH(c) L ## c
#define DIR_SEPARATOR L'\\'
#define COUT std::wcout

#else

string_t resolve_absolute_path(char *prog_name);
#define STR(s) s
#define CH(c) c
#define DIR_SEPARATOR '/'
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
    // remove warning.

    COUT << "0" << std::endl;
    string_t prog_path = resolve_absolute_path(argv[0]);
    COUT << "Program's absolute path: " << prog_path.c_str() << std::endl;

    COUT << "1" << std::endl;

    Hostfxr_ctx *hfxr = new Hostfxr_ctx;
    assert(hfxr->is_ready && "Hostfxr not loaded");

    COUT << "2" << std::endl;

    // FIXME: faire une fonction libloader -> qui attend juste path to LibSharp,
    // and will search for configuration file just next to this?

    // NOOB -> need to load runtime assembly first with config.............!
    // this file is the .deps.json, generated when compiled.
    // same here, use WINDOWS to know the separator.
    string_t config_file = prog_path + STR("build\\LibSharp.runtimeconfig.json");
    COUT << "config file " << config_file << std::endl;

    bool runtime_loaded = hfxr->dotnet_assembly_load(config_file.c_str());
    if (!runtime_loaded)
    {
	COUT << "Couldn't load .net runtime for config " << config_file << std::endl;
	return EXIT_FAILURE;
    }

    // check here to use something better to merge the  paths??
    // use if windows...!
    string_t lib_path = prog_path + STR("build\\LibSharp.dll");
    //COUT << "Lib path" << lib_path.c_str() << std::endl;

    component_entry_point_fn hi = hfxr->dotnet_managed_load(
        lib_path.c_str(),
	STR("LibSharp.Hello, LibSharp"),
	STR("Hi"));

    if (hi == nullptr)
    {
	COUT << "Can't load managed symbol" << std::endl;
	return EXIT_FAILURE;
    }

    COUT << "4" << std::endl;
    // Run managed code.
    for (int i = 0; i < 3; ++i)
    {
	lib_args args {
	    STR("called from host!"),
	    i
	};

	int r = hi(&args, sizeof(args));
	COUT << "return value = " << r << std::endl;
    }

    return EXIT_SUCCESS;
}

/*
 * Resolves and gets the full path of a given executable/program name.
 */
#ifdef WINDOWS
    string_t resolve_absolute_path(wchar_t *name)
#else
    string_t resolve_absolute_path(char *name)
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

    string_t root_path = host_path;
    auto pos = root_path.find_last_of(DIR_SEPARATOR);
    assert(pos != string_t::npos);
    root_path = root_path.substr(0, pos + 1);

    return root_path;
}
