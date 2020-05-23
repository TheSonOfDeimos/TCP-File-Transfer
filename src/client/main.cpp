#include "client.h"

int main(int argc, char *argv[])
{
    // connect several clients
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
    char* messages[] = { "John says hi", "so does James", "Lucy just got home", 0 };
    for ( char ** message = messages; *message; ++message) {
        talk_to_svr::start(ep, *message);
        boost::this_thread::sleep( boost::posix_time::millisec(100));
    }
    service.run();
}