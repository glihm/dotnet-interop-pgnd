#ifndef _H_DOTNET_HOST_
#define _H_DOTNET_HOST_

// Standard headers.
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

// Dotnet headers.
// Provided by the AppHost NuGet package and installed as an SDK pack.
#include <nethost.h>

#include <coreclr_delegates.h>
#include <hostfxr.h>

// Platform headers.
#ifdef WINDOWS

#include <Windows.h>
#define COUT std::wcout

#else

#include <dlfcn.h>
#include <limits.h>
#define COUT std::cout
#define MAX_PATH PATH_MAX

#endif


/*
 * Custom .NET host to control .NET runtime from native code.
 */
class Dotnet_host
{

public:	

    /*
     * Loads hostfxr library.
     *
     * Hostfxr is the library that handles the runtime initialization
     * and execution.
     * The API proposes some function pointers to be retrieved to then
     * control the runtime.
     *
     * As this library is located into a .NET SDK, the loaded hostfxr
     * library's path defined which SDK is used.
     * FIXME: test if that's true, but it should.
     */
    bool hostfxr_load_from_path(const char_t *hostfxr_path);

    /*
     * Loads hostfxr through nethost library using `get_hostfxr_path`
     * which auto-located the installed SDK.
     */
    bool hostfxr_load_from_nethost(void);

    /*
     * Loads dotnet runtime from runtime configuration file.
     */
    bool runtime_load(const char_t *config_path);

    /*
     * Gets managed method entry point to be callable from native.
     */
    component_entry_point_fn managed_method_load(const char_t *lib_file,
						 const char_t *dotnet_type,
						 const char_t *dotnet_type_method);

    /*
     * Initializes a new instance.
     */
    Dotnet_host();

private:
    //
    load_assembly_and_get_function_pointer_fn runtime_managed_load;
    //
    hostfxr_initialize_for_runtime_config_fn hostfxr_init_cfg;
    //
    hostfxr_get_runtime_delegate_fn hostfxr_get_runtime_delegate;
    //
    hostfxr_close_fn hostfxr_close;

    /*
     * Loads a dynamic library from it's path, OS api.
     */
    void *library_load(const char_t *path);

    /*
     * Gets a symbol's address in memory from the given library handle, OS api.
     */
    void *symbol_get_addr(void *handle, const char *name);
};


#endif
