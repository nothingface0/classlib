#include "test/Test.h" 
#include "classlib/iobase/InetSocket.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    struct H { const char *host; int port; } HOSTS[] = {
	{ "", 80 },
	{ "::", 80 },
	{ "0.0.0.0", 80 },
	{ "::1", 80 },
	{ "www.cern.ch", 80 },
	{ 0, 0 }
    };

    int errors = 0;
    for (H *h = HOSTS; h->host; ++h)
    {
        InetAddress addr(h->host, h->port);
        std::cout << "[" << h->host << "#" << h->port << "] -> ["
	          << addr.hostname() << "#" << addr.service() << "] = ["
		  << addr.hostname(NI_NUMERICHOST) << "#" << addr.port() << "] = ["
		  << addr.hostname(NI_NUMERICHOST) << "#" << addr.service(NI_NUMERICSERV) << "]\n";

        try
        {
	    InetSocket sock(addr);
	    sock.write("GET / HTTP/1.1\nHost: www.cern.ch\n\n", 34);
	    sock.shutdown(SocketConst::CloseDisallowSend);

	    char buf[1024];
	    IOSize n;
	    std::cout << "-----------\n";
	    while ((n = sock.read(buf, sizeof(buf))) > 0)
		std::cout.write(buf, n);
	    std::cout << "\n-----------\n";
        }
        catch (Error &e)
        {
	    std::cout << "ERROR: " << e.explain() << std::endl;
	    errors++;
        }
    }

    return errors;
}
