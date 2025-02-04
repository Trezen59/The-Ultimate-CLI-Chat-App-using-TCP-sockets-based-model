/*

Author: Trezen Parmar
E-MAIL: trezen7984891023@gmail.com

DESCRIPTION:

This app is a server-client app which uses sockets mainly for communication.
This is a bidirectional app in which both participants can send and receive
messages simultaneously. It also has a menu based approach to select the
appropriate options.

USE:
1. Compile the server and client app using the command "$make"
2. Run the server "$./server" and then run the client "$./client".
3. Select an option from the Menu shown.
4. Enjoy the chat.
5. Follow the instructions.

*/

#include "../libs/libClient.h"
#include "../libs/libCommon.h"

CLIENT_DATA *gdata = NULL;

/* signal handler to handle abrupt termination of app */
void signal_handler(int sig){
	printf("\n Client received signal %d. Shutting down... \n", sig);

	keep_running = LOOP_STOP;

	if (gdata != NULL){
		free(gdata);
		gdata = NULL;
	}

	exit(EXIT_FAILURE);
}

int showFileInfo(int cliFD)
{
	int ret = -1;
	char fileNames[MAX] = {0};

	ret = recv(cliFD, fileNames, MAX, 0);
	if (ret < 0){
		printf("Failed receiving file names from server.\n");
		return EXIT_FAILURE;
	}

	printf("Files on server location:\n%s\n", fileNames);

	return 0;
}

/* Func to send file to server */
int sendFileToServer(int cliFD)
{
	int ret = -1;
	int fileFD = -1;
	char fileLocation[MAX] = "/home/einfochips/users/trezen/files/server-client-enhanced/ClientFiles/";
	char fileName[MAX] = "toServer.txt";
	struct stat st;

	strcat(fileLocation, fileName);

	/* check if file exists */
	ret = access(fileLocation, F_OK | R_OK);
	if(ret < 0){
		printf("file '%s' not found\n", fileName);
		return -1;
	}
	if (CLIENT_DEBUG_PRINTS)
		printf("file is found and can be read..\n");

	fileFD = open((char *)fileLocation, O_RDONLY);
	if(fileFD < 0){
		printf("error in opening fd\n");
		return -1;
	}

	/* get file size */
	stat(fileLocation, &st);
	int *size = malloc(sizeof(int));
	*size = st.st_size;

	/* init buffer with file size */
	char *buff = (char *)malloc(*size);
	bzero(buff, *size);

	if (CLIENT_DEBUG_PRINTS)
		printf("size of file is %d\n", *size);

	/* send size of file */
	write(cliFD, size, sizeof(int));

	/* store contents of file into buffer */
	int len = read(fileFD, buff, *size);

	if (CLIENT_DEBUG_PRINTS)
		printf("lenth read = %d\n", len);
	if (CLIENT_DEBUG_PRINTS)
		printf("file contents : %s\n", buff);

	/* send file */
	ret = write(cliFD, buff, *size);
	if(ret < 0){
		printf("write unsuccessful\n");
		return -1;
	} else {
		printf("======== write successful. File sent ========\n");
	}

	bzero(buff, *size);
	free(size);
	free(buff);
	return 0;
}

/* Func to receive file from server */
int receiveFileFromServer(int connFD)
{
	int ret = -1;
	int fileFD = -1;
	char fileLocation[MAX] = "/home/einfochips/users/trezen/files/server-client-enhanced/ClientFiles/";
	char fileName[MAX] = "fromServer.txt";

	/* init size to store size of file */
	int *size = malloc(sizeof(int));
	bzero(size, sizeof(int));

	strcat(fileLocation, fileName);

	/* check if file exists at a given location */
	ret = access(fileLocation, F_OK | R_OK);
	if (ret >= 0){
		printf("file already found\n\n");
	}

	/* open and create file if file not available */
	fileFD = open((char *)fileLocation, O_RDWR | O_CREAT);
	if(fileFD < 0){
		printf("error in opening fd\n");
		return -1;
	}

	/* get file size */
	ret = read(connFD, size, sizeof(int));
	if (ret < 0)
		perror("read");
	else {
		if (CLIENT_DEBUG_PRINTS)
			printf("read length success\n");
	}

	if (CLIENT_DEBUG_PRINTS)
		printf("size recieved is %d\n", *size);

	char *buff = (char *)malloc(*size);
	bzero(buff, *size);

	/* read file into buff */
	ret = read(connFD, buff, *size);
	if(ret < 0){
		printf("read unsuccessful\n");
		return -1;
	}

	if (CLIENT_DEBUG_PRINTS)
		printf("buffer value after read is : %s", buff);

	/* write the contents of buff to file */
	int len = write(fileFD, buff, *size);
	if(len < 0){
		printf("write unsuccessful\n");
	} else {
		printf("======== written to file ========\n");
		if (CLIENT_DEBUG_PRINTS)
			printf("buffer content after write : %s", buff);
	}
	bzero(buff, *size);
	free(size);
	free(buff);
	return 0;
}

void *handleChatThread(void *client_data)
{
	int ret = -1;
	int max_fd = -1;
	char buff[MAX_CHAT_MSG_LEN] = {0};
	CLIENT_DATA *data = (CLIENT_DATA *)client_data;
	int mainClientFD = data->mainClientFD;
	int secondaryClientFD = data->secondaryClientFD;

	struct timeval timeout;

	time_t last_heartbeat_sent = time(NULL);
	time_t last_heartbeat_received = time(NULL);

	fd_set read_fds;
	max_fd = mainClientFD > STDIN_FILENO ? mainClientFD : STDIN_FILENO;

	/* Infinit Loop */
	while(keep_running)
	{
		FD_ZERO(&read_fds);
		/*Monitor stdin for user input */
		FD_SET(STDIN_FILENO, &read_fds);
		/* Monitor the client FD */
		FD_SET(mainClientFD, &read_fds);

		/* Set timeout for select */
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		bzero(buff, MAX_CHAT_MSG_LEN);

		ret = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
		if (ret < 0) {
			perror("select");
			keep_running = LOOP_STOP;
			break;
		}
		else if (ret == 0) {
			printf("%s: Timeout.\n", __func__);
			time_t now = time(NULL);

			/* Send heartbeat if interval has elapsed */
			if (difftime(now, last_heartbeat_sent) >= 15) {
				if (write(mainClientFD, HEARTBEAT_MSG, 9) < 0) {
					printf("Failed to send the heartbeat\n");
					keep_running = LOOP_STOP;
					break;
				}
				/* Update last sent time */
				if (CLIENT_DEBUG_PRINTS)
					printf("Sending heartbeat...\n");
				last_heartbeat_sent = now;
			}

			if (difftime(now, last_heartbeat_received) >= 20) {
				printf("Hearbeat timeout. Server disconnected !\n");
				keep_running = LOOP_STOP;
				break;
			}
			continue;
		}

		/* If data available on client FD */
		if (FD_ISSET(mainClientFD, &read_fds)) {
			/* read data into buff */
			ret = read(mainClientFD, buff, MAX_CHAT_MSG_LEN);
			if(ret < 0){
				printf("Error in reading, ERROR:%d.\n", ret);
				keep_running = LOOP_STOP;
				break;
			} else if (ret == 0) {
				printf("server Disconnected.\n");
				close(mainClientFD);
				keep_running = LOOP_STOP;
				pthread_exit((void *)-1);
				break;
			}

			/* Remove the new line char from end of msg */
			if (strlen(buff) > 0 && (buff[strlen(buff) - 1] == '\n')) {
				buff[strlen(buff) - 1] = '\0';
			}

			printMessageInBox(buff);

			/* Update the last received hearbeat */
			if((strncmp(HEARTBEAT_MSG, buff, 9))){
				if (CLIENT_DEBUG_PRINTS)
					printf("Received heartbeat...\n");
				last_heartbeat_received = time(NULL);
			}

			/* compare and check for "bye" to exit chat */
			if(!(strncmp(CHAT_TERMINATION_MSG, buff , 3))){
				keep_running = LOOP_STOP;
				break;
			}
		}

		/* Check if stdin has input */
		if (FD_ISSET(STDIN_FILENO, &read_fds)) {
			bzero(buff, MAX_CHAT_MSG_LEN);

			/* Read user input */
			if (fgets(buff, sizeof(buff), stdin) == NULL) {
				keep_running = LOOP_STOP;
				break;
			}
			printf("\n");

			/* Send the input to the client */
			ret = write(secondaryClientFD, buff, strlen(buff));
			if (ret < 0) {
				keep_running = LOOP_STOP;
				perror("write");
				break;
			}

			/* Check if the user entered "bye" to exit the chat */
			if (strncmp(CHAT_TERMINATION_MSG, buff, 3) == 0) {
				keep_running = LOOP_STOP;
				break;
			}
		}
	}
	pthread_exit(NULL);
}

/* Func to setup the server */
int setup_server(int server_fd,
				int *connfd,
				struct sockaddr_in *servaddr,
				struct sockaddr_in *cli,
				socklen_t *len)
{
	int ret = -1;
	int reuse1;

	printf("\nStarting server setup...\n");

	/* to reuse socket with same address */
	reuse1 = 1;
	ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
					(const char *)&reuse1, sizeof(reuse1));
	if (ret < 0){
		perror("setsockopt(SO_REUSEADDR) failed");
		return ret;
	}

	/* clear the servaddr */
	memset(servaddr, '\0', sizeof(*servaddr));

	/* assign IP, PORT */
	servaddr->sin_family = AF_INET;
	/* long integer from host byte order to network byte order. */
	servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
	/* short integer from host byte order to network byte order. */
	servaddr->sin_port = htons(PORT2);

	/* Binding newly created socket to given IP and verification */
	ret = bind(server_fd, (struct sockaddr*)servaddr, sizeof(*servaddr));
	if (ret != 0) {
		printf("Server: socket bind failed... ret: %d\n", ret);
		return ret;
	} else {
		printf("Server: socket binded to PORT:%d\n", PORT2);
		printf("Server: socket successfully binded..\n");
	}

	/* server listens */
	ret = listen(server_fd, 5);
	if (ret != 0) {
		printf("Server: Listen failed...\n");
		return ret;
	} else {
		printf("Server: listening..\n");
	}

	*len = sizeof(cli);

	/* Accept connection from client */
	*connfd = accept(server_fd, (struct sockaddr*)&cli, len);
	if (*connfd < 0) {
		printf("Server: failed to accept client req...\n");
		return *connfd;
	}
	else{
		printf("Server: accepted the client connection...\n");
	}

	return 0;
}

/* Func to setup the client */
int setup_client(int client_fd,
				struct sockaddr_in *cliaddr)
{
	int reuse = 1;
	int ret = -1;

	printf("\nStarting client setup...\n");

	/* to reuse socket with same address */
	ret = setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));
	if (ret < 0){
		perror("setsockopt(SO_REUSEADDR) failed");
		return ret;
	}

	bzero(cliaddr, sizeof(*cliaddr));

	/* address structure */
	cliaddr->sin_family = AF_INET;
	cliaddr->sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
	cliaddr->sin_port = htons(PORT1);

	/* connect to server */
	ret = connect(client_fd, (struct sockaddr *)cliaddr, sizeof(*cliaddr));
	if(ret < 0){
		perror("connect");
		return ret;
	} else {
		printf("Client: connected to socket on PORT: %d, IP:%s\n", PORT1, SERVER_IP_ADDR);
	}

	return 0;
}

/* Multi-threaded chat func */
int multiThreadedChatFunction(CLIENT_DATA *client_data,
								pthread_t *ThreadID)
{
	int ret = -1;
	int *retval = NULL;

	printf("\n================= Bidirectional Chat Started ================\n");
	printf("NOTE: send 'bye' to exit chat app\n\n");

	/* creating threads by default joinable */
	ret = pthread_create(ThreadID, NULL, handleChatThread, (void *)client_data);
	if(ret != 0){
		perror("thread1");
		return -1;
	}

	/* Joining the threads */
	pthread_join(*ThreadID, (void *)&retval);

	if (retval == (void*)-1){
		printf("Thread exited because server disconnected.\n");
		return -1;
	}

	printf("\n================= Bidirectional Chat ended ================\n");
	return 0;
}

int main()
{
	printf("Client App version: %s\n", CLIENT_VERSION);

	int ret = -1;
	int serverFD = -1;
	int connectedClientFD = -1;
	int internalClientFD = -1;
	int running = 1;
	socklen_t len = -1;
	struct sockaddr_in servaddr, connectedClientAddr;
	struct sockaddr_in internalClientAddr;
	char choice_buff[MAX];
	CLIENT_DATA *clientData;

	/* Signal handler to handle abrupt code exit with ctrl+c */
	signal(SIGINT, signal_handler);

	clientData = (CLIENT_DATA *) malloc (sizeof(CLIENT_DATA));
	if (clientData == NULL){
		printf("Cannot allocate memory for clientData\n");
		exit(EXIT_FAILURE);
	}

	gdata = clientData;

	/* Threading implementation */
	pthread_t Thread;

	/* create server socket */
	serverFD = create_socket();
	if (serverFD < 0){
		printf("Failed creating server FD\n");
		if (clientData != NULL)
			free(clientData);
		exit(EXIT_FAILURE);
	}

	/* create client socket */
	internalClientFD = create_socket();
	if (internalClientFD < 0){
		printf("Failed creating internal client FD\n");
		if (serverFD >= 0)
			close(serverFD);
		if (clientData != NULL)
			free(clientData);
		exit(EXIT_FAILURE);
	}

	/* client setup */
	ret = setup_client(internalClientFD, &internalClientAddr);
	if (ret < 0) {
		printf("Client: Setup Failed!\n");
		if (serverFD >= 0)
			close(serverFD);
		if (connectedClientFD >= 0)
			close(connectedClientFD);
		if (clientData != NULL)
			free(clientData);
		exit(EXIT_FAILURE);
	}
	printf("Client: Setup Complete!\n");

	/* server setup */
	ret = setup_server(serverFD, &connectedClientFD, &servaddr, &connectedClientAddr, &len);
	if (ret < 0) {
		printf("Server: Setup Failed!\n");
		if (serverFD >= 0)
			close(serverFD);
		if (connectedClientFD >= 0)
			close(connectedClientFD);
		if (clientData != NULL)
			free(clientData);
		exit(EXIT_FAILURE);
	}
	printf("Server: Setup Complete!\n");

	clientData->mainServerFD = serverFD;
	clientData->mainClientFD = connectedClientFD;
	clientData->secondaryClientFD = internalClientFD;

	/* Infinite Loop */
	do
	{
		int choice = -1;
		bzero(choice_buff, MAX);

		showMenu();
		choice = getIntInput("\nPlease enter a choice. ");

		if (CLIENT_DEBUG_PRINTS)
			printf("Received %d as input\n", choice);

		snprintf(choice_buff, MAX, "%d", choice);
		send(internalClientFD, choice_buff, strlen(choice_buff), 0);

		switch(choice)
		{
			case CHAT:

				/* Re-initialize the threads flag to run the chat app again */
				keep_running = LOOP_START;
				ret = multiThreadedChatFunction(clientData,
						&Thread);
				if (ret < 0) {
					printf("Chat functionality exited with code %d\n", ret);
					running = 0;
					break;
				}
				break;

			case SHOW_FILES_INFO:

				printf("\nShowing all files information located on server.\n");
				ret = showFileInfo(connectedClientFD);
				if (ret < 0) {
					printf("Failed showing file info.\n");
					running = 0;
					break;
				}
				break;

			case SEND_FILE:

				printf("\n\n======== Sending file to Server: ========\n\n");
				ret = sendFileToServer(internalClientFD);
				if (ret < 0) {
					printf("Chat functionality exited with code %d\n", ret);
					running = 0;
					break;
				}
				break;

			case RECIEVE_FILE:

				printf("\n\n======== Recieving file from server: ========\n\n");
				ret = receiveFileFromServer(connectedClientFD);
				if (ret < 0) {
					printf("Chat functionality exited with code %d\n", ret);
					running = 0;
					break;
				}
				break;

			case EXIT:

				printf("\nThank you for using this App.\n\n");
				running = 0;
				break;

			default:
				printf("\nPlease select a valid choice\n");
		}
	}
	while(running);

	if (connectedClientFD >= 0)
		close(connectedClientFD);
	if (serverFD >= 0)
		close(serverFD);
	if (internalClientFD >= 0)
		close(internalClientFD);
	if (clientData != NULL)
		free(clientData);

	return 0;
}
