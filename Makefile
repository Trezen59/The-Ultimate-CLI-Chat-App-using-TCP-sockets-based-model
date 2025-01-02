target=server client server.o client.o
normal: $(target)
client: client.c
	gcc -Wall client.c -o client -lpthread
	#gcc -c -Wall client.c -lpthread
server: server.c
	gcc -Wall server.c -o server -lpthread
	#gcc -c -Wall server.c -lpthread
clean:
	rm  $(target)
