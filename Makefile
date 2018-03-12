CC=gcc
CFLAGS=
RM=rm -rf
OUT=server

all: server client1 client2

server: server.o vector.o 
	$(CC) $(CFLAGS)-g -Wall -pthread -o $(OUT) server.c vector.c -lrt 
	$(RM) *.o

client1: client1.o 
	gcc -g -Wall -pthread -o client1 client1.c -lrt -L. -lkeys
client2: client2.o 
	gcc -g -Wall -pthread -o client2 client2.c -lrt -L. -lkeys
debug: CFLAGS+=-DDEBUG_ON
debug: build

server.o: server.c vector.h
	$(CC) $(CFLAGS) -c server.c
	
client1.o: client1.c keys.c keys.h
	gcc -g -c -Wall -pthread -o keys -l keys.c 
	ar -cvq libkeys.a keys
	gcc -g -Wall -pthread -o client1 client1.c -lrt -L. -lkeys
client2.o: client2.c keys.c keys.h
	gcc -g -c -Wall -pthread -o keys -l keys.c 
	ar -cvq libkeys.a keys
	gcc -g -Wall -pthread -o client2 client2.c -lrt -L. -lkeys
	

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c

clean:
	$(RM) *.o $(OUT)
