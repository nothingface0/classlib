#ifndef CLASSLIB_UNWIND_H
# define CLASSLIB_UNWIND_H
# include "classlib/sysapi/system.h"

namespace lat {

/*<doc>		Automatic free store deletion.

		<<Unwind>> is a free-store object holder like ISO
		standard <<auto_ptr>>.  It basically exists to fill
		the gaps in C++ compiler implementations and should be
		removed when <<auto_ptr>> is universally available.  */
template <class T>
class Unwind
{
public:
    explicit
    Unwind (T *object = 0);
    Unwind (const Unwind<T> &x);
    ~Unwind (void);
    Unwind<T> &operator= (const Unwind<T> &x);

    T &			operator* (void) const;
    T *			operator-> (void) const;
    T *			get (void) const;
    T *			release (void) const;

private:
    T			*m_object;
    mutable bool	m_owner;
};

template <class T>
inline
Unwind<T>::Unwind (T *object /* = 0 */)
    : m_object (object),
      m_owner (object != 0)
{}

template <class T>
inline
Unwind<T>::Unwind (const Unwind<T> &x)
{
    m_object = x.m_object;
    m_owner = x.m_owner;
    x.m_owner = false;
}

template <class T>
inline
Unwind<T>::~Unwind (void)
{
    if (m_owner)
	delete m_object;
}

template <class T>
inline Unwind<T> &
Unwind<T>::operator= (const Unwind<T> &x)
{
    if (m_owner && m_object != x.m_object)
	delete m_object;

    bool xowner = x.m_owner;
    x.m_owner = false;
    m_object = x.m_object;
    m_owner = xowner;
    return *this;
}

template <class T>
inline T &
Unwind<T>::operator* (void) const
{
    return *m_object;
}

template <class T>
inline T *
Unwind<T>::operator-> (void) const
{
    return m_object;
}

template <class T>
inline T *
Unwind<T>::get (void) const
{
    return m_object;
}

template <class T>
inline T *
Unwind<T>::release (void) const
{
    m_owner = false;
    return m_object;
}

} // namespace lat
#endif // CLASSLIB_UNWIND_H
