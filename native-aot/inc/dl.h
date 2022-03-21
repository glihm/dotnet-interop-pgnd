/*
 * Dynamic library handlers.
 */

#ifndef _H_DL_
#define _H_DL_

#ifdef WINDOWS

#include "windows.h"
#define symLoad GetProcAddress
#define HANDLE HINSTANCE

int access(const char *, int);

#else

#include "dlfcn.h"
#include <unistd.h>
#define symLoad dlsym
#define HANDLE void*

#endif

#ifndef F_OK
#define F_OK    0
#endif

/*
 * Loads a dynamic library from given path.
 *
 * Returns NULL if library can't be loaded.
 */
HANDLE
dl_load_from_path(const char *path);

/*
 * Loads a symbol from the given library handle.
 *
 * Returns NULL if the symbol is not found.
 */
void *
dl_load_symbol(HANDLE handle, const char *name);

#endif

