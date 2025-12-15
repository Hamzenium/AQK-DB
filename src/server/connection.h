#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <memory>

class connection : public std::enable_shared_from_this<connection> {
public:
    using ptr = std::shared_ptr<connection>;

    static ptr create(boost::asio::ip::tcp::socket socket);

    explicit connection(boost::asio::ip::tcp::socket socket);

    void start();

private:
    void read_from_client();
    boost::asio::ip::tcp::socket socket_;
    std::array<char, 1024> buffer_;
};
