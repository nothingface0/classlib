
#include "classlib/utils/ProcessInfo.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/ProcessInfo.h"
#include "classlib/sysapi/Windows.h"
#include <cstring>

// GLIBC 2.5+ dynamic linker offers argc, argv and envp pointers to the
// module constructors in .init and .init_array sections on ELF platforms.
// On those systems add a suitable constructor to grab the values.  On
// older GLIBC use the private _dl_argv to grab the parameter list.
#if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 5)
# define HAVE_GLIBC_DYNINIT 1
static int linux_argc;
static char **linux_argv;
static char **linux_envp;
static void linux_init(int argc, char **argv, char **envp)
{
    linux_argc = argc;
    linux_argv = argv;
    linux_envp = envp;
}
static void (*const dyninit[]) (int argc, char **argv, char **envp)
__attribute__ ((section (".init_array"), aligned (sizeof (void *))))
__attribute_used__ = { linux_init };
#elif __GLIBC__
# define HAVE_GLIBC_DL_ARGV 1
extern char **_dl_argv;
#endif // GLIBC 

namespace lat {

/** Get program's @c argv vector if available.  */
char **
ProcessInfo::argv (void)
{
#ifdef _WIN32
    return __argv;
#elif HAVE_GLIBC_DYNINIT
    return linux_argv;
#elif HAVE_GLIBC_DL_ARGV
    return _dl_argv;
#elif __APPLE__
    return *_NSGetArgv ();
#else
    static char *s_fakeargv [1] = { 0 };
    return s_fakeargv;
#endif
}

/** Get program's @c argc if available. */
int
ProcessInfo::argc (void)
{
#ifdef _WIN32
    return __argc;
#elif HAVE_GLIBC_DYNINIT
    return linux_argc;
#elif __APPLE__
    return *_NSGetArgc ();
#else
    int nargs = 0;
    for (char **args = argv (); args && *args; ++args)
	++nargs;

    return nargs;
#endif
}

/** Get program's full name if possible, otherwise best available name. */
std::string
ProcessInfo::fullProgramName (void)
{
#ifdef _WIN32
    std::string	result;
    size_t	size = 256;
    char	*buffer = 0;
    DWORD	len;

    do
    {
	delete [] buffer;
	buffer = new char [size *= 2];
	memset (buffer, 0, size);

	len = GetModuleFileName (0, buffer, size);
    } while (len >= size);

    if (len > 0)
	result = buffer;

    delete [] buffer;
    return result;
#elif __linux
    std::string	result;
    int		size = 256;
    char	*buffer = 0;
    int		len;

    do
    {
	delete [] buffer;
	buffer = new char [size *= 2];
	memset (buffer, 0, size);
	len = readlink ("/proc/self/exe", buffer, size);
    } while (len >= size);

    if (len > 0)
	result = buffer;

    delete [] buffer;
    return result;
#elif __APPLE__
    std::string result;
    uint32_t size = 0;
    _NSGetExecutablePath (0, &size);
    result.resize (size);
    if (_NSGetExecutablePath (&result [0], &size) == 0)
    {
	result.resize (size-1);
	return result;
    }

    char **args = argv ();
    return args && *args ? *args : "";
#else
    char **args = argv ();
    return args && *args ? *args : "";
#endif
}

/** Get program's bare name if possible. */
std::string
ProcessInfo::shortProgramName (void)
{ return Filename (fullProgramName ()).nondirectory ().name (); }

/** Get the process id.  */
pid_t
ProcessInfo::pid (void)
{
#ifdef _WIN32
    return GetCurrentProcessId ();
#else
    return ::getpid ();
#endif
}

/** Get the parent process id. */
pid_t
ProcessInfo::ppid (void)
{
#ifdef _WIN32
    PROCESS_BASIC_INFORMATION pbi;
    if (NtQueryInformationProcess (GetCurrentProcess(),ProcessBasicInformation,
				   &pbi, sizeof (pbi), 0) == STATUS_SUCCESS)
	return pbi.InheritedFromUniqueProcessId;

    // FIXME: throw systemerror!
    ASSERT (false);
    return -1;
#else
    return ::getppid ();
#endif
}

} // namespace lat
