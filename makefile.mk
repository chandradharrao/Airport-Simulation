a.out:client.o server.o
	gcc -o a.out client.o server.o
client.o:client.c server.h
	gcc -c client.c server.h
server.o:server.c server.h
	gcc -c server.c server.h
