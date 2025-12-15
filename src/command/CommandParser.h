#pragma once
#include "command/Command.h"
#include <string>

class CommandParser {
public:
    Command parse(const std::string& input);
};
