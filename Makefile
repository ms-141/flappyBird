CC = cc68x
CFLAGS = -g
TARGET = prog.tos
OBJS = main.o asynch.o bird.o cond.o model.o score.o set_of_pipes.o synch.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c model.h asynch.h synch.h cond.h
	$(CC) $(CFLAGS) -c main.c

asynch.o: asynch.c asynch.h model.h
	$(CC) $(CFLAGS) -c asynch.c

bird.o: bird.c bird.h
	$(CC) $(CFLAGS) -c bird.c

cond.o: cond.c cond.h model.h bird.h set_of_pipes.h score.h
	$(CC) $(CFLAGS) -c cond.c

model.o: model.c model.h bird.h set_of_pipes.h score.h
	$(CC) $(CFLAGS) -c model.c

score.o: score.c score.h
	$(CC) $(CFLAGS) -c score.c

set_of_pipes.o: set_of_pipes.c set_of_pipes.h
	$(CC) $(CFLAGS) -c set_of_pipes.c

synch.o: synch.c synch.h model.h
	$(CC) $(CFLAGS) -c synch.c

clean:
	rm -f *.o $(TARGET)
