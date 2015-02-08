CC=gcc
CFLAGS=-Wall -Wextra -O3 -g
LDFLAGS=-lncurses
SRC_DIR=src

SRC_OBJECTS = 	$(SRC_DIR)/draw.o \
		$(SRC_DIR)/game.o \
		$(SRC_DIR)/damas.o \
		$(SRC_DIR)/connection.o \

damas: $(SRC_OBJECTS)
	$(CC) -o damas $(SRC_OBJECTS) $(LDFLAGS)
	@rm $(SRC_DIR)/*.o

clean:
	rm damas
