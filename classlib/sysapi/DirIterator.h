#ifndef CLASSLIB_SYSAPI_DIR_ITERATOR_H
# define CLASSLIB_SYSAPI_DIR_ITERATOR_H
# include "classlib/sysapi/system.h"
# include <cerrno>
# ifdef _WIN32
#  include <windows.h>
# else
#  include <dirent.h>
# endif

namespace lat {

struct DirIteratorData
{
#ifdef _WIN32
    HANDLE		m_dir;
    WIN32_FIND_DATA	m_dirent;
#else
    DIR			*m_dir;
    dirent		*m_dirent;
#endif
    unsigned int	m_refs;
    bool		m_last;
    bool		m_valid;
    bool		m_dots;
    std::string		*m_last_name;
};

} // namespace lat
#endif // CLASSLIB_SYSAPI_DIR_ITERATOR_H
