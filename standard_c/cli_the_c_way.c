#include <stdio.h>

static char* shift_args(int* argc, char*** argv)
{
	return (*argc)--, *(*argv)++;
}

int main(int argc, char** argv)
{
	for (int i = 0; argc; i++)
	{
		printf("%d: %16s\n", i, shift_args(&argc, &argv));
	}
}
