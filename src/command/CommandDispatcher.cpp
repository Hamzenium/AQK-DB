#include "command/CommandDispatcher.h"

CommandDispatcher::CommandDispatcher(KVStore& store)
    : store_(store) {}

std::string CommandDispatcher::execute(Command cmd) {
    if (cmd.name == "PING") return "PONG\n";

    if (cmd.name == "SET") {
        if (cmd.args.size() != 2) return "ERROR\n";
        store_.set(cmd.args[0], cmd.args[1]);
        return "OK\n";
    }

    if (cmd.name == "GET") {
        if (cmd.args.size() != 1) return "ERROR\n";
        std::string value;
        return store_.get(cmd.args[0], value)
               ? value + "\n"
               : "(nil)\n";
    }

    if (cmd.name == "DEL") {
        if (cmd.args.size() != 1) return "ERROR\n";
        return store_.del(cmd.args[0]) ? "1\n" : "0\n";
    }

    return "ERROR unknown command\n";
}
