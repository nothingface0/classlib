#include "test/Test.h" 
#include "classlib/utils/UUID.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <sstream>

using namespace lat;
typedef lat::UUID TestUUID;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    TestUUID uuid = TestUUID::generateTime();
    std::ostringstream str; str << uuid;
    CLASSLIB_TEST_ASSERT_EQUAL(TestUUID(str.str()), uuid);
    CLASSLIB_TEST_ASSERT(Time::current().ns() / 100 - uuid.time() < TimeConst::SEC_NSECS/100);
    return 0;
}
