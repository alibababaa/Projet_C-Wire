CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

all: program

program: main.o avl_tree.o processgraphique.o
	$(CC) $(CFLAGS) main.o avl_tree.o processgraphique.o -o program

main.o: main.c avl_tree.h
	$(CC) $(CFLAGS) -c main.c

avl_tree.o: avl_tree.c avl_tree.h
	$(CC) $(CFLAGS) -c avl_tree.c

processgraphique.o: processgraphique.c avl_tree.h
	$(CC) $(CFLAGS) -c processgraphique.c

clean:
	rm -f *.o program
