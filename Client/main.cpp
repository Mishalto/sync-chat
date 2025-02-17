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

// Its just checking the message for commands
bool commands(std::string_view command)
{
    constexpr std::string_view exit = "/exit";

    return command == exit;
}

// This is where we send
bool send_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    // Get message to send
    std::string message;
    std::cout << "Client: ";
    std::getline(std::cin, message);

    // Cheking for a desire to get out
    if(commands(message)) {
        return false;
    }
    // Message transportation
    boost::asio::write(*socket_ptr, boost::asio::buffer(message), ec);

    // Error handling
    if (ec) {
        std::cout << "Writing error!\n"<< '\n' << ec.message() << '\n';
    }
    return true;
}
// This is where is receive
bool receive_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    // Start reading information from the socket
    std::array<char, 1024> data;
    size_t length = socket_ptr->read_some(boost::asio::buffer(data), ec);

    // Error handling
    if (ec) {
        if(ec == boost::asio::error::eof) {
            std::cout << "The server dropped the connection\n";
        }
        else
        {
            std::cerr << "Read error!\n";
            std::cerr << ec.message() << '\n';
        }
        return false;
    }

    // Retrieve/извлекаем the message
    std::cout << std::string(data.data(), length) << '\n';

    return true;
}

int main()
{
    // For correct error display
    setlocale(LC_ALL, "");

    std::cout << "Trying to connect to server...\n";

    auto socket_ptr = std::make_shared<tcp::socket>(io_context);

    // Infinity loop for connection
    bool try_to_connect = true;
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

    // Infinity loop of communication
    // In progress
    // Needs to be finalized
    for(;;)
    {
        // Its a little confusing here / Это немного запутанно
        // But reads like this / но читается так, if send message sends false, it is a loop break
        if(receive_message(socket_ptr)) {}
        else {
            break;
        }

        if (!send_message(socket_ptr)) {
            break;
        }
    }

    // The end
    std::cout << "This communication is over, thank you!\n";

    return 0;
}