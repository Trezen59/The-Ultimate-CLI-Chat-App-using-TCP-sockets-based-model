#ifndef LIBSERVER_H
#define LIBSERVER_H

/* for server in server app */
#define SERVER_IP_ADDR "127.0.0.1"

/* Server app version */
#define MAJOR_VERSION "1"
#define MINOR_VERSION "0"
#define PATCH_VERSION "8"
#define SERVER_VERSION MAJOR_VERSION"."MINOR_VERSION"."PATCH_VERSION

/* Server app debug */
#define SERVER_DEBUG_PRINTS 0

/* Enum for loop state */
enum LOOP_STATE {
   LOOP_STOP = 0,
   LOOP_START
};

/* Flag to keep the reading and writing in sync in server app */
volatile int keep_running = LOOP_START;

/* struct for server data */
typedef struct{
	int mainServerFD;
	int mainClientFD;
	int secondaryClientFD;
}SERVER_DATA;

/* For switch case in menu */
enum MENU {
	CHAT = 1,
	SHOW_FILES_INFO,
	RECIEVE_FILE,
	SEND_FILE,
	EXIT
};

#endif // LIBSERVER_H
