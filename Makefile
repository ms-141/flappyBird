prog.tos: main.o asynch.o bird.o cond.o model.o score.o pipepair.o synch.o raster.o
	cc68x -g main.o asynch.o bird.o cond.o model.o score.o pipepair.o synch.o raster.o -o prog.tos

main.o: main.c model.h asynch.h synch.h cond.h
	cc68x -g -c main.c

asynch.o: asynch.c asynch.h model.h
	cc68x -g -c asynch.c

bird.o: bird.c bird.h
	cc68x -g -c bird.c

cond.o: cond.c cond.h model.h bird.h pipepair.h score.h
	cc68x -g -c cond.c

model.o: model.c model.h bird.h pipepair.h score.h
	cc68x -g -c model.c

score.o: score.c score.h
	cc68x -g -c score.c

pipepair.o: pipepair.c pipepair.h
	cc68x -g -c pipepair.c

synch.o: synch.c synch.h model.h
	cc68x -g -c synch.c

raster.o: raster.c raster.h
	cc68x -g -c raster.c