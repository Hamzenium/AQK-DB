#pragma once
#include <unordered_map>
#include <string>

class Shard {
public:
    bool get(const std::string& key, std::string& out);
    void set(const std::string& key, const std::string& value);
    bool del(const std::string& key);

private:
    std::unordered_map<std::string, std::string> data;
};
