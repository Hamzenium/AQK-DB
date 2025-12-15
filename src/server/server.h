#pragma once

#include <boost/asio/ip/tcp.hpp>
#include "command/CommandDispatcher.h"

class server {
public:
    server(boost::asio::io_context& io,
           unsigned short port,
           CommandDispatcher& dispatcher);

private:
    void accept_clients();

    boost::asio::ip::tcp::acceptor acceptor_;
    CommandDispatcher& dispatcher_;
};
