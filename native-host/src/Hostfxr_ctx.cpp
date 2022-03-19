#include <Hostfxr_ctx.h>

/*
 *
 */
Hostfxr_ctx::Hostfxr_ctx()
{
    is_ready = hostfxr_load();
}

/*
 *
 */
bool
Hostfxr_ctx::hostfxr_load(void)
{
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);

    // Exposed by nethost.
    int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
    if (rc != 0)
    {
	return false;
    }

    // Load hostfxr and get functions to be used afterwards during assembly loading.
    void *lib = library_load(buffer);
    if (lib == nullptr)
    {
	return false;
    }

    hostfxr_init = (hostfxr_initialize_for_runtime_config_fn)symbol_get_addr(
	lib,
	"hostfxr_initialize_for_runtime_config");

    hostfxr_get_runtime_delegate = (hostfxr_get_runtime_delegate_fn)symbol_get_addr(
	lib,
	"hostfxr_get_runtime_delegate");

    hostfxr_close = (hostfxr_close_fn)symbol_get_addr(
	lib,
	"hostfxr_close");

    bool loaded =
	hostfxr_init != nullptr
	&& hostfxr_get_runtime_delegate != nullptr
	&& hostfxr_close != nullptr;

    std::wcout << "hostfxr LOADED: " << loaded << std::endl;
    return loaded;
}

/*
 *
 */
void
*Hostfxr_ctx::library_load(const char_t *path)
{
#ifdef WINDOWS
    HMODULE h = ::LoadLibraryW(path);
#else
    void *h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
#endif
    assert(h != nullptr);
    return (void*)h;
}

/*
 *
 */
void
*Hostfxr_ctx::symbol_get_addr(void *handle, const char *name)
{
#ifdef WINDOWS
    void *addr = ::GetProcAddress((HMODULE)handle, name);
#else
    void *addr = dlsym(handle, name);
#endif
    assert(addr != nullptr);
    return addr;    
}

/*
 *
 */
bool
Hostfxr_ctx::dotnet_assembly_load(const char_t *config_path)
{
    // Load .NET Core
    hostfxr_handle cxt = nullptr;
    int rc = hostfxr_init(config_path, nullptr, &cxt);
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
Hostfxr_ctx::dotnet_managed_load(const char_t *lib_file,
				 const char_t *dotnet_type,
				 const char_t *dotnet_type_method)
{
    std::wcout << lib_file << std::endl;
    std::wcout << dotnet_type << std::endl;
    std::wcout << dotnet_type_method << std::endl;

    component_entry_point_fn method = nullptr;

    int rc = runtime_managed_load(
	lib_file,
	dotnet_type,
	dotnet_type_method,
	nullptr /*delegate_type_name*/,
	nullptr,
	(void**)&method);

    std::wcout << "HHH! " << (method != nullptr) << std::endl;

    return rc ? nullptr : method;
}
