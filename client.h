#define _GNU_SOURCE
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX 100
#define PORT1 8080
#define PORT2 8081

#define MAJOR_VERSION "1"
#define MINOR_VERSION "0"
#define PATCH_VERSION "1"
#define CLIENT_VERSION MAJOR_VERSION"."MINOR_VERSION"."PATCH_VERSION
