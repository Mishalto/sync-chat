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

// This is where we send
void send_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    std::string message;
    std::cout << "Lempa: ";
    std::getline(std::cin, message);

    boost::asio::write(*socket_ptr, boost::asio::buffer(message));
}

// This is where is receive
std::string receive_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    std::array<char, 1024> data;
    size_t length = socket_ptr->read_some(boost::asio::buffer(data));

    return std::string(data.data(), length);
}

int main()
{
    // For correct error display
    setlocale(LC_ALL, "");

    std::cout << "Trying to connect to server...\n";
    bool try_to_connect = true;

    auto socket_ptr = std::make_shared<tcp::socket>(io_context);

    // Infinity loop for connection
    while(try_to_connect)
    {
        // Starting the connection
        socket_ptr->connect(ep, ec);

        // Connection error handling
        if (ec) {
            std::cerr << "Connection error!\n" << ec.message() << '\n';
            std::cerr << "Trying to reconnect in 5 seconds...\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        } else {
            std::cout << "Connected!\n";
            try_to_connect = false;
        }
    }

    for(;;)
    {
        std::cout << receive_message(socket_ptr) << '\n';
        send_message(socket_ptr);
    }

    return 0;
}