#include <boost/asio/io_context.hpp>
#include <iostream>

#include "server/server.h"
#include "storage/KVStore.h"
#include "command/CommandDispatcher.h"

int main() {
    try {
        boost::asio::io_context io;

        KVStore store(8);
        CommandDispatcher dispatcher(store);

        server s(io, 6379, dispatcher);

        io.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
}
