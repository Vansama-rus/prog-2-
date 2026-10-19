CC = gcc
CFLAGS = -Wall -Iinclude
AR = ar
LIB = libcar.a
OBJ = src/car.o
OUT = app

.PHONY: all clean

all: $(OUT)

$(OBJ): src/car.c include/car.h
	$(CC) $(CFLAGS) -c src/car.c -o $(OBJ)

$(LIB): $(OBJ)
	$(AR) rcs $(LIB) $(OBJ)

$(OUT): src/main.c $(LIB)
	$(CC) $(CFLAGS) src/main.c -L. -lcar -o $(OUT)

clean:
	del src\car.o $(LIB) $(OUT).exe
