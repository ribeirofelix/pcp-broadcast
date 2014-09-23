brdcst: broadcast.o main.c
	gcc -o brdcst broadcast.o main.c -pthread
broadcast.o: broadcast.c
	gcc -c -o broadcast.o broadcast.c
clean: rm *.o & rm brdcst 
