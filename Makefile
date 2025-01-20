target=server client
normal: $(target)

MY_C_FLAGS=-Wall -ggdb3

client: client.c
	gcc $(MY_C_FLAGS) client.c -o client -lpthread

server: server.c
	gcc $(MY_C_FLAGS) server.c -o server -lpthread

clean:
	rm  $(target)
	rm ServerFiles/fromClient.txt ClientFiles/fromServer.txt
