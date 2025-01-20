#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>

#define MAX 100
#define PORT1 8080
#define PORT2 8081
#define MAX_CHAT_MSG_LEN 1000

#define SERVER_IP_ADDR "127.0.0.1"

#define MAJOR_VERSION "1"
#define MINOR_VERSION "0"
#define PATCH_VERSION "5"
#define SERVER_VERSION MAJOR_VERSION"."MINOR_VERSION"."PATCH_VERSION

#define SERVER_DEBUG_PRINTS 0

enum MENU {
	CHAT = 1,
	RECIEVE_FILE,
	SEND_FILE,
	EXIT
};

typedef struct{
	int mainServerFD;
	int mainClientFD;
	int secondaryClientFD;
}SERVER_DATA;

