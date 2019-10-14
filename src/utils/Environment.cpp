
#include "classlib/utils/Environment.h"
#include <string>

namespace lat {

Environment::~Environment (void)
{}

bool
Environment::has (const std::string &name) const
{
    bool have = false;
    get (name, &have);
    return have;
}

bool
Environment::get (const std::string &name, std::string &value) const
{
    bool have = false;
    value = get (name, &have);
    return have;
}

} // namespace lat
