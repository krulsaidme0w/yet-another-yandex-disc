#include <memory>
#include <string>

#include "pqxx/pqxx"

#include "disc_models.cpp"

namespace Repository {
    class DiscRepo {
    public:
        DiscRepo() = default;
        explicit DiscRepo(const char *options) : c(options) {}

        void InsertFiles(std::vector<Models::Item> files, std::string date) {
            for(auto file : files) {
                if(file.type != "FILE") {
                    throw "WRONG TYPE";
                    return;
                }

                if(file.parentId == "null") {
                    std::string values = 
                        "(" +
                        pqxx::to_string(file.id) + ", " + 
                        pqxx::to_string(file.url) + ", " + 
                        pqxx::to_string(file.parentId) + ", " +
                        pqxx::to_string(file.size) + ", " +
                        pqxx::to_string(file.type) + ", " +
                        pqxx::to_string(date) + ")";

                    std::string query = "INSERT INTO storage (id, url, parentId, size, type, updateDate)"
                                        + values +
                                        "ON DUPLICATE KEY UPDATE" +
                                        "id = " + pqxx::to_string(file.id) + "," +
                                        "url = " + pqxx::to_string(file.url) + "," +
                                        "parentId = " + pqxx::to_string(file.parentId) + "," +
                                        "size = " + pqxx::to_string(file.size) + "," +
                                        "type = " + pqxx::to_string(file.type) + "," +
                                        "updateDate = " + pqxx::to_string(date) + ";";

                    pqxx::work txn(this->c);
                    try {
                        pqxx::result R{txn.exec(query)};
                    }
                    catch ( ... ) {
                        throw "CANNOT INSERT FILE";
                    }
                } 
                else {
                    std::string values = 
                        "(" +
                        pqxx::to_string(file.id) + ", " +
                        pqxx::to_string(file.url) + ", (" +
                        "SELECT parentId FROM storage WHERE parentId = " + pqxx::to_string(file.parentId) + "), " +
                        pqxx::to_string(file.size) + ", " +
                        pqxx::to_string(file.type) + ", " +
                        pqxx::to_string(date) + ")";

                    std::string query = "INSERT INTO storage (id, url, parentId, size, type, updateDate)"
                                        + values +
                                        "ON DUPLICATE KEY UPDATE" +
                                        "id = " + pqxx::to_string(file.id) + "," +
                                        "url = " + pqxx::to_string(file.url) + "," +
                                        "parentId = " + pqxx::to_string(file.parentId) + "," +
                                        "size = " + pqxx::to_string(file.size) + "," +
                                        "type = " + pqxx::to_string(file.type) + "," +
                                        "updateDate = " + pqxx::to_string(date) + ";";
                    
                    pqxx::work txn{this->c};
                    try {
                        pqxx::result R{txn.exec(query)};
                    }
                    catch ( ... ) {
                        throw "CANNOT INSERT FILE";
                    }
                }

                return;
            }
        }

    private:
        pqxx::connection c;
    };
}
