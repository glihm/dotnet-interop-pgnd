#include <Dotnet_host.h>

/*
 *
 */
Dotnet_host::Dotnet_host()
{

}

/*
 *
 */
bool
Dotnet_host::hostfxr_load_from_nethost(void)
{
    return false;
    // char_t buffer[MAX_PATH];
    // size_t buffer_size = sizeof(buffer) / sizeof(char_t);

    // // Exposed by nethost.
    // int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
    // if (rc != 0)
    // {
    // 	return false;
    // }

    // return hostfxr_load_from_path(buffer);
}

/*
 *
 */
bool
Dotnet_host::hostfxr_load_from_path(const char_t *hostfxr_path)
{
    COUT << "loading hostfxr from: " << hostfxr_path << std::endl;

    // Load hostfxr and get functions to be used afterwards during assembly loading.
    void *lib = library_load(hostfxr_path);
    if (lib == nullptr)
    {
	return false;
    }

    hostfxr_init_cfg = (hostfxr_initialize_for_runtime_config_fn)symbol_get_addr(
	lib,
	"hostfxr_initialize_for_runtime_config");

    hostfxr_get_runtime_delegate = (hostfxr_get_runtime_delegate_fn)symbol_get_addr(
	lib,
	"hostfxr_get_runtime_delegate");

    hostfxr_close = (hostfxr_close_fn)symbol_get_addr(
	lib,
	"hostfxr_close");

    bool loaded =
	hostfxr_init_cfg != nullptr
	&& hostfxr_get_runtime_delegate != nullptr
	&& hostfxr_close != nullptr;

    return loaded;
}

/*
 *
 */
void
*Dotnet_host::library_load(const char_t *path)
{
#ifdef WINDOWS
    HMODULE h = ::LoadLibraryW(path);
#else
    void *h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
#endif
    return (void*)h;
}

/*
 *
 */
void
*Dotnet_host::symbol_get_addr(void *handle, const char *name)
{
#ifdef WINDOWS
    void *addr = ::GetProcAddress((HMODULE)handle, name);
#else
    void *addr = dlsym(handle, name);
#endif
    return addr;    
}

/*
 *
 */
bool
Dotnet_host::runtime_load(const char_t *config_path)
{
    // Load .NET runtime
    hostfxr_handle cxt = nullptr;
    int rc = hostfxr_init_cfg(config_path, nullptr, &cxt);
    if (rc != 0 || cxt == nullptr)
    {
	std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
	hostfxr_close(cxt);
	return false;
    }

    void *fptr = nullptr;
    rc = hostfxr_get_runtime_delegate(
	cxt,
	hdt_load_assembly_and_get_function_pointer,
	&fptr);
    if (rc || fptr == nullptr)
    {
	std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;
	hostfxr_close(cxt);
	return false;
    }

    hostfxr_close(cxt);

    runtime_managed_load = (load_assembly_and_get_function_pointer_fn)fptr;

    return runtime_managed_load != nullptr;
}

/*
 *
 */
component_entry_point_fn
Dotnet_host::managed_method_load(const char_t *lib_file,
				 const char_t *dotnet_type,
				 const char_t *dotnet_type_method)
{
    component_entry_point_fn method = nullptr;

    int rc = runtime_managed_load(
	lib_file,
	dotnet_type,
	dotnet_type_method,
	nullptr /*delegate_type_name*/,
	nullptr,
	(void**)&method);

    return rc ? nullptr : method;
}
