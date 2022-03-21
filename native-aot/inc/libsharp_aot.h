/*
 * libsharp-aot interface from C.
 */

#ifndef _H_LIBSHARP_AOT_
#define _H_LIBSHARP_AOT_

// Typedefs to have our functions into a structure.
typedef int (*sum_fn)(int, int);
typedef char * (*concat_fn)(char*, char*);

// Actual structure to load library functions.
typedef struct libsharp_aot {
    sum_fn sum;
    concat_fn concat;
} libsharp_aot_t;

/*
 * Initializes the library loading the function pointers
 * from the C# assembly compiled into native code.
 * Doesn't allocate ctx, it must be provided.
 *
 * Returns 0 on success, 1 otherwise.
 */
int
libsharp_aot_init(const char *lib_path, libsharp_aot_t *const ctx);

#endif

