#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

// Listening this port
const int port = 12345;

// Creates an io_context to manage
boost::asio::io_context io_context;

// Acceptor settings
tcp::endpoint ep(tcp::v4(), port);
tcp::acceptor acceptor(io_context, ep);

int main()
{
    // Waiting client
    auto socket_ptr = std::make_shared<tcp::socket>(io_context);
    std::cout << "Waiting client...\n";
    acceptor.accept(*socket_ptr);

    return 0;
}