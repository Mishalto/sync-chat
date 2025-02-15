#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

// Server port
const int port = 12345;

// Creates an io_context to manage and boost::system::error_code for error handling
boost::asio::io_context io_context;
boost::system::error_code ec;

// Server addr
tcp::endpoint ep(boost::asio::ip::make_address_v4("127.0.0.1"), port);

int main()
{
    // Trying to connect to the server
    auto socket_ptr = std::make_shared<tcp::socket>(io_context);
    socket_ptr->connect(ep, ec);

    // Connection error handling
    if (ec) {
        std::cerr << "Connection error: " << ec.what() << '\n';
    } else {
        std::cout << "Connected\n";
    }

    return 0;
}