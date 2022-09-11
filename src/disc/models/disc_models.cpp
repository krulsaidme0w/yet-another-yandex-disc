#include "cstdint"
#include "vector"
#include "iostream"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Models {
    enum ItemType {
        file = 0,
        folder = 1,
    };
    
    struct Item {
    public:
        Item(json js) {
            std::cout << js["id"] << std::endl;
            std::cout << js["url"] << std::endl;
            std::cout << js["parentId"] << std::endl;
            std::cout << js["size"] << std::endl;
            std::cout << js["type"] << std::endl;
        };

        std::string id;
        std::string url;
        std::string parentId;
        uint64_t size;
        ItemType type;
    };

    std::vector<Item> ParseItems(json js) {
        std::vector<Item> items;

        for (json::iterator it = js.begin(); it != js.end(); ++it) {
            auto item = Item(it.value());
            items.push_back(item);
        }

        return items;
    }
}
