PLATFORM=$(shell uname -s)
ARCH=$(shell uname -m)

CFLAGS=-Wall -O3

SRC=main.c hitable.c material.c vec3.c ray.c camera.c
OBJ:=$(SRC:.c=.o)
BIN=raytracer

# Run each test program and echo exit code
$(BIN): $(OBJ)
	$(CC) -o $@ $^  $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -v *.o $(BIN)

.PHONY: clean all
