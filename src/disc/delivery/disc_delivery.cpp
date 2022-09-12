#include "iostream"
#include <memory>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "disc_models.cpp"
#include "disc_repo.cpp"

using namespace Pistache;

namespace Endpoints {
    class DiscEndpoints {
    public:
        DiscEndpoints() = delete;
        DiscEndpoints(Address addr, std::shared_ptr<Repository::DiscRepo> r) {
            repo = r;
            this->httpEndpoint = std::make_shared<Http::Endpoint>(addr);
        }

        void Init(size_t thr = 4) {
            auto opts = Http::Endpoint::options()
                            .threads(static_cast<int>(thr));
            httpEndpoint->init(opts);
            setupRoutes();
        }

        void Start() {
            httpEndpoint->setHandler(router.handler());
            httpEndpoint->serve();
        }

    private:
        void setupRoutes() {
            using namespace Rest;

            Routes::Post(router, "/imports", Routes::bind(&DiscEndpoints::importFiles, this));
            Routes::Delete(router, "/delete/:id", Routes::bind(&DiscEndpoints::deleteFileByID, this));
            Routes::Get(router, "/nodes/:id", Routes::bind(&DiscEndpoints::getNodeByID, this));
        }

        void importFiles(const Rest::Request& request, Http::ResponseWriter response) {
            json j;
            try {
                j = json::parse(request.body());
            }
            catch (...) {
                response.send(Http::Code::Bad_Request, "BAD JSON");
                return;
            }
            
            if(!j.contains("items") || !j.contains("updateDate")) {
                response.send(Http::Code::Bad_Request, "NO ITEMS ARRAY OR NO UPDATE DATE FIELD");
                return;
            }

            const auto updateDate = j["updateDate"].get<std::string>();
            json jsItems = j["items"];

            std::vector<Models::Item> items;
            try {
                items = Models::ParseItems(jsItems);
            } catch (...) {
                response.send(Http::Code::Bad_Request, "BAD ITEM JSON");
                return;
            }

            for(auto i : items) {
                if(i.type == "FILE") {
                    try {
                        repo.get()->InsertFile(i, updateDate);
                    }
                    catch (std::exception const &e) {
                        if(e.what() == "NO PARENT") {
                            response.send(Http::Code::Bad_Request, e.what());
                        }
                        else {
                            response.send(Http::Code::Internal_Server_Error, e.what());
                        }
                    }
                    continue;
                }
            }

            response.send(Http::Code::Ok, "OK");
        }

        void deleteFileByID(const Rest::Request& request, Http::ResponseWriter response) {
            auto id = request.param(":id").as<std::string>();

            response.send(Http::Code::Ok, "OK");
        }

        void getNodeByID(const Rest::Request& request, Http::ResponseWriter response) {
            auto id = request.param(":id").as<std::string>();

            response.send(Http::Code::Ok, id);
        }

        const char *options = "host=0.0.0.0 port=5432 user=user password=password dbname=disc";
        std::shared_ptr<Http::Endpoint> httpEndpoint;
        Rest::Router router;
        std::shared_ptr<Repository::DiscRepo> repo;
    };
}
