.PHONY: all configure build clean rebuild

all: configure build

configure:
	mkdir -p build
	cd build && cmake ..

build:
	cmake --build build

clean:
	rm -rf build

rebuild: clean all
