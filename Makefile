CC = gcc
CFLAGS = -Wall -pedantic --std=c99

default = bark

bark: bin.o board.o deck.o error.o game.o main.o player.o score.o save.o
	$(CC) $(CFLAGS) -o bark bin.o board.o deck.o error.o game.o main.o player.o score.o save.o

bin.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c bin.c

board.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c board.c

deck.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c deck.c

error.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c error.c

game.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c game.c

main.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c main.c

player.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c player.c

score.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c score.c

save.o: bin.c bin.h board.c board.h deck.c deck.h error.c error.h game.c game.h main.c personalization.h player.c player.h score.c score.h save.c save.h
	$(CC) $(CFLAGS) -c save.c

clean:
	$(RM) bark *.o *~
