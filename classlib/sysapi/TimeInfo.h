#ifndef CLASSLIB_SYSAPI_TIME_INFO_H
# define CLASSLIB_SYSAPI_TIME_INFO_H
# include "classlib/sysapi/system.h"
# include <cerrno>
# ifdef _WIN32
#  include <windows.h>
#  include <time.h>
# else
#  include <unistd.h>
#  include <fcntl.h>
#  include <linux/sysctl.h>
#  include <sys/resource.h>
#  include <sys/time.h>
#  include <time.h>
#  include <sys/times.h>
#  if __sun
#   include <sys/processor.h>
#   include <procfs.h>
#  endif
#  if __aix
#   include <sys/processor.h>
#   include <pmapi.h>
#  endif
#  if __sgi
#   include <invent.h>
#  endif
#  if __APPLE__
#   include <mach/mach_time.h>
#  endif
#  if __hpux
#   define _PSTAT64
#   include <sys/param.h>
#   include <sys/pstat.h>
//   # include <machine/inline.h>
#  endif
# endif
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

// A gross hack for linux which lies that CLK_TCK is 1000000 when the
// values are really 100.
# if __linux
#  undef CLK_TCK
#  define CLK_TCK 100
# endif

# if __GNUC__
#  if __i386 || __x86_64__
#   define CPU_REAL_CYCLES_ASM		"rdtsc" : "=a" (low), "=d" (high)
#   define CPU_REAL_CYCLES_VALUE	((unsigned long long) low) | ((unsigned long long) high << 32)
#  elif __ia64__
#   define CPU_REAL_CYCLES_ASM		"mov %0=ar.itc" : "=r" (time)
#   define CPU_REAL_CYCLES_VALUE	time
#  elif __alpha__ && (__linux || __osf)
#   define CPU_REAL_CYCLES_ASM		"rpcc %0" : "=r" (time)
#   define CPU_REAL_CYCLES_VALUE	time & 0xffffffff
#   define CPU_VIRTUAL_CYCLES_ASM	"rpcc %0" : "=r" (time)
#   define CPU_VIRTUAL_CYCLES_VALUE	(time + (time << 32)) >> 32
#  elif __hppa__
#   define CPU_REAL_CYCLES_ASM		"mfctl 16,%0" : "=r" (time)
#   define CPU_REAL_CYCLES_VALUE	time
#  elif __mips__
#   define CPU_REAL_CYCLES_ASM		".set push\n\t" \
   					".set reorder\n\t" \
         				"mfc0 %0,$9\n\t" \
	          			".set pop" : "=r" (time)
#   define CPU_REAL_CYCLES_VALUE	time
#  elif __sparc__
#   define CPU_REAL_CYCLES_ASM		"rd %%tick,%0" : "=r" (time)
#   define CPU_REAL_CYCLES_VALUE	time
#  endif
# endif

// Define some helper states to simplify the logic

#if __aix || defined CPU_REAL_CYCLES_ASM || __APPLE__ || defined _WIN32
# define CLASSLIB_HAVE_CPU_REAL_CYCLE_COUNTER 1
#endif

#if (__sun || (_POSIX_TIMERS > 0 && __sgi) || CLASSLIB_HAVE_CPU_REAL_CYCLE_COUNTER)
# define CLASSLIB_HAVE_CPU_REAL_TIME_COUNTER 1
#endif

#if defined CPU_VIRTUAL_CYCLES_ASM
# define CLASSLIB_HAVE_CPU_VIRTUAL_CYCLE_COUNTER 1
#endif
#endif // CLASSLIB_SYSAPI_TIME_INFO_H
