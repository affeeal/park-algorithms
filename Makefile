.PHONY: clean generate build
	
clean:
	rm -rf build/ .cache/

generate:
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build

silent-build:
	cmake --build build/

build:
	@make -s silent-build 

