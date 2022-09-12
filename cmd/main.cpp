#pragma once

#include "iostream"
#include <memory>

#include "pqxx/pqxx"

#include "disc_delivery.cpp"
#include "disc_repo.cpp"

int main() {
    try {
        const char *options = "host=0.0.0.0 port=5432 user=user password=password dbname=disc";
        pqxx::connection c(options);
    }
    catch (std::exception const &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    const char *options = "host=0.0.0.0 port=5432 user=user password=password dbname=disc";
    auto repo = std::make_shared<Repository::DiscRepo>(Repository::DiscRepo(options));

    Port port(9080);

    int thr = 4;

    Address addr(Ipv4::any(), port);
    Endpoints::DiscEndpoints endpoints(addr, repo);

    endpoints.Init(thr);
    endpoints.Start();
}
