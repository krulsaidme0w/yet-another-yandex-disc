build:
	rm -rf build && mkdir build && cd build && cmake .. && cmake --build .

run:
	./build/cmd/yet_another_yandex_disc
