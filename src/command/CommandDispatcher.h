#pragma once
#include "command/Command.h"
#include "storage/KVStore.h"
#include <string>

class CommandDispatcher {
public:
    explicit CommandDispatcher(KVStore& store);
    std::string execute(Command cmd);

private:
    KVStore& store_;
};

