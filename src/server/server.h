#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

class server {
public:
    server(boost::asio::io_context& io, unsigned short port);

private:
    void accept_clients();

    boost::asio::ip::tcp::acceptor acceptor_;
};
