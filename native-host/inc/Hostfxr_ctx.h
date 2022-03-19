// Standard headers.
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

// Dotnet headers.
// Provided by the AppHost NuGet package and installed as an SDK pack.
#include <nethost.h>
// Header files copied from https://github.com/dotnet/core-setup.
#include <coreclr_delegates.h>
#include <hostfxr.h>


// Platform headers.
#ifdef WINDOWS

#include <Windows.h>

#else

#include <dlfcn.h>
#include <limits.h>

#endif


/*
 * Handles Hostfxr operations.
 *
 * Some types name are very long, handling them
 * in a separate class will help to the reading.
 */
class Hostfxr_ctx
{

public:	
    // Indicates if the hostfxr was loaded correctly during constructor.
    bool is_ready;

    /*
     * Loads dotnet assembly with the given configuration.
     */
    bool dotnet_assembly_load(const char_t *config_path);

    /*
     * Gets memory address of a managed method based on
     * it's type and library.
     */
    component_entry_point_fn dotnet_managed_load(const char_t *lib_file,
						 const char_t *dotnet_type,
						 const char_t *dotnet_type_method);

    /*
     * Initializes a new instance, loading Hostfxr.
     * `isReady` will reflect the status of Hostfxr loading process.
     */
    Hostfxr_ctx();

private:
    //
    load_assembly_and_get_function_pointer_fn runtime_managed_load;
    //
    hostfxr_initialize_for_runtime_config_fn hostfxr_init;
    //
    hostfxr_get_runtime_delegate_fn hostfxr_get_runtime_delegate;
    //
    hostfxr_close_fn hostfxr_close;

    /*
     * Loads Hostfxr through nethost library.
     */
    bool hostfxr_load(void);

    /*
     * Loads a dynamic library from it's path.
     */
    void *library_load(const char_t *path);

    /*
     * Gets a symbol's address in memory from the given library handle.
     */
    void *symbol_get_addr(void *handle, const char *name);
};
