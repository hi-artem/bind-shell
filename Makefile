all: run

run: build
	./build/bin/nc

build: clean
	conan build . --build-folder build	

clean:
	rm -rf ./build/bin/nc

install-deps:
	conan install . --install-folder build	
