#ifndef CLASSLIB_SYSAPI_SYSTEM_INFO_H
# define CLASSLIB_SYSAPI_SYSTEM_INFO_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/param.h>
#  include <sys/utsname.h>
#  include <linux/sysctl.h>
#  if __APPLE__
#    include <sys/vmmeter.h>
#  elif BSD > 0
#    include <vm/vm_param.h>
#  endif
#  if __sun || __aix
#    include <sys/processor.h>
#  endif
#  include <cstdio>
# endif
#endif // CLASSLIB_SYSAPI_SYSTEM_INFO_H
