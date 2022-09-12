#pragma once

#include "cstdint"
#include "vector"
#include "iostream"

#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace Models {
    struct Item {
    public:
        Item(json js) {
            if(js.contains("type")) {
                this->type = js["type"].get<std::string>();
            } else {
                throw std::runtime_error("BAD JSON");
            }

            if(this->type != "FILE" && this->type != "FOLDER") {
                throw std::runtime_error("BAD JSON");
            }
        
            if(js.contains("id")) {
                this->id = js["id"].get<std::string>();
            } else {
                throw std::runtime_error("BAD JSON");
            }

            if(js.contains("parentId")) {
                this->parentId = js["parentId"].get<std::string>();
            } else {
                throw std::runtime_error("BAD JSON");
            }

            if (this->type == "FOLDER") {
                this->url = nullptr;
                this->size = 0;

                return;
            }

            if(js.contains("size")) {
                this->size = js["size"];
            } else {
                throw std::runtime_error("BAD JSON");
            }

            if(this->url.size() > 255) {
                throw std::runtime_error("BAD JSON");
            }

            if(js.contains("url")) {
                this->url = js["url"].get<std::string>();
            } else {
                throw std::runtime_error("BAD JSON");
            }
        };

        std::string id;
        std::string url;
        std::string parentId;
        uint64_t size;
        std::string type;
    };

    std::vector<Item> ParseItems(json js) {
        std::vector<Item> items;

        for (json::iterator it = js.begin(); it != js.end(); ++it) {
            try {
                auto item = Item(it.value());
                items.push_back(item);
            }
            catch (...) {
                throw std::runtime_error("BAD JSON");
            }
        }

        return items;
    }
}
