#include "client.h"

void *reading(void *server_data)
{
    int flag = 1;
    int ret = -1;
    char buff[MAX_CHAT_MSG_LEN] = {0};

    /* Infinit Loop */
    while(flag)
    {
        bzero(buff, MAX_CHAT_MSG_LEN);

        /* read data into buff */
        ret = read(((SERVER_DATA *)server_data)->mainClientFD, buff, MAX_CHAT_MSG_LEN);
        if(ret < 0)
        {
            perror("read :");
            break;
        }
        printf("\n\t\t\t %s\n", buff);

        /* compare and check for "bye" to exit chat */
        if(!(strncmp("bye", buff , 3)))
        {
            flag = 0;
            break;
        }
    }
    pthread_exit(NULL);
}

void *writing(void *server_data)
{
    int flag = 1;
    int ret = -1;
    char buff[MAX_CHAT_MSG_LEN] = {0};

    while(flag)
    {
        int i = 0;
        printf("\n");

        bzero(buff, MAX_CHAT_MSG_LEN);

        /* get the message and store it in buffer */
        while (( buff[i++] = getchar() ) != '\n' || i == MAX_CHAT_MSG_LEN)
        buff[i] = '\0';

        /* write the content */
        ret = write(((SERVER_DATA *)server_data)->secondaryClientFD, buff, MAX_CHAT_MSG_LEN);
        if(ret < 0)
        {
            perror("write : ");
            break;
        }

        /* compare msg for "bye" to exit chat */
        if(!(strncmp("bye", buff , 3)))
        {
            flag = 0;
            break;
        }
    }
    pthread_exit(NULL);
}

int create_socket()
{
	int sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		printf("socket creation failed...\n");
		return sockfd;
	}
	else
	{
		printf("Socket successfully created..\n");
		return sockfd;
	}
}

int setup_server(int server_fd,
                int *connfd,
                struct sockaddr_in *servaddr,
                struct sockaddr_in *cli,
                socklen_t *len)
{
    int ret = -1;
    int reuse1;

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
        printf("socket bind failed... ret: %d\n", ret);
        return ret;
    }
    else
        printf("Socket successfully binded..\n");

    /* server listens */
    ret = listen(server_fd, 5);
    if (ret != 0) {
        printf("Listen failed...\n");
        return ret;
    }
    else
        printf("Server listening..\n");

    *len = sizeof(cli);

    /* Accept connection from client */
    *connfd = accept(server_fd, (struct sockaddr*)&cli, len);
    if (*connfd < 0) {
        printf("server accept failed...\n");
        return *connfd;
    }
    else{
        printf("server accepted the client...\n");
    }

    return 1;
}

int setup_client(int client_fd,
                struct sockaddr_in *cliaddr)
{
    int reuse = 1;
    int ret = -1;

    /* to reuse socket with same address */
    ret = setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));
    if (ret < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
        return ret;
    }

    bzero(cliaddr, sizeof(*cliaddr));

    /* address structure */
    cliaddr->sin_family = AF_INET;
    cliaddr->sin_addr.s_addr = inet_addr("127.0.0.1");
    cliaddr->sin_port = htons(PORT1);

    /* connect to server */
    ret = connect(client_fd, (struct sockaddr *)cliaddr, sizeof(*cliaddr));
    if(ret < 0)
    {
        perror("connect");
        return ret;
    }
    else
        printf("connected to socket\n");

    return 0;
}

int multiThreadedChatFunction(SERVER_DATA *server_data,
                                pthread_t *readThreadID,
                                pthread_t *writeThreadID)
{
    int ret = -1;

    printf("\n\n  *********** Chat APP started **********\n\n");
    printf("\nsend 'bye' to exit chat app\n");

    /* creating threads by default joinable */
    ret = pthread_create(readThreadID, NULL, reading, (void *)server_data);
    if(ret != 0)
    {
        perror("thread1");
        return -1;
    }
    ret = pthread_create(writeThreadID, NULL, writing, (void *)server_data);
    if(ret != 0)
    {
        perror("thread2");
        return -1;
    }

    /* Joining the threads */
    pthread_join(*readThreadID, NULL);
    pthread_join(*writeThreadID, NULL);

    return 0;
}

int main()
{
	printf("Client App version: %s\n", CLIENT_VERSION);

    int ret = -1;
    int serverFD = -1;
    int connectedClientFD = -1;
    int internalClientFD = -1;
    socklen_t len = -1;
    struct sockaddr_in servaddr, connectedClientAddr;
    struct sockaddr_in internalClientAddr;
    char choice_buff[MAX];
    int choice = 1;
    SERVER_DATA *serverData;

    serverData = (SERVER_DATA *) malloc (sizeof(SERVER_DATA));
    if (serverData == NULL){
        printf("Cannot allocate memory for serverData\n");
        exit(1);
    }

    /* Threading implementation */
    pthread_t readingThread, writingThread;

    /* create server socket */
    serverFD = create_socket();
    if (serverFD < 0){
        printf("Failed creating server FD\n");
        exit(1);
    }

    /* create client socket */
    internalClientFD = create_socket();
    if (internalClientFD < 0){
        printf("Failed creating internal client FD\n");
        if (serverFD)
            close(serverFD);
        exit(1);
    }

    /* client setup */
    ret = setup_client(internalClientFD, &internalClientAddr);
    if (ret < 0) {
        if (serverFD)
            close(serverFD);
        if (connectedClientFD)
            close(connectedClientFD);
        exit(1);
    }

    /* server setup */
    ret = setup_server(serverFD, &connectedClientFD, &servaddr, &connectedClientAddr, &len);
    if (ret < 0) {
        if (serverFD)
            close(serverFD);
        if (connectedClientFD)
            close(connectedClientFD);
        exit(1);
    }

    serverData->mainServerFD = serverFD;
    serverData->mainClientFD = connectedClientFD;
    serverData->secondaryClientFD = internalClientFD;

	/* Infinite Loop */
	do
	{
		char *c;
		bzero(choice_buff, MAX);

		/* display menu and get choice */
		printf("\n\n1) Chat.\n2) Send File.\n3) Recieve File.\n4) Exit.\n\n Please enter a choice : ");
		scanf("%d", &choice);

		asprintf(&c, "%d", choice);
		strcat(choice_buff, c);

		write(internalClientFD, choice_buff, sizeof(choice));

        switch(atoi(choice_buff))
        {
            case CHAT:
                ret = multiThreadedChatFunction(serverData,
                        &readingThread,
                        &writingThread);
                if (ret < 0) {
                    printf("Chat functionality exited with code %d\n", ret);
                    if (connectedClientFD)
                        close(connectedClientFD);
                    if (serverFD)
                        close(serverFD);
                    if (internalClientFD)
                        close(internalClientFD);
					return 0;
                }

                printf("Chat ended\n\n");
                break;

            case SEND_FILE:
                printf("\nsending file\n");
                //sendfile(internalClientFD);
                break;

            case RECIEVE_FILE:
                printf("recieving file..\n");
                //recvfile(connectedClientFD);
                break;

            case EXIT:
                printf("\nThank you for using this App.\n\n");
                if (connectedClientFD)
                    close(connectedClientFD);
                if (serverFD)
                    close(serverFD);
                if (internalClientFD)
                    close(internalClientFD);
				return 0;

            default:
				printf("\nPlease select a valid choice\n");
        }
    }
    while(1);

    if (connectedClientFD)
		close(connectedClientFD);
    if (serverFD)
		close(serverFD);
    if (internalClientFD)
		close(internalClientFD);

	return 0;
}
