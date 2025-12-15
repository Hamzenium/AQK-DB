#include "storage/Shard.h"

bool Shard::get(const std::string& key, std::string& out) {
    auto it = data.find(key);
    if (it == data.end()) return false;
    out = it->second;
    return true;
}

void Shard::set(const std::string& key, const std::string& value) {
    data[key] = value;
}

bool Shard::del(const std::string& key) {
    return data.erase(key) > 0;
}
