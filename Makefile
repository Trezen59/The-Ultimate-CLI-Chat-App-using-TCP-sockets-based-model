CC = gcc
MY_C_FLAGS=-Wall -ggdb3
TARGET=server client

all: $(TARGET)

server: server.o common.o
	$(CC) $(MY_C_FLAGS) $^ -o $@ -lpthread

client: client.o common.o
	$(CC) $(MY_C_FLAGS) $^ -o $@ -lpthread

%.o: %.c
	$(CC) $(MY_C_FLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o
	rm ServerFiles/fromClient.txt ClientFiles/fromServer.txt
