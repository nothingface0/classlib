#ifndef CLASSLIB_ARGZ_H
# define CLASSLIB_ARGZ_H
# include "classlib/sysapi/system.h"
# include <string>
# include <vector>

namespace lat {

class Argz
{
public:
    Argz (void);
    Argz (const std::string &command);
    Argz (const char **argz);
    Argz (const char *a1, const char *a2);
    Argz (const char *a1, const char *a2, const char *a3);
    Argz (const char *a1, const char *a2, const char *a3, const char *a4);
    Argz (const Argz &x);
    Argz &operator= (const Argz &x);
    ~Argz (void);

    Argz &		clear (void);
    Argz &		add (const char *arg);
    Argz &		add (const char **argz);
    Argz &		add (const Argz &x);
    const char **	argz (void) const;
    std::string		quote (void) const;

private:
    void		reargz (size_t n);
    void		build (const char **argz);
    void		build (const std::string &command);

    std::string		m_data;
    std::vector<char *>	m_argz;
};

} // namespace lat
#endif // CLASSLIB_ARGZ_H
