#define SUV_SUS2

#include <unistd.h>
#include <stdio.h>

int main() {
   printf("Request:\n");

#ifdef _POSIX_SOURCE
   printf("\t_POSIX_SOURCE defined\n");
#else
   printf("\t_POSIX_SOURCE undefined\n");
#endif

#ifdef _XOPEN_SOURCE
   #if _XOPEN_SOURCE +0 == 0
      printf("\t_XOPEN_SOURCE defined (no value)\n");
   #else
      printf("\t_XOPEN_SOURCE = %d\n", _XOPEN_SOURCE);
   #endif
#else
   printf("\t_XOPEN_SOURCE undefined\n");
#endif

#ifdef _XOPEN_SOURCE_EXTENDED
   printf("\t_XOPEN_SOURCE_EXTENDED defined\n");
#else
   printf("\t_XOPEN_SOURCE_EXTENDED undefined\n");
#endif

   printf("Claims:\n");

#ifdef _POSIX_VERSION
   printf("\t_POSIX_VERSION = %ld\n", _POSIX_VERSION);
#else
   printf("\tNot POSIX");
#endif

#ifdef _XOPEN_UNIX
   printf("\tX/Open\n");
   #ifdef _XOPEN_VERSION
       printf("\t_XOPEN_VERSION = %d\n", _XOPEN_VERSION);
   #else
       printf("\tError: _XOPEN_UNIX defined, but not "
	"_XOPEN_VERSION\n");
   #endif
#else
   printf("\tNot X/Open\n");
#endif

   printf("--------------------\n");
   printf("Sysconf:\n");
   
   int i;
   for (i=_SC_ARG_MAX; i <=_SC_THREAD_ROBUST_PRIO_PROTECT; i++)
      printf("\tConf[%d]: %ld\n", i, sysconf(i));

   printf("pathconf:\n");
   for (i=_PC_LINK_MAX; i <=_PC_2_SYMLINKS; i++)
      printf("\tConf[%d]: %ld\n", i, pathconf("/", i));

   printf("confstr:\n");
   for (i=_CS_PATH; i <=_CS_V7_ENV; i++)
      printf("\tConf[%d]: %ld\n", i, confstr(i, NULL, 0));
    

   return 0;
}
