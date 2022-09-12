#pragma once

#include <memory>
#include <string>
#include <vector>

#include "pqxx/pqxx"

#include "disc_models.cpp"

namespace Repository {
    class DiscRepo {
    public:
        DiscRepo() = default;
        explicit DiscRepo(const char *options) : c(options) {}

        void InsertFile(Models::Item file, std::string date) {
            if(file.type != "FILE") {
                throw std::runtime_error("WRONG ITEM TYPE");
                return;
            }

            if(file.parentId == "null") {
                std::string values = 
                    "('" +
                    pqxx::to_string(file.id) + "', '" + 
                    pqxx::to_string(file.url) + "', '" + 
                    pqxx::to_string(file.parentId) + "', '" +
                    pqxx::to_string(file.size) + "', '" +
                    pqxx::to_string(file.type) + "', '" +
                    pqxx::to_string(date) + "') ";

                std::string query = "INSERT INTO storage (id, url, parentId, size, type, updateDate) VALUES "
                    + values +
                    "ON CONFLICT (id) DO UPDATE SET " +
                    "id = '" + pqxx::to_string(file.id) + "'," +
                    "url = '" + pqxx::to_string(file.url) + "'," +
                    "parentId = '" + pqxx::to_string(file.parentId) + "'," +
                    "size = '" + pqxx::to_string(file.size) + "'," +
                    "type = '" + pqxx::to_string(file.type) + "'," +
                    "updateDate = '" + pqxx::to_string(date) + "';";

                pqxx::work txn(this->c);
                try {
                    pqxx::result R{txn.exec(query)};
                    txn.commit();
                }
                catch (std::exception const &e) {
                    throw std::runtime_error(std::string("CANNOT INSERT FILE (NO PARENT): ") + std::string(e.what()) + "\n" + query);
                }
            } 
            else {
                std::string values = 
                    "('" +
                    pqxx::to_string(file.id) + "', '" +
                    pqxx::to_string(file.url) + "', (" +
                    "SELECT id FROM storage WHERE id = '" + pqxx::to_string(file.parentId) + "'), '" +
                    pqxx::to_string(file.size) + "', '" +
                    pqxx::to_string(file.type) + "', '" +
                    pqxx::to_string(date) + "') ";

                std::string query = "INSERT INTO storage (id, url, parentId, size, type, updateDate) VALUES "
                    + values +
                    "ON CONFLICT (id) DO UPDATE SET " +
                    "id = '" + pqxx::to_string(file.id) + "'," +
                    "url = '" + pqxx::to_string(file.url) + "'," +
                    "parentId = '" + pqxx::to_string(file.parentId) + "'," +
                    "size = '" + pqxx::to_string(file.size) + "'," +
                    "type = '" + pqxx::to_string(file.type) + "'," +
                    "updateDate = '" + pqxx::to_string(date) + "';";
                    
                pqxx::work txn{this->c};
                try {
                    pqxx::result R{txn.exec(query)};
                    txn.commit();
                }
                catch (std::exception const &e) {
                    if(std::string(e.what()).rfind("ERROR:  null value in column \"parentid\" of relation \"storage\" violates not-null constraint", 0) == 0) {
                        throw std::runtime_error("NO PARENT"); 
                    }

                    throw std::runtime_error(std::string(e.what()));
                }
            }

            return;
        }

        void InsertFolder(Models::Item folder, std::string date) {
            if(folder.type != "FOLDER") {
                throw std::runtime_error("WRONG ITEM TYPE");
                return;
            }

            if(folder.parentId == "null") {
                std::string values = 
                    "('" +
                    pqxx::to_string(folder.id) + "', '" + 
                    pqxx::to_string(folder.parentId) + "', '" +
                    pqxx::to_string(folder.type) + "', '" +
                    pqxx::to_string(date) + "') ";

                std::string query = "INSERT INTO storage (id, parentId, type, updateDate) VALUES "
                    + values +
                    "ON CONFLICT (id) DO UPDATE SET " +
                    "id = '" + pqxx::to_string(folder.id) + "'," +
                    "parentId = '" + pqxx::to_string(folder.parentId) + "'," +
                    "type = '" + pqxx::to_string(folder.type) + "'," +
                    "updateDate = '" + pqxx::to_string(date) + "';";

                pqxx::work txn(this->c);
                try {
                    pqxx::result R{txn.exec(query)};
                    txn.commit();
                }
                catch (std::exception const &e) {
                    throw std::runtime_error(std::string("CANNOT INSERT FILE (NO PARENT): ") + std::string(e.what()) + "\n" + query);
                }
            } 
            else {
                std::string values = 
                    "('" +
                    pqxx::to_string(folder.id) + "', " +
                    "(SELECT id FROM storage WHERE id = '" + pqxx::to_string(folder.parentId) + "'), '" +
                    pqxx::to_string(folder.type) + "', '" +
                    pqxx::to_string(date) + "') ";

                std::string query = "INSERT INTO storage (id, parentId, type, updateDate) VALUES "
                    + values +
                    "ON CONFLICT (id) DO UPDATE SET " +
                    "id = '" + pqxx::to_string(folder.id) + "'," +
                    "parentId = '" + pqxx::to_string(folder.parentId) + "'," +
                    "type = '" + pqxx::to_string(folder.type) + "'," +
                    "updateDate = '" + pqxx::to_string(date) + "';";
                    
                pqxx::work txn{this->c};
                try {
                    pqxx::result R{txn.exec(query)};
                    txn.commit();
                }
                catch (std::exception const &e) {
                    if(std::string(e.what()).rfind("ERROR:  null value in column \"parentid\" of relation \"storage\" violates not-null constraint", 0) == 0) {
                        throw std::runtime_error("NO PARENT"); 
                    }

                    throw std::runtime_error(std::string(e.what()));
                }
            }

            return;
        }

        std::optional<Models::Item> GetItemById(std::string id) {
            std::string query = "SELECT * FROM storage WHERE id = '" + pqxx::to_string(id) + "';";

            pqxx::work txn{this->c};
            try {
                pqxx::result R{txn.exec(query)};
                txn.commit();
                
                for(auto row : R) {
                    Models::Item item;
                    item.type = row[4].as<std::string>(); 
                    item.id = row[0].as<std::string>();
                    item.parentId = row[2].as<std::string>();
                    item.updateDate = row[5].as<std::string>();

                    if(item.type == "FOLDER") {
                        item.url = "null";
                        item.size = 0;
                        return item;
                    }
                    
                    item.url = row[1].as<std::string>();
                    item.size = row[3].as<int>();
                    return item;
                }
                return std::nullopt;
            }
            catch (std::exception const &e) {
                throw std::runtime_error(std::string(e.what()));
            }
        }

        std::vector<Models::ItemWithChildren*> GetChildren(std::string id) {
            std::string query = "SELECT * FROM storage WHERE parentId = '" + pqxx::to_string(id) + "';";

            pqxx::work txn{this->c};
            try {
                pqxx::result R{txn.exec(query)};
                txn.commit();
                
                std::vector<Models::ItemWithChildren*> items;
                for(auto row : R) {
                    Models::Item item;
                    item.type = row[4].as<std::string>(); 
                    item.id = row[0].as<std::string>();
                    item.parentId = row[2].as<std::string>();
                    item.updateDate = row[5].as<std::string>();

                    if(item.type == "FOLDER") {
                        item.url = "null";
                        item.size = 0;
                        Models::ItemWithChildren* itemWithChildren = new Models::ItemWithChildren;
                        itemWithChildren->item = item;
                        items.push_back(itemWithChildren);
                        continue;
                    }
                    
                    item.url = row[1].as<std::string>();
                    item.size = row[3].as<int>();
                    
                    Models::ItemWithChildren* itemWithChildren = new Models::ItemWithChildren;
                    itemWithChildren->item = item;
                    items.push_back(itemWithChildren);
                }

                return items;
            }
            catch (std::exception const &e) {
                throw std::runtime_error(std::string(e.what()));
            }
        }

        void UpdateDate(std::string id, std::string date) {
            std::string queryUpdateParent = "UPDATE storage SET updateDate = '" + pqxx::to_string(date) + "' WHERE parentId = '" + pqxx::to_string(id) + "';";
            pqxx::work txn{this->c};
            try {
                pqxx::result R{txn.exec(queryUpdateParent)};
                txn.commit();
            }
            catch (std::exception const &e) {
                throw std::runtime_error(std::string("CANNOT UPDATE PARENT: ") + std::string(e.what()) + "\n" + queryUpdateParent);
            }
        }

    private:
        pqxx::connection c;
    };
}
