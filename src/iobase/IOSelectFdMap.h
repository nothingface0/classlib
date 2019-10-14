#ifndef CLASSLIB_IOSELECT_FD_MAP_H
# define CLASSLIB_IOSELECT_FD_MAP_H
# include "IOSelectMethod.h"
# ifndef _WIN32

namespace lat {

/** #IOSelectMethod that provides mapping from file descriptors to
    client request index.  */
class IOSelectFdMap : public IOSelectMethod
{
protected:
    size_t		fds (void);
    void		fdmap (IOFD fd, size_t index);
    size_t		fd2req (IOFD fd);
    size_t		fdclear (IOFD fd);
    void		compact (void);

private:
    //< Mapping table from file descriptor to requests.
    std::vector<size_t> m_fd2req;
};

inline size_t
IOSelectFdMap::fds (void)
{ return m_fd2req.size (); }

inline size_t
IOSelectFdMap::fd2req (IOFD fd)
{
    ASSERT (fd >= 0);
    ASSERT ((size_t) fd < m_fd2req.size ());
    return m_fd2req [fd];
}

} // namespace lat
# endif // !_WIN32
#endif // CLASSLIB_IOSELECT_FD_MAP_H
