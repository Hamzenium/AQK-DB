#include "connection.h"
#include <iostream>

using boost::asio::ip::tcp;

connection::connection(tcp::socket socket)
    : socket_(std::move(socket))  // give ownership of the socket to this object
{
}

std::shared_ptr<connection> connection::create(tcp::socket socket)
{
    return std::make_shared<connection>(std::move(socket));
}

void connection::start()
{
    read_from_client();
}

void connection::read_from_client()
{
    // keep this object alive while async operation is running
    std::shared_ptr<connection> self = shared_from_this();

    socket_.async_read_some(
        boost::asio::buffer(buffer_),
        [this, self](boost::system::error_code error, std::size_t bytes_read)
        {
            if (error) {
                // client disconnected or error happened
                return;
            }

            std::string message(buffer_.data(), bytes_read);
            std::cout << "Received: " << message << "\n";

            // read again (loop)
            read_from_client();
        }
    );
}
