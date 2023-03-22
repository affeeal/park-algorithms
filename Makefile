.PHONY: clean generate build \
        01 02 03 04 05
clean:
	rm -rf build/ .cache/

generate:
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build

silent-build:
	cmake --build build/

build:
	@make -s silent-build 

01:
	./build/01/01

02:
	./build/02/02

03:
	./build/03/03

04:
	./build/04/04

05:
	./build/05/05
