#ifndef KVSTORE_H
#define KVSTORE_H

#include "Shard.h"
#include <vector>
#include <string>

class KVStore {
public:
    KVStore(int numShards = 8);

    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value);
    bool del(const std::string& key);

private:
    std::vector<Shard> shards;
    int getShardIndex(const std::string& key);
};

#endif // KVSTORE_H
