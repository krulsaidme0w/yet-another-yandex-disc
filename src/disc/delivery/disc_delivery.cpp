#include "iostream"

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "disc_models.cpp"

using namespace Pistache;

namespace Endpoints {
    class DiscEndpoints {
    public:
        explicit DiscEndpoints(Address addr)
            : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}

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
            json j = json::parse(request.body());
            const auto updateDate = j["updateDate"].get<std::string>();
            std::cout << updateDate << '\n';

            json items = j["items"];
            std::vector<Models::Item> items = Models::ParseItems(items);

            response.send(Http::Code::Ok, "import files");
        }

        void deleteFileByID(const Rest::Request& request, Http::ResponseWriter response) {
            response.send(Http::Code::Ok, "delete files");
        }

        void getNodeByID(const Rest::Request& request, Http::ResponseWriter response) {
            response.send(Http::Code::Ok, "get node");
        }

        std::shared_ptr<Http::Endpoint> httpEndpoint;
        Rest::Router router;
    };
}
