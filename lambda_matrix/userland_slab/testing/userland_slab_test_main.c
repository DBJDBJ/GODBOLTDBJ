#ifdef _WIN32
#define UL_FORMAT_SPEC "%zu"
#include "generate_dump.h"
#else // ! _WIN32
#define UL_FORMAT_SPEC "%lu"
#endif // ! _WIN32
/*

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// #include "queue.h"
#define USERLAND_SLAB_IMPLEMENTED_HERE
#include "../src/slab_implementation.h"

#define N 1000000

static int run(const int argc, char **argv)
{

  /*You can compare the memory consumed by this program.
    <program> <alloc_type> <size>
    <alloc_type> = 1 - malloc based allocation
    <alloc_type> = 2 - slab based allocation
    <size> = size in bytes of the objects to allocate
  */

  if (argc < 3)
  {
    printf("Arguments expected !\n"
           "program <alloc_type> <size>\n"
           "<alloc_type> = 1 - malloc based allocation\n"
           "<alloc_type> = 2 - slab based allocation\n"
           "<size> = size in bytes of the objects to allocate\n");
    return 0;
  }

  void *array[N] = {0}; // clang actually zeroes this array
  size_t obj_size = strtol(argv[2], NULL, 10);

  if (!obj_size)
  {
    printf("The second parameter should be an unsigned integer (size in bytes of object).\n");
    return -1;
  }

  if (argv[1][0] == '1')
  {
    printf("Allocation of %d objects of size " UL_FORMAT_SPEC "  with malloc()\n", N, obj_size);

    // dbj removed
    // if (array == NULL){
    //   printf("Allocation failed\n");
    //   return -1;
    // }

    for (int i = 0; i < N; i++)
    {
      array[i] = malloc(obj_size);
      // dbj added
      assert(array[i] != 0);
    }

    getchar();
  }
  else
  {
    printf("Allocation of %d objects of size " UL_FORMAT_SPEC "  with the slab allocator\n", N, obj_size);

    struct Objs_cache cache;

    // we initialise the slab allocator once and for all
    if (!slab_allocator_init())
    {
      printf("Error : slab allocator initialisation failed !\n");
      exit(-1);
    }

    // we initialise a cache to allocate objects of obj_size

    if (!objs_cache_init(&cache, obj_size, NULL))
    {
      printf("Error : cache initialisation failed !\n");
      exit(-1);
    }

    for (int i = 0; i < N; i++)
    {
      array[i] = objs_cache_alloc(&cache);
      if (!array[i])
      {
        printf("Failed to allocate an object from a cache !\n");
        exit(-1);
      }
    }

    getchar();

    // We do something with the allocated objects
    //  ...

    objs_cache_destroy(&cache);

    slab_allocator_destroy();
  }

  return 42;
}

// dbj added win32 SEH to be able to catch stack overflow
// and everything else _WIN32 does offer to be caught
// NOTE: this is superior to the mess of C++ exceptions

static inline int filterException(int code, PEXCEPTION_POINTERS ex)
{
  /*
  https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/596a1078-e883-4972-9bbc-49e60bebca55
  You can look up exception code in ntstatus.h
  0xC00000FD is STATUS_STACK_OVERFLOW
  although if you have a minidump you open it in VStudio and all the info is nicely laid out there
  */
  printf("\n\nSEH code: %x", code);
  return EXCEPTION_EXECUTE_HANDLER;
}

#ifdef _WIN32
int main(const int argc, char **argv)
{
  __try
  {
    run(argc, argv);
  }
  // __except (filterException(GetExceptionCode(), GetExceptionInformation()))
  // {
  //   printf(" , caught\n\n");
  // }
  __except (
      GenerateDump(GetExceptionInformation())
      /* returns 1 aka EXCEPTION_EXECUTE_HANDLER */
  )
  {
    // MS STL "throws" are raised SE's under "no exceptions" builds
    // they are best caught here and nowhere else in the app
    printf("\n: Structured Exception caught");

    if (dump_last_run.finished_ok)
    {
      printf("\n: Minidump creation succeeded");
      printf("\n: Dump folder: %s", dump_last_run.dump_folder_name);
      printf("\n: Dump file  : %s", dump_last_run.dump_file_name);
    }
    else
    {
      printf("\nMinidump creation failed");
    }
    return 0;
  }
}
#else  // linux, probably
int main(const int argc, char **argv)
{
  run(argc, argv);
  return 0;
}
#endif // linux, probably
