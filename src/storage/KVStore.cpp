#include "storage/KVStore.h"
#include <functional>

KVStore::KVStore(int shards) : shards_(shards) {}

int KVStore::shard_for(const std::string& key) {
    return std::hash<std::string>{}(key) % shards_.size();
}

bool KVStore::get(const std::string& key, std::string& out) {
    return shards_[shard_for(key)].get(key, out);
}

void KVStore::set(const std::string& key, const std::string& value) {
    shards_[shard_for(key)].set(key, value);
}

bool KVStore::del(const std::string& key) {
    return shards_[shard_for(key)].del(key);
}
