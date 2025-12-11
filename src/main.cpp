#include <iostream>
#include "storage/KVStore.h"

int main() {
    KVStore store(8);

    store.set("foo", "bar");
    store.set("name", "Hamza");
    store.set("course", "C++ Systems");

    std::cout << "foo = " << store.get("foo") << std::endl;
    std::cout << "name = " << store.get("name") << std::endl;
    std::cout << "course = " << store.get("course") << std::endl;

    std::string missing = store.get("missing");
    if (missing == "") {
        std::cout << "missing = <not found>" << std::endl;
    }

    bool deleted = store.del("foo");
    std::cout << "delete foo: " << (deleted ? "success" : "failed") << std::endl;
    std::cout << "foo after delete = " << store.get("foo") << std::endl;

    return 0;
}
