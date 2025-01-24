CC = gcc
MY_C_FLAGS = -Wall -ggdb3
SOURCE_FILES_DIR = sourceFiles/
COMMON_SRC = $(SOURCE_FILES_DIR)common.c
SERVER_SRC = $(SOURCE_FILES_DIR)server.c
CLIENT_SRC = $(SOURCE_FILES_DIR)client.c

# Object files
SERVER_OBJ = server.o common.o
CLIENT_OBJ = client.o common.o

TARGET = server client

# Rule to compile .c files to .o files
# Necessary for incremental build. Saves time by not rebuilding source again
%.o: $(SOURCE_FILES_DIR)%.c
	$(CC) $(MY_C_FLAGS) -c $< -o $@

all: $(TARGET)

server: $(SERVER_OBJ)
	$(CC) $(MY_C_FLAGS) $(SERVER_OBJ) -o server -lpthread

client: $(CLIENT_OBJ)
	$(CC) $(MY_C_FLAGS) $(CLIENT_OBJ) -o client -lpthread

clean:
	rm -f $(TARGET) *.o  ClientFiles/fromServer.txt ServerFiles/fromClient.txt

