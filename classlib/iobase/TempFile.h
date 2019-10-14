#ifndef CLASSLIB_TEMP_FILE_H
# define CLASSLIB_TEMP_FILE_H
# include "classlib/iobase/Filename.h"
# include "classlib/iobase/File.h"

namespace lat {

/** Temporary file name handling.  */
class TempFile
{
public:
    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    static Filename	tempdir (void);
    static File *	file (Filename &name);
    static void		dir (Filename &name);

private:
    // Implementation-specific functions
    static bool		tryTempDir (Filename &name, Filename candidate);
};

} // namespace lat
#endif // CLASSLIB_TEMP_FILE_H
