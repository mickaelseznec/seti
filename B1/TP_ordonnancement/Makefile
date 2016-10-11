CC=gcc
CFLAGS=-I. -DDEBUG
DEPS = threads_ports.h threads_dispatch.h Makefile
OBJ = main.o threads_ports.o threads_dispatch.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	gcc -o first_rt_posix_app $^ $(CFLAGS) -lpthread -lrt

clean:
	rm -f *.o core *~ first_rt_posix_app