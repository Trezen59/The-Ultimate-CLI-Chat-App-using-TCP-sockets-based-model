#ifndef LIBCLIENT_H
#define LIBCLIENT_H

/* for server in client app */
#define SERVER_IP_ADDR "127.0.0.1"

/* Client app version */
#define MAJOR_VERSION "1"
#define MINOR_VERSION "0"
#define PATCH_VERSION "7"
#define CLIENT_VERSION MAJOR_VERSION"."MINOR_VERSION"."PATCH_VERSION

/* Client app debug */
#define CLIENT_DEBUG_PRINTS 0

/* Enum for loop state */
enum LOOP_STATE {
   LOOP_STOP = 0,
   LOOP_START
};

/* Flag to keep the reading and writing in sync in client app */
volatile int keep_running = LOOP_START;

/* struct for client data */
typedef struct{
	int mainServerFD;
	int mainClientFD;
	int secondaryClientFD;
}CLIENT_DATA;

/* For switch case in menu */
enum MENU {
	CHAT = 1,
	SEND_FILE,
	RECIEVE_FILE,
	EXIT
};

#endif // LIBCLIENT_H
