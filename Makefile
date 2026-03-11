flappy_B.tos: flappy_B.o asynch.o bird.o cond.o model.o score.o pipepair.o synch.o renderer.o input.o
	cc68x -g flappy_B.o asynch.o bird.o cond.o model.o score.o pipepair.o synch.o renderer.o input.o -o flappy_B.tos 

test.tos: test.o asynch.o bird.o cond.o model.o score.o pipepair.o synch.o
	cc68x -g test.o asynch.o bird.o cond.o model.o score.o pipepair.o synch.o -o test.tos

rndertst.tos: rnderTST.o bird.o model.o score.o pipepair.o synch.o raster.o renderer.o
	cc68x -g rnderTST.o bird.o model.o score.o pipepair.o synch.o raster.o renderer.o -o rndertst.tos

rstertst.tos: rsterTST.o raster.o
	cc68x -g rsterTST.o raster.o -o rstertst.tos

results.txt: test.tos
	test.tos > results.txt

flappy_B.o: flappy_B.c model.h asynch.h synch.h cond.h input.h
	cc68x -g -c flappy_B.c

test.o: test.c model.h asynch.h synch.h cond.h
	cc68x -g -c test.c

rnderTST.o: rnderTST.c model.h synch.h raster.h renderer.h
	cc68x -g -c rnderTST.c

rsterTST.o: rsterTST.c raster.h
	cc68x -g -c rsterTST.c

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

renderer.o: renderer.c renderer.h model.h
	cc68x -g -c renderer.c

input.o: input.c input.h
	cc68x -g -c input.c
