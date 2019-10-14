
#include "classlib/utils/ShellEnvironment.h"
#include <cstdlib>

namespace lat {

#ifdef _WIN32
#define putenv putenv_
extern "C" { extern char **environ; }
#endif


bool
ShellEnvironment::has (const std::string &name) const
{ return getenv (name.c_str ()); }

std::string
ShellEnvironment::get (const std::string &name, bool *have /* = 0 */) const
{
    std::string value;
    if (const char *val = getenv (name.c_str ()))
    {
	value = val;
	if (have) *have = true;
    }
    else
    {
	value = "";
	if (have) *have = false;
    }

    return value;
}


void
ShellEnvironment::unset (const std::string &name)
{
#ifndef _WIN32
    unsetenv (name.c_str ());
#else
    char **val;
    char **out;
    for (val = out = environ; *val; ++val)
	if (strncmp (*val, name.c_str (), name.size ())
	    || (*val) [name.size ()] != '=')
	    *out++ = *val;

    *out++ = 0;
#endif
}

void
ShellEnvironment::set (const std::string &name, const std::string &value)
{
#ifndef _WIN32
    setenv (name.c_str (), value.c_str (), true);
#else
    char *val = (char *) malloc (name.size () + 1 + value.size () + 1);
    strcpy (val, name.c_str ());
    strcat (val, "=");
    strcat (val, value.c_str ());
    putenv (val);
#endif
}

} // namespace lat
