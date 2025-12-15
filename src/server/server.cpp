#include "server/server.h"
#include "server/connection.h"
#include <iostream>

using boost::asio::ip::tcp;

server::server(boost::asio::io_context& io,
               unsigned short port,
               CommandDispatcher& dispatcher)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port)),
      dispatcher_(dispatcher) {
    std::cout << "Server listening on port " << port << "\n";
    accept_clients();
}

void server::accept_clients() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                auto conn = connection::create(
                    std::move(socket), dispatcher_);
                conn->start();
            }
            accept_clients();
        }
    );
}
