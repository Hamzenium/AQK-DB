#pragma once

#include <string>
#include "Command.h"
#include "KVStore.h"

class CommandDispatcher {
public:
    CommandDispatcher(KVStore& store);

    std::string execute(Command cmd);

private:
    KVStore& store_;
};
