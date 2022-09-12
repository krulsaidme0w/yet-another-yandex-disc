#pragma once

#include <memory>
#include <string>

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
                    "SELECT parentId FROM storage WHERE parentId = '" + pqxx::to_string(file.parentId) + "'), '" +
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

    private:
        pqxx::connection c;
    };
}
