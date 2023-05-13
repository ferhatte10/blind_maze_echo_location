build:
	gcc -Wall -Wextra -pedantic ./src/*.c -o maze -lopenal -lalut -lm -lpthread -ldl  `sdl2-config --cflags` `sdl2-config --libs` -I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lSDL2_ttf;
run:
	./maze;

clean:
	rm maze;

install:
	sudo apt-get install libsdl2-dev libsdl2-ttf-dev libalut-dev;
