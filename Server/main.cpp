#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

// Listening this port
const int port = 12345;

// Creates an io_context to manage and boost::system::error_code for error handling
boost::asio::io_context io_context;
boost::system::error_code ec;

// Acceptor settings
tcp::endpoint ep(tcp::v4(), port);
tcp::acceptor acceptor(io_context, ep);

// This is where we send
void send_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    std::string message;
    std::cout << "Mishalto: ";
    std::getline(std::cin, message);

    boost::asio::write(*socket_ptr, boost::asio::buffer(message));
}

// This is where is receive
void receive_message(std::shared_ptr<tcp::socket>& socket_ptr)
{
    // Start reading information from the socket
    std::array<char, 1024> data;
    size_t length = socket_ptr->read_some(boost::asio::buffer(data), ec);

    // Temporary solution, probably bad practice
    if (ec) {
        std::cerr << "Read error!\n";
        std::cerr << ec.message() << '\n';
        std::cout << "<blank>\n";
    }

    std::cout << std::string(data.data(), length) << '\n';
}

int main()
{
    // For correct error display
    setlocale(LC_ALL, "");

    // Start accepting connections
    auto socket_ptr = std::make_shared<tcp::socket>(io_context);
    std::cout << "Waiting client...\n";
    acceptor.accept(*socket_ptr, ec);

    // Accept error handling
    if (ec) {
        std::cerr << "Accept failed: " << ec.message() << '\n';
    } else {
        std::cout << "Client connected.\n";
    }

    // Infinity loop of communication
    // In progress
    // Needs to be finalized
    for(;;)
    {
        send_message(socket_ptr);
        receive_message(socket_ptr);
    }

    return 0;
}