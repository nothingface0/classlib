#ifndef CLASSLIB_SYSAPI_ELF_ABI_H
# define CLASSLIB_SYSAPI_ELF_ABI_H
# include "classlib/sysapi/system.h"
# ifndef _WIN32
#  include <limits.h>
#  include <sys/stat.h>
#  include <unistd.h>
#  if __ELF__
#   include <elf.h>
#   include <link.h>
#  endif
#  if __osf
#   include <loader.h>
#  endif
#  if __sgi
#   include <sgidefs.h>
#   include <objlist.h>
#   include <obj_list.h>
#   include <obj.h>
#  endif
# endif // ! _WIN32

#if /* irix */       (defined ABI64 || defined _ABI64 ||		\
 		      (defined _MIPS_SIM && _MIPS_SIM == _MIPS_SIM_ABI64)) \
    /* solaris */ || (defined sparcv9 || defined _sparcv9		\
		      || defined __sparcv9 || defined __sparcv9__)	\
    /* tru64 */   || (defined arch64 || defined _arch64 ||		\
		      defined __arch64 || defined __arch64__)
# define ELF_ABI 64
#else
# define ELF_ABI 32
#endif
#ifndef ElfW
# define ElfW(type)   ElfW1(Elf,ELF_ABI,type)
# define ElfW1(e,w,t) ElfW2(Elf,w,_##t)
# define ElfW2(e,w,t) e ## w ## t
#endif

#endif // CLASSLIB_SYSAPI_ELF_ABI_H
