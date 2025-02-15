#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>
#include <chrono>

using boost::asio::ip::tcp;

// Server port
const int port = 12345;

// Creates an io_context to manage and boost::system::error_code for error handling
boost::asio::io_context io_context;
boost::system::error_code ec;

// Server addr
tcp::endpoint ep(boost::asio::ip::make_address_v4("127.0.0.1"), port);

// Receive message from client
std::string receive_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    std::string message;
    std::getline(std::cin, message);

    char data[1024];
    size_t length = socket_ptr->read_some(boost::asio::buffer(message));
    return std::string(data, length);
}

int main()
{
    // For correct error display
    setlocale(LC_ALL, "");

    std::cout << "Trying to connect to server...\n";
    bool try_to_connect = true;

    // 
    while(try_to_connect)
    {
        auto socket_ptr = std::make_shared<tcp::socket>(io_context);
        socket_ptr->connect(ep, ec);

        // Connection error handling
        if (ec) {
            std::cerr << "Connection error!\n" << ec.message() << '\n';
            std::cerr << "Trying to reconnect in 5 seconds\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        } else {
            std::cout << "Connected\n";
            try_to_connect = false;
        }
    }

    return 0;
}