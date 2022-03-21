#include <libsharp_aot.h>
#include <dl.h>

/*
 *
 */
int
libsharp_aot_init(const char *lib_path, libsharp_aot_t *const ctx)
{
    HANDLE lib = dl_load_from_path(lib_path);
    if (lib == NULL || ctx == NULL)
    {
	return -1;
    }

    ctx->sum = (sum_fn)dl_load_symbol(lib, "add");
    ctx->concat = (concat_fn)dl_load_symbol(lib, "sum_string");

    if (ctx->sum == NULL || ctx->concat == NULL)
    {
	return -2;
    }

    return 0;
}
