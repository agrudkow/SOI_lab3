# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC = gcc                       # compiler to use
OBJS = main.0 queue.o sem.o

edit: $(OBJS)
				$(CC) -o edit $(OBJS)

main.o: queue.h sem.h

.PHONY = clean
clean:
				rm edit $(OBJS)
