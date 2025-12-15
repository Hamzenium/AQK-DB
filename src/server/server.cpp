#include "server.h"
#include "connection.h"

#include <iostream>

using boost::asio::ip::tcp;

server::server(boost::asio::io_context& io, unsigned short port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server started on port " << port << "\n";
    accept_clients();
}

void server::accept_clients()
{
    acceptor_.async_accept(
        [this](boost::system::error_code error, tcp::socket client_socket)
        {
            if (!error) {
                std::cout << "Client connected\n";

                auto client = connection::create(std::move(client_socket));
                client->start();
            }

            accept_clients();
        }
    );
}
