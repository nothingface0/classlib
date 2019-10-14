
#include "src/iobase/IOSelectFdMap.h"
#ifndef _WIN32

namespace lat {

void
IOSelectFdMap::fdmap (IOFD fd, size_t index)
{
    ASSERT (fd >= 0);
    if ((size_t) fd >= m_fd2req.capacity())
	m_fd2req.reserve(fd+1);

    if ((size_t) fd >= m_fd2req.size ())
    {
	size_t growth = fd + 1 - m_fd2req.size();
	m_fd2req.insert (m_fd2req.end(), growth, ~0lu);
    }

    // We require the descriptor to be unused, i.e. not to refer to
    // request index.  We cannot support client which re-subscribe
    // themselves before unsubscribing.  The derived class is allowed
    // to look at only the registered descriptors without counting
    // duplicates.  If a descriptor is added twice, the second writes
    // over the first one, and unsubscribing the first then loses the
    // second.  The only solution that can be guaranteed to be correct
    // is to either not re-subscribe or always unsubscribe first, and
    // never to subscribe any descriptor twice.
    ASSERT (m_fd2req [fd] == ~0lu);
    m_fd2req [fd] = index;
}

size_t
IOSelectFdMap::fdclear (IOFD fd)
{
    // Mark the descriptor unused
    ASSERT (fd >= 0);
    ASSERT ((size_t) fd < m_fd2req.size ());

    size_t index = m_fd2req [fd];

    m_fd2req [fd] = ~0lu;

    // This is always used in combination to removeReq, which causes
    // indices greater than those of this file descriptor to shrink.
    // So update all indices larger than the one we are removing.
    // Remember which fd was the last actually used one.
    for (size_t i = 0, e = m_fd2req.size (); i < e; ++i)
	if (m_fd2req [i] != ~0ul && m_fd2req [i] > index)
	    --m_fd2req [i];

    // Return original index for removing client request.
    return index;
}

void
IOSelectFdMap::compact (void)
{
    m_fd2req.resize(0);
}

} // namespace lat
#endif // !_WIN32
