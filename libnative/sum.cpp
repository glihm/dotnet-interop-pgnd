#if defined(WINDOWS)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__ ((visibility ("default")))
#endif

// .h
extern "C"
{
    EXPORT int add(int a, int b);
}

// .c
/*
 * Super bad sum.
 */
int add(int a, int b)
{
	return a + b;
}
