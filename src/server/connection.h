#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <memory>
#include <string>

#include "command/CommandDispatcher.h"
#include "command/CommandParser.h"

class connection : public std::enable_shared_from_this<connection> {
public:
    using ptr = std::shared_ptr<connection>;

    static ptr create(boost::asio::ip::tcp::socket socket,
                      CommandDispatcher& dispatcher);

    void start();

private:
    connection(boost::asio::ip::tcp::socket socket,
               CommandDispatcher& dispatcher);

    void read_from_client();
    void write_to_client(const std::string& response);

    boost::asio::ip::tcp::socket socket_;
    std::array<char, 1024> buffer_;

    CommandDispatcher& dispatcher_;
    CommandParser parser_;
};
