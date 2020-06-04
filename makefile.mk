a.out : functions.o main.o
	gcc functions.o main.o -o a.out -lm
functions.o : functions.c server.h
	gcc -c functions.c server.h 
main.o : main.c server.h
	gcc -c main.c server.h 
clean :
	rm -f *.o*.h.gch a.out
