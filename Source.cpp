#include "server.h"
#include "client.h"
#include <future>

void server() {
    try{
        boost::asio::io_context io_context;
        tcp_server server1(io_context);
        //udp_server server2(io_context);
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main()
{
    auto server_ = std::async(std::launch::async, server);
    Sleep(2000);
   /*if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }*/

    boost::asio::io_context io_context;
    tcp_client client = tcp_client(io_context);

    
    //exit(EXIT_SUCCESS);
    return 0;
}