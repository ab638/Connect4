# Makefile CS375 Project 3


connect4: main.c menu.c game.c score.c
	g++ -o connect4 main.c menu.c game.c score.c connect4.h -lncurses -lgdbm
clean:
	rm -f *.o connect4 *.*~
