#ifndef LIBCOMMON_H
#define LIBCOMMON_H

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

#define MAX						100
#define PORT1					8080
#define PORT2					8081
#define MAX_CHAT_MSG_LEN		1000

#define HEARTBEAT_MSG			"HEARTBEAT"
#define CHAT_TERMINATION_MSG	"bye"

/* func declarations */
int create_socket();
void showMenu();
void printMessageInBox(const char *);
int getIntInput(const char *);

#endif // LIBCOMMON_H
