#include "test/Test.h" 
#include "classlib/utils/IntBits.h"
#include "classlib/utils/Signal.h"
#include <typeinfo>
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "8 :"
	      << " min=" << INT8_MIN
	      << " max=" << INT8_MAX
	      << " umax=" << UINT8_MAX
	      << "\n";
	
    std::cout << "16 :"
	      << " min=" << INT16_MIN
	      << " max=" << INT16_MAX
	      << " umax=" << UINT16_MAX
	      << "\n";
	
    std::cout << "32 :"
	      << " min=" << INT32_MIN
	      << " max=" << INT32_MAX
	      << " umax=" << UINT32_MAX
	      << "\n";
	
    std::cout << "64 :"
	      << " min=" << INT64_MIN
	      << " max=" << INT64_MAX
	      << " umax=" << UINT64_MAX
	      << "\n";

    return 0;
}

