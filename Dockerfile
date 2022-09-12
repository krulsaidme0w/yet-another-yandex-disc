FROM ubuntu:latest

WORKDIR deps

RUN apt-get update && apt-get install cmake build-essential git python3 -y
RUN apt-get install nlohmann-json3-dev -y
RUN apt-get install libpq-dev -y
RUN git clone https://github.com/jtv/libpqxx.git && cd libpqxx && ./configure && make -j4 && make install

WORKDIR app

COPY . .

RUN make build

EXPOSE 9080

CMD ["./build/cmd/yet_another_yandex_disc"]
