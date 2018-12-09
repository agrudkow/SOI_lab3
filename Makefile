# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC = gcc                       # compiler to use
OBJS = main.o queue.o sem.o

lab3: $(OBJS)
				$(CC) -o lab3 $(OBJS)

main.o: queue.h sem.h
queue.o: queue.h
sem.o: sem.h

.PHONY = clean
clean:
				rm lab3 $(OBJS)
