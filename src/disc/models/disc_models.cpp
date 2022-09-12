#pragma once

#include "cstdint"
#include "vector"
#include "iostream"

#include <cstddef>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace Models {
    struct Item {
    public:
        Item() {}
        Item(json js) {
            if(js.contains("type")) {
                this->type = js["type"].get<std::string>();
            } else {
                throw std::runtime_error("BAD JSON: NO TYPE");
            }

            if(this->type != "FILE" && this->type != "FOLDER") {
                throw std::runtime_error("BAD JSON: BAD TYPE");
            }
        
            if(js.contains("id")) {
                this->id = js["id"].get<std::string>();
            } else {
                throw std::runtime_error("BAD JSON: NO ID");
            }

            if(js.contains("parentId")) {
                if(js["parentId"].is_null()) {
                    this->parentId = "null";
                }
                else {
                    this->parentId = js["parentId"].get<std::string>();
                }
            } else {
                throw std::runtime_error("BAD JSON: NO PARENT ID");
            }

            if (this->type == "FOLDER") {
                this->url = "null";
                this->size = 0;

                return;
            }

            if(js.contains("size")) {
                this->size = js["size"];
            } else {
                throw std::runtime_error("BAD JSON: NO SIZE");
            }

            if(this->url.size() > 255) {
                throw std::runtime_error("BAD JSON: BAD SIZE");
            }

            if(js.contains("url")) {
                this->url = js["url"].get<std::string>();
            } else {
                throw std::runtime_error("BAD JSON: NO URL");
            }
        };

        std::string id;
        std::string url;
        std::string parentId;
        uint64_t size;
        std::string type;
        std::string updateDate;
    };

    struct ItemWithChildren {
        ItemWithChildren() {
            children = std::vector<ItemWithChildren*>();
        }
        Item item;
        std::vector<ItemWithChildren*> children;

        void SetSize() {
            for(auto i = 0; i < children.size(); i++) {
                children[i]->SetSize();
                item.size += children[i]->item.size;
            }
        }

        json ToJson() {
            json j;
            j["id"] = item.id;
            
            if(item.url == "null") {
                j["url"] = nullptr;
            }
            else {
                j["url"] = item.url;
            }

            j["type"] = item.type;

            if(item.parentId == "null") {
                j["parentId"] = nullptr;
            }
            else {
                j["parentId"] = item.parentId;
            }

            j["date"] = item.updateDate;
            j["size"] = item.size;

            json jsonChildren = json::array();
            for(auto i = 0; i < children.size(); i++) {
                jsonChildren.push_back(children[i]->ToJson());
            }

            j["children"] = jsonChildren;
            
            if(children.size() == 0 && item.type == "FILE") {
                j["children"] = nullptr;
            }

            return j;
        }
    };

    std::vector<Item> ParseItems(json js) {
        std::vector<Item> items;

        for (json::iterator it = js.begin(); it != js.end(); ++it) {
            try {
                auto item = Item(it.value());
                items.push_back(item);
            }
            catch (std::exception const &e) {
                throw std::runtime_error(e.what());
            }
        }

        return items;
    }
}
