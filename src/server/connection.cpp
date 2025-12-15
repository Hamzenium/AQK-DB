#include "server/connection.h"
#include <boost/asio/write.hpp>

using boost::asio::ip::tcp;

connection::connection(tcp::socket socket,
                       CommandDispatcher& dispatcher)
    : socket_(std::move(socket)),
      dispatcher_(dispatcher) {}

connection::ptr connection::create(tcp::socket socket,
                                   CommandDispatcher& dispatcher) {
    return std::shared_ptr<connection>(
        new connection(std::move(socket), dispatcher)
    );
}

void connection::start() {
    read_from_client();
}

void connection::read_from_client() {
    auto self = shared_from_this();

    socket_.async_read_some(
        boost::asio::buffer(buffer_),
        [this, self](boost::system::error_code ec, std::size_t bytes) {
            if (ec) return;

            std::string input(buffer_.data(), bytes);
            Command cmd = parser_.parse(input);
            std::string response = dispatcher_.execute(cmd);

            write_to_client(response);
        }
    );
}

void connection::write_to_client(const std::string& response) {
    auto self = shared_from_this();

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(response),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                read_from_client();
            }
        }
    );
}
