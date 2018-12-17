# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC = gcc                       # compiler to use
OBJS = main.o queue.o

lab3: $(OBJS)
				$(CC) -pthread -o lab3 $(OBJS)

main.o: queue.h
queue.o: queue.h


.PHONY = clean
clean:
				rm lab3 $(OBJS)
