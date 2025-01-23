#ifndef LIBCLIENT_H
#define LIBCLIENT_H

#define SERVER_IP_ADDR "127.0.0.1"

#define MAJOR_VERSION "1"
#define MINOR_VERSION "0"
#define PATCH_VERSION "6"
#define CLIENT_VERSION MAJOR_VERSION"."MINOR_VERSION"."PATCH_VERSION

#define CLIENT_DEBUG_PRINTS 0

volatile int keep_running = 1;

enum MENU {
    CHAT = 1,
    SEND_FILE,
    RECIEVE_FILE,
    EXIT
};

#endif // LIBCLIENT_H
