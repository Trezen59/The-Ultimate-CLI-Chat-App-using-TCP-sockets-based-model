#include "../libs/libCommon.h"

/* Func to create socket */
int create_socket()
{
	int sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		printf("socket creation failed... exit code %d\n", sockfd);
		return sockfd;
	} else {
		printf("Socket successfully created.. FD: %d\n", sockfd);
		return sockfd;
	}
}

/* Menu fucntion */
void showMenu()
{
	/* display menu and get choice */
	printf("\nWelcome to the Ultimate Chat App.\n");
	printf("\n1) Chat.");
	printf("\n2) Send File.");
	printf("\n3) Recieve File.");
	printf("\n4) Exit.\n");
}

/* Func to print received msg in a box */
void printMessageInBox(const char *message) {
	int length = strlen(message);
	/* 2 spaces for padding on each side, plus 2 for the borders */
	int boxWidth = length + 4;

	/* Print top border */
	printf("\t\t\t");
	for (int i = 0; i < boxWidth; i++) {
		printf("-");
	}
	printf("\n");

	/* Print message with side borders */
	printf("\t\t\t");
	printf("| %s |\n", message);

	/* Print bottom border */
	printf("\t\t\t");
	for (int i = 0; i < boxWidth; i++) {
		printf("-");
	}
	printf("\n");
}

/* Get input for the menu */
int getIntInput(const char *prompt)
{
	/* Buffer to store user input */
	char input[100];
	int value;
	while (1) {
		/* Show prompt to the user */
		printf("%s", prompt);
		if (fgets(input, sizeof(input), stdin) == NULL) {
			printf("Error reading input. Please try again.\n");
			continue;
		}

		/* Remove trailing newline character, if any */
		input[strcspn(input, "\n")] = '\0';

		/* Try to parse the integer */
		if (sscanf(input, "%d", &value) == 1) {
			return value;
		} else {
			printf("Invalid input. Please enter a valid integer.\n");
		}
	}
}
