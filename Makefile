COMPILER = g++

all: compile run

compile: 
	$(COMPILER) ./src/main.cpp -o ./build/main
run: 
	./build/main

clean:
	rm -f ./build/main
