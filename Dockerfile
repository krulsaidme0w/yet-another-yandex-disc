FROM fedora:36

RUN dnf -y update
RUN dnf -y install git
RUN dnf -y install gcc-c++ cmake
RUN dnf -y install json-devel libpq-devel libpqxx-devel

WORKDIR app

COPY . .

RUN make build

EXPOSE 9080

CMD ["./build/cmd/yet_another_yandex_disc"]
