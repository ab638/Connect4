# Makefile CS375 Project 3
# A really bad and simple makefile
# Austin Brummett

connect4: main.c game.c connect4.h
	gcc -g -o connect4 main.c game.c connect4.h -lncurses -lgdbm
clean:
	rm -f *.o connect4 *.*~
