#pragma once
#include "storage/Shard.h"
#include <vector>
#include <string>

class KVStore {
public:
    explicit KVStore(int shards = 8);

    bool get(const std::string& key, std::string& out);
    void set(const std::string& key, const std::string& value);
    bool del(const std::string& key);

private:
    std::vector<Shard> shards_;
    int shard_for(const std::string& key);
};
