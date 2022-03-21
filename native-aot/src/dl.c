#include <dl.h>

/*
 *
 */
HANDLE
dl_load_from_path(const char *path)
{
    if (access(path, F_OK) == -1)
    {
	return NULL;
    }

#ifdef WINDOWS
    return LoadLibraryA(path);
#else
    return dlopen(path, RTLD_LAZY);
#endif
}

/*
 *
 */
void *
dl_load_symbol(HANDLE handle, const char *name)
{
    return (void *)symLoad(handle, name);
}

