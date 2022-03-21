// Standard headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libsharp_aot.h>

/*
 *
 */
int main()
{
    libsharp_aot_t lib;

    #ifdef LINUX
    int rc = libsharp_aot_init("./build/LibSharpAOT.so", &lib);
    #else
    int rc = libsharp_aot_init("./build/LibSharpAOT.dll", &lib);
    #endif
    if (rc)
    {
	printf("Library could not be loaded or symbol not found (%d).\n", rc);
	return -1;
    }

    int sum = lib.sum(2, 8);
    if (sum != 10)
    {
	printf("Sum fails.\n");
	return -1;
    }

    // String is allocated when passed from C#. Must be freed.
    char *sumstring = lib.concat("ok", "ko");
    if (strcmp(sumstring, "okko") != 0)
    {
	printf("Concat fails.\n");
	return -1;
    }
    free(sumstring);

    printf("ok\n");
    return 0;
}
