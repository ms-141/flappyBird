prog.tos: main.o asynch.o bird.o cond.o model.o score.o set_of_pipes.o synch.o
	cc68x -g main.o asynch.o bird.o cond.o model.o score.o set_of_pipes.o synch.o -o prog.tos

main.o: main.c model.h asynch.h synch.h cond.h
	cc68x -g -c main.c

asynch.o: asynch.c asynch.h model.h
	cc68x -g -c asynch.c

bird.o: bird.c bird.h
	cc68x -g -c bird.c

cond.o: cond.c cond.h model.h bird.h set_of_pipes.h score.h
	cc68x -g -c cond.c

model.o: model.c model.h bird.h set_of_pipes.h score.h
	cc68x -g -c model.c

score.o: score.c score.h
	cc68x -g -c score.c

set_of_pipes.o: set_of_pipes.c set_of_pipes.h
	cc68x -g -c set_of_pipes.c

synch.o: synch.c synch.h model.h
	cc68x -g -c synch.c

clean:
	rm -f *.o prog.tos
