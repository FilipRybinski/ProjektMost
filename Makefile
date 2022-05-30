program: main.o
	gcc main.o -o program -lpthread -lrt
program2: mainn.o
	gcc mainn.o -o program2 -lpthread -lrt
program3: mainnn.o
	gcc mainnn.o -o program3 -lpthread -lrt