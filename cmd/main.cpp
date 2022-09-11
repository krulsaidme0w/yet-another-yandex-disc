#include "iostream"

#include "disc_delivery.cpp"

int main() {
    Port port(80);

    int thr = 4;

    Address addr(Ipv4::any(), port);

    Endpoints::DiscEndpoints endpoints(addr);

    endpoints.Init(thr);
    endpoints.Start();
}
