#include "KVStore.h"
#include <functional>

KVStore::KVStore(int numShards) {
    shards.resize(numShards);
}

int KVStore::getShardIndex(const std::string& key) {
    std::hash<std::string> hashFn;
    return hashFn(key) % shards.size();
}

std::string KVStore::get(const std::string& key) {
    int idx = getShardIndex(key);
    return shards[idx].get(key);
}

void KVStore::set(const std::string& key, const std::string& value) {
    int idx = getShardIndex(key);
    shards[idx].set(key, value);
}

bool KVStore::del(const std::string& key) {
    int idx = getShardIndex(key);
    return shards[idx].del(key);
}
