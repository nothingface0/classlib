#include "test/Test.h" 
#include "classlib/utils/UUID.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <sstream>

using namespace lat;
typedef lat::UUID TestUUID;

Time UUIDTime(int64_t timeval)
{
   timeval -= TestUUID::UNIX_TIME_OFFSET;
   if (timeval < 0)
	return 0;
   else if (timeval > TimeConst::TIME_MAX / 100)
        return Time(TimeConst::TIME_MAX);
   else
	return Time(timeval * 100);
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    int64_t time;
    const char text[] = "bdd8d9b5-c591-4673-a6d0-f418f3f0b301";
    TestUUID t (text);
    time = t.time();
    std::cout << "value = " << text << "\n";
    std::cout << "uuid = " << t << "\n";
    std::cout << "time = " << time
		   << " (UTC " << UUIDTime(time).format (false, "%Y-%m-%d %H:%M:%S.")
		   << UUIDTime(time).nanoformat() << ")\n";
    std::cout << "type = " << t.type () << "\n";
    std::cout << "variant = " << t.variant () << "\n";

    t = TestUUID::generateTime();
    time = t.time();
    std::cout << "uuid = " << t << "\n";
    std::cout << "time = " << time
		   << " (UTC " << UUIDTime(time).format (false, "%Y-%m-%d %H:%M:%S.")
		   << UUIDTime(time).nanoformat() << ")\n";
    std::cout << "type = " << t.type () << "\n";
    std::cout << "variant = " << t.variant () << "\n";

    t = TestUUID::generate();
    time = t.time();
    std::cout << "uuid = " << t << "\n";
    std::cout << "time = " << time
		   << " (UTC " << UUIDTime(time).format (false, "%Y-%m-%d %H:%M:%S.")
		   << UUIDTime(time).nanoformat() << ")\n";
    std::cout << "type = " << t.type () << "\n";
    std::cout << "variant = " << t.variant () << "\n";
    return 0;
}
