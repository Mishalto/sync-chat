#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <vector>

using boost::asio::ip::tcp;

// Listening this port
constexpr int port = 12345;

// Creates an io_context to manage and boost::system::error_code for error handling
boost::asio::io_context io_context;
boost::system::error_code ec;

// Acceptor settings
tcp::endpoint ep(tcp::v4(), port);
tcp::acceptor acceptor(io_context, ep);

bool commands(std::string_view command)
{
    constexpr std::string_view exit = "/exit";
    if (command == exit) {
        return true;
    }

    return false;
}

// This is where we send
bool send_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    // Get message to send
    std::string message;
    std::cout << "Mishalto: ";
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
void receive_message(std::shared_ptr<tcp::socket> &socket_ptr)
{
    // Start reading information from the socket
    std::array<char, 1024> data;
    size_t length = socket_ptr->read_some(boost::asio::buffer(data), ec);

    // Error handling
    if (ec) {
        std::cerr << "Read error!\n";
        std::cerr << ec.message() << '\n';
    }

    // Retrieve/извлекаем the message
    std::cout << std::string(data.data(), length) << '\n';
}

int main()
{
    // For correct error display
    setlocale(LC_ALL, "");

    auto socket_ptr = std::make_shared<tcp::socket>(io_context);

    // Constantly trying to conect a client
    while (true)
    {
        // Start accepting connections
        std::cout << "Waiting client...\n";
        acceptor.accept(*socket_ptr, ec);

        // Accept error handling
        // If an error is detected, continue accepting connections
        if (ec) {
            std::cerr << "Accept failed: " << ec.message() << '\n';
        } else {
            // If the connection is established suc, loop break
            std::cout << "Client connected.\n";
            break;
        }
    }

    // Infinity loop of communication
    // In progress
    // Needs to be finalized
    for(;;)
    {
        // Its a little confusing here
        // But reads like this, if sending a message sends false, it is a loop break
        if (!send_message(socket_ptr)) {
            break;
        }
        receive_message(socket_ptr);
    }

    // The end!
    std::cout << "This communication is over, thank you!\n";

    return 0;
}