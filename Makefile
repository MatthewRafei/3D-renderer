build:
	gcc -Wall -std=c17 ./src/*.c -I/src/include -lSDL2 -o renderer

debug:
	gcc -Wall -std=c17 -ggdb ./src/*.c -I/src/include -lSDL2 -o renderer_debug

clean:
	rm -f renderer renderer_debug

run:
	./renderer
