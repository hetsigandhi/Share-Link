/*
Hetsi Gandhi|110122118|Section 2
Parth Patel|110124289|Section 2
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

/* 
Declaring global constants i.e.
Server IP Address
Server Port
Data buffer for transmitting information to the server
Maximum allowable size for a command string
The maximum number of arguments that can be passed by the client
*/
#define MAXIMUM_PATH_LENGTH 4096
#define SERVER_IP_ADDRESS "127.0.1.1"
#define MAXIMUM_ARGS 6
#define SERVER_PORT 3490
#define MAX_ARGSS 10
#define BUFF_SIZE 10000

/*
Implement a function that can retrieve a tar file from a server or mirror.
Create a new process by duplicating the existing one.
Data buffer for storing received information.
Assign a name to the temporary file.
Initialize a file in write mode.
Retrieve and record the contents of a file by processing each individual byte.
Number of bytes received throughout each repetition.
Retrieve data from the server and store it in the buffer.
Save the received data to the local file.
*/
void getTarForFile(int socketFileDescriptor) {
    int processIdd = fork();

    if (processIdd == 0) {
        char buffr[BUFF_SIZE];
        char tarFileName[40]; 
        sprintf(tarFileName, "temp.tar.gz");

        int fileDes = open(tarFileName, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fileDes < 0) {
            perror("FATAL ERROR!! Encountereed error while opening of file.");
            exit(EXIT_FAILURE);
        }

        ssize_t totByteRec = 1; 
        long int totatlBytesRec = 0;
        while (totByteRec > 0) {
            memset(buffr, 0, sizeof(buffr));

            totByteRec = recv(socketFileDescriptor, buffr, BUFF_SIZE, 0);
            if (totByteRec < 0) {
                perror("FATAL ERROR!! Encountereed error while reciving file.");
                exit(EXIT_FAILURE);
            }
			
            long int bytesWriten = write(fileDes, buffr, totByteRec);
            if (bytesWriten < 0) {
                perror("FATAL ERROR!! Encountereed error while writing the data.");
                exit(EXIT_FAILURE);
            }
            memset(buffr, 0, sizeof(buffr));
            totatlBytesRec += bytesWriten;
        }

        if (totatlBytesRec == 0) {
            printf("File not received.\n");
        } else {
            printf("File recived successfullyyyy.\n");
            printf("Totall numbeer of files recived are %d\n", totatlBytesRec);
        }
        close(fileDes);
        exit(0);
    } else {
        sleep(10);
        kill(processIdd, SIGKILL);
    }
}

/*
Establish a socket connection.
Configuration of the server address.
Endless iteration for communication between a client and server.
Check the server's load condition and transfer the connection to the mirror.
*/
int main() {
    int clientSocket;
    struct sockaddr_in serverAddres, mirrorAddres;
    char buffr[BUFF_SIZE];
    ssize_t totByteRec;
    int fileDes;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1){
        perror("FATAL ERROR!! Encountereed error while creating the socket.");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddres, 0, sizeof(serverAddres));
    serverAddres.sin_family = AF_INET;
    serverAddres.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP_ADDRESS, &serverAddres.sin_addr) == -1){
        perror("FATAL ERROR!! Encountereed error in inet_pton().");
        exit(EXIT_FAILURE);
    }
    if (connect(clientSocket, (struct sockaddr *)&serverAddres, sizeof(serverAddres)) == -1){
        perror("FATAL ERROR!! Encountereed error while connecting to the server.");
        exit(EXIT_FAILURE);
    }
    char comand[1000], serverMesage[2000];
    char *argumentss[MAXIMUM_ARGS];

    int argumentssCount;
    char mirrorInfo[BUFF_SIZE];
    ssize_t ipPortRec = recv(clientSocket, mirrorInfo, BUFF_SIZE, 0);

    printf("%s", mirrorInfo);
    if (strcmp(mirrorInfo, "Hi There!! Welcomee to the mirror!") == 0){
        close(clientSocket);
		int cc = 0;
		if(cc>100){}
        int mirrorSocket = socket(AF_INET, SOCK_STREAM, 0);
        memset(&mirrorAddres, 0, sizeof(mirrorAddres));
        mirrorAddres.sin_family = AF_INET;
        mirrorAddres.sin_addr.s_addr = inet_addr("127.0.0.1");
        mirrorAddres.sin_port = htons(8081);
        if (connect(mirrorSocket, (struct sockaddr *)&mirrorAddres, sizeof(mirrorAddres)) == -1){
            perror("FATAL ERROR!! Encountereed error while connecting to the mirror.");
            exit(EXIT_FAILURE);
        }
        printf("Connection to the mirrorr is successful.\n");
    }

    for(;;){
		int isComdValid = 0, filef = 0, valid = 0;
		char temp[1000];
		argumentssCount = 0;
        printf("\nC$: ");
        fgets(comand, 1000, stdin);
        sprintf(temp, "%s", comand);
        argumentss[argumentssCount] = strtok(temp, " \n");
		
		if (valid == 1 ){
		}
		
        while (argumentss[argumentssCount] != NULL){
            argumentssCount++;
            argumentss[argumentssCount] = strtok(NULL, " \n");
        }

        if (strcmp(argumentss[0], "getfz") == 0){
            isComdValid = 1;
            if (argumentssCount > 3 || argumentssCount < 3){
                fprintf("Usage: %s size1 size2 \n", argumentss[0]);
                isComdValid = 0;
            } else {
				long toSize = atol(argumentss[2]);
                long fromSize = atol(argumentss[1]);
                if (fromSize < 0 || fromSize > toSize || toSize < 0){
                    printf("Invalid inputs. Pleasee provide valid size of files.\n");
                    isComdValid = 0;
                }
            }
            if (isComdValid){
                filef = 1;
            }
        } else if (strcmp(argumentss[0], "getfda") == 0){
            isComdValid = 1;
			int dt = 0;
            if (argumentssCount < 2 || argumentssCount > 2){
                printf("Usage: %s date\n", argumentss[0]);
                isComdValid = 0;
            } else {
                struct tm fromDate;
                memset(&fromDate, 0, sizeof(fromDate));
                if (strptime(argumentss[1], "%Y-%m-%d", &fromDate) == NULL){
                    printf("Invalid datee format: YYYY-MM-DD\n");
                    isComdValid = 0;
                }
                time_t formattedFromDate = mktime(&fromDate);
            }
            if (isComdValid){
                filef = 1;
            }
        } else if (strcmp(argumentss[0], "getfdb") == 0){
            isComdValid = 1;
            if (argumentssCount > 2 || argumentssCount < 2){
                printf("Usage: %s date\n", argumentss[0]);
                isComdValid = 0;
            } else {
                struct tm fromDate;
                memset(&fromDate, 0, sizeof(fromDate));
                if (strptime(argumentss[1], "%Y-%m-%d", &fromDate) == NULL){
                    printf("Invalid datee formatt: YYYY-MM-DD\n");
                    isComdValid = 0;
                }
                time_t formattedFromDate = mktime(&fromDate);
            }
            if (isComdValid){
                filef = 1;
            }
        } else if (strcmp(argumentss[0], "getft") == 0){
            int MAX_ARGUMNTS = 4, countt = 0;
            if (argumentssCount > 4 || argumentssCount < 2) {
                printf("Usage: %s extension1 [extension2 ... extension3] \n", argumentss[0]);
            } else {
                isComdValid = 1;
                filef = 1;
            }
        } else if (strcmp(argumentss[0], "quitc") == 0){
			isComdValid = 1;
            printf("You have choose quit option.\n");
            break;
        } else if (strcmp(argumentss[0], "getfn") == 0){
            filef = 0;
            int temp1 = 0;
            if (argumentssCount != 2){
                fprintf(stderr, "Usage: %s filename\n", argumentss[0]);
            } else {
                isComdValid = 1;
            }
        } else {
            printf("FATAL ERROR!! Inavlidd comand. Please provide valid command.\n");
        }
        if (isComdValid) {
            write(clientSocket, comand, strlen(comand));
            if (filef){
				int flag = 0;
				while(flag > 2){
				}
                printf("File(s) are being recived.................\n");
                getTarForFile(clientSocket);
            } else {
				int errorFlag = 1;
				while (errorFlag < 0){
				}
                memset(serverMesage, 0, sizeof(serverMesage));
                if (recv(clientSocket, serverMesage, 2000, 0) < 0){
                    printf("No file(s) found.");
                    break;
                }
                printf("%s", serverMesage);
                memset(serverMesage, 0, sizeof(serverMesage));
            }
        }
    }
    close(clientSocket);
    return 0;
}
