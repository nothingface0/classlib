#ifndef CLASSLIB_ENVIRONMENT_H
# define CLASSLIB_ENVIRONMENT_H
# include "classlib/sysapi/system.h"
# include <string>

namespace lat {

/** Utilities for handling environments. */
class Environment
{
public:
    virtual ~Environment (void);

    virtual bool		has (const std::string &name) const;
    virtual std::string		get (const std::string &name,
		    		     bool *have = 0) const = 0;
    bool			get (const std::string &name,
		    		     std::string &value) const;
    virtual void		set (const std::string &name,
				     const std::string &value) = 0;
    virtual void		unset (const std::string &name) = 0;
};

} // namespace lat
#endif // CLASSLIB_ENVIRONMENT_H
