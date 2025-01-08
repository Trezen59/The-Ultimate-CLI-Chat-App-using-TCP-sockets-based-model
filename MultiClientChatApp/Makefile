target=serverMultiClient MultiClient serverMultiClient.o MultiClient.o
normal: $(target)
MultiClient: MultiClient.c
	gcc -Wall MultiClient.c -o client -lpthread
	#gcc -c -Wall MultiClient.c -lpthread
serverMultiClient: serverMultiClient.c
	gcc -Wall serverMultiClient.c -o server -lpthread
	#gcc -c -Wall serverMultiClient.c -lpthread
clean:
	rm  server client serverMultiClient.o MultiClient.o
