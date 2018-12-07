OBJC = main.0 queue.o sem.o

main.o: main.c queue.h sem.h
				cc -c main.c

gueue.o: queue.c sem.h
				cc -c queue.c

sem.o: sem.c
				cc -c sem.c

clean:
				rm edit $(OBJC)
