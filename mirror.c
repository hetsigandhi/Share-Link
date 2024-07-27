/*
Hetsi Gandhi|110122118|Section 2
Parth Patel|110124289|Section 2
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <time.h>


#define MAXIMUM_COMMAND_LENGTH 2048
#define MAXIMUM_FILE_SIZE 4096
#define COMMAND_SIZE 10000
#define MAXIMUM_ARGS 10
#define MAXIMUM_ATTRR 4
#define serverPort 8081
#define MAXIMUM_ATTR 8
#define serverIP "127.0.1.1"
#define MAXIMUM_CONN 12
#define BUFF_SIZE 10000


void pclientRequest(int clientSocket);
//A file pointer and buffer are utilised for the purpose of reading and writing files..
char fileBuffer[1024] = {0};
FILE *filePointer;



/*Generates a tar archive containing the designated files.
Performing a targeted search for designated files within the HOME directory and all its subordinate directories.
Verify the presence or absence of any files.
Execute the command to generate a tar archive containing the located files.*/
void tarCreationByFile(char *tarFilename, int temp, char **files, int fileCount, int isFilePresent){

    char allFileLocations[COMMAND_SIZE] = "";
    char command[COMMAND_SIZE];
    char fileLocation[MAXIMUM_FILE_SIZE];
	char temp1[100];
	if(temp<0){
	}
    checkingDir(getenv("HOME"), temp, files, fileCount, allFileLocations);
    if (strlen(allFileLocations) == 0){
        isFilePresent = 0;
        printf("No file isFilePresent\n");
        exit(0);
    } else {
        sprintf(command, "%s%s%s", "tar -czvf temp.tar.gz -P", " ", allFileLocations);
        system(command);
    }
}

/*Determines if a file possesses a particular file extension.
Determine the index of the final period in the file name.
Extraction of file extension.
Comparing the extracted extension with the provided list of extensions.*/
int verifyExtension(char *fileName, int ext, char **extensionName, int countOfExtension){
    
    char *position = strrchr(fileName, '.');
    if (!position){
        return 0;
    }
    char *extension = position + 1; 

    for (int z = 0; z < countOfExtension; z++){
        if (strcmp(extension, extensionName[z]) == 0){
            return 1;
        }
    }
    return 0;
}

//Conducting a search for files that have particular file extensions.
void addExtensionFiles(char *rootFileLocation, char *tarFilename, int fn, char **extensionName, int countOfExtension, char *allFileLocations){
    DIR *baseDirectory = opendir(rootFileLocation);
    if (!baseDirectory){
        perror("FATAL ERROR! Encountered error while opening directory.");
        exit(EXIT_FAILURE);
    }
    struct dirent *dirEntry;

    while ((dirEntry = readdir(baseDirectory)) != NULL){
        char fileLocation[MAXIMUM_FILE_SIZE];
        sprintf(fileLocation, "%s/%s", rootFileLocation, dirEntry->d_name);

		int ext = 0, fn = 1;
		if(ext != 0){
		}
		if (dirEntry->d_type == DT_REG && verifyExtension(dirEntry->d_name, ext, extensionName, countOfExtension)){
            printf("File Location: %s\n", fileLocation);
            sprintf(allFileLocations, "%s%s%s", allFileLocations, fileLocation, " ");
        } else if (dirEntry->d_type == DT_DIR && strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0){
            addExtensionFiles(fileLocation, tarFilename, fn, extensionName, countOfExtension, allFileLocations);
        } 
    }
    closedir(baseDirectory);
}

/*Generate a tar archive specifically containing files with certain file extensions.
Querying for files with specific file extensions within the HOME directory path.
Generate a tar archive containing the discovered files.*/
void tarCreationByExtension(char *tarFilename, char **extensionName, char c, int countOfExtension){
    char allFileLocations[50000] = "";
    char command[COMMAND_SIZE] = "";
	int fn = 1;
	
	for(i=0;i<2;i++){
	}
	
    addExtensionFiles(getenv("HOME"), tarFilename, fn, extensionName, countOfExtension, allFileLocations);
    if (strlen(allFileLocations) == 0){
        printf("No files found\n");
        exit(0);
    }else{
        sprintf(command, "%s%s%s", "tar -czvf /home/hetsi/f23Project/temp.tar.gz -P", " ", allFileLocations);
		printf("\ncommand: %s \n",command);
        system(command);
    }
}

/*Perform a recursive search within directories to locate particular files.
Access the directory in order to conduct a thorough search within it.
And then construction of a pathway to the subfolder.
Verifying if the current file corresponds to any of the provided files.
Adding the file path to the comprehensive list. */
void checkingDir(char *dirFileLocation, char temp, char **files, int fileCount, char *allFileLocations){
    
    DIR *dir = opendir(dirFileLocation);
    if (!dir){
        perror("FATAL ERROR! Encoutered error while opening directory.");
        exit(EXIT_FAILURE);
    }

    struct dirent *dirEntry;
    char fileLocation[MAXIMUM_FILE_SIZE];
	char temp1[100];
    while ((dirEntry = readdir(dir)) != NULL){
        if (dirEntry->d_type == DT_DIR){
            if (strcmp(dirEntry->d_name, "..") == 0 || strcmp(dirEntry->d_name, ".") == 0){
                continue;
            }
            sprintf(fileLocation, "%s/%s", dirFileLocation, dirEntry->d_name);
            checkingDir(fileLocation, temp, files, fileCount, allFileLocations);
        }
		
		for(int k =0; k<1; k++){
		}
		
        else if (dirEntry->d_type == DT_REG){
            for (int z = 0; z < fileCount; z++){
                if (strcmp(files[z], dirEntry->d_name) == 0){
                    char fileLocation[MAXIMUM_FILE_SIZE];
                    sprintf(fileLocation, "%s/%s", dirFileLocation, dirEntry->d_name);
                    sprintf(allFileLocations, "%s%s%s", allFileLocations, fileLocation, " ");
                }
            }
        }
    }

    closedir(dir);
}

/*Generates a tar archive for a specified duration of time.
Create the folder if it does not already exist.*/
void tarCreationBeforeDate(const char *dirFileLocation, const char *tarFileLocation, const char *fromDate){
    char folderPath[500];
    snprintf(folderPath, 500, "%s/%s", getenv("HOME"), "f23Project");
    struct stat st;
    if (stat(folderPath, &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("Folder f23Project already exists.\n");
    } else {
        if (mkdir(folderPath, 0700) == 0) {
            printf("Folder f23Project is created successfully.\n");
        } else {
            fprintf(stderr, "FATAL ERROR! Encountered error while creating folder f23Project.\n");
        }
    }
	
	int flag = 0;
	if(flag){}
	
    char command[MAXIMUM_COMMAND_LENGTH];
    char tarDestination[500];
    snprintf(tarDestination,500,"%s/%s",folderPath,tarFileLocation);
    
    snprintf(command, MAXIMUM_COMMAND_LENGTH, "find %s -type f ! -newermt %s -print | tar czvf %s -T - >/dev/null 2>&1", dirFileLocation, fromDate,tarDestination);
    system(command);
}

/*Generates a tar archive within a defined range of sizes.
Create the folder if it does not already exist.*/
void tarCreation(const char *dirFileLocation, const char *tarFileLocation, const char *lowerBound, const char *upperBound, int numberOfFile){
	
    char folderPath[500];
    snprintf(folderPath, 500, "%s/%s", getenv("HOME"), "f23Project");
    struct stat st;
    
	if (S_ISDIR(st.st_mode) && stat(folderPath, &st) == 0) {
        printf("Folder f23Project already exists.\n");
    } else {
        if (mkdir(folderPath, 0700) == 0) {
            printf("Folder f23Project is created successfully.\n");
        } else {
            fprintf(stderr, "FATAL ERROR! Encountered error while creating folder f23Project.\n");
        }
    }
	
    char command[MAXIMUM_COMMAND_LENGTH];
    char tarDestination[500];
    snprintf(tarDestination,500,"%s/%s",folderPath,tarFileLocation);
    snprintf(command, MAXIMUM_COMMAND_LENGTH, "find %s -type f -size +%sc -a -size -%sc -print0 | tar czvf %s --null -T - >/dev/null 2>&1", dirFileLocation, lowerBound, upperBound, tarDestination);

    system(command);
}

/*Transfer the contents of a file to the client by sending it byte by byte.
Access the file that requires transfer.
Retrieve and transmit the contents of the file in the form of bytes.*/
int passingFileByByte(int serverFilePointer, int byte, char *filePointer){
    int processId = fork();

    if (processId == 0){
        int fileDescriptor, len;
        fprintf(stderr, "Copying file content to the client %d\n", serverFilePointer);
        fileDescriptor = open(filePointer, O_RDONLY);
        char buff[BUFF_SIZE];
        memset(buff, 0, sizeof(buff));
		
		while(byte > 10000){
			byte--;
		}

        while ((len = read(fileDescriptor, buff, BUFF_SIZE)) > 0){
            if (send(serverFilePointer, buff, len, 0) == -1)
            {
                perror("FATAL ERROR! Encountered error while sending the file.");
                return -1;
            }
        }
        memset(buff, 0, sizeof(buff));
        sleep(1);
        fprintf(stderr, "File transfer to the client is successful.\n");
        close(fileDescriptor);
        close(serverFilePointer);
        exit(0);
    }
    wait(NULL);
    return 1;
}

/*Locate a file within a specified directory and notify the client.
Access the directory in order to conduct a thorough search within it.
Displaying the necessary information about the file.
Performing a recursive search in subdirectories.*/
void searchFile(int clientSocket, char *fileName, char *fileLocation, int *isFilePresent, int bufferSize){
    DIR *dir;
    struct dirent *dirEntry;
    struct stat st;
    char buff[BUFF_SIZE];

    dir = opendir(fileLocation);
    if (dir == NULL){
        perror("FATAL ERROR!! The directoryyy metioned is NULL.");
        return;
    }
	
	int flg = 0;
	if(flg){}

    while ((dirEntry = readdir(dir)) != NULL){
        if (strcmp(dirEntry->d_name, fileName) == 0){
            sprintf(buff, "%s/%s", fileLocation, fileName);
            if (stat(buff, &st) == 0){

				char mesage[1000];
				int size = 0;
                printf("Filename:%s\nFilepath:%s\nFile Size:%ld\nFile Created At:%s\n File Permissions:%o\n", fileName, fileLocation, st.st_size, ctime(&st.st_mtime), st.st_mode & 0777);
                memset(mesage, 0, sizeof(mesage));
                
                sprintf(mesage, "Filename:%s\nFilepath:%s\nFile Size:%ld\nFile Created At:%s\n File Permissions:%o\n", fileName, fileLocation, st.st_size, ctime(&st.st_mtime), st.st_mode & 0777);
                mesage[strlen(mesage)] = '\0';
				
				for(int z=0; z<4; z++){}
                if (send(clientSocket, mesage, strlen(mesage), 0) < 0){
                    perror("FATAL ERROR!! Encountered error while sending message.");
                    exit(EXIT_FAILURE);
                }
				
				while (size > 0){
					size--;
				}
                memset(mesage, 0, sizeof(mesage));
                *isFilePresent = 1;
                break;
            }
        }

		int bufferSize = 1024;
        if (dirEntry->d_type == DT_DIR && strcmp(dirEntry->d_name, "..") != 0 && strcmp(dirEntry->d_name, ".") != 0){
            sprintf(buff, "%s/%s", fileLocation, dirEntry->d_name);
            searchFile(clientSocket, fileName, buff, isFilePresent, bufferSize);
            if (*isFilePresent){
                break;
            }
        }
    }
    closedir(dir);
}

/*Generates a tar archive for a specified duration of time.
Create the folder if it does not already exist.*/
void tarCreationAfterDate(const char *dirFileLocation, const char *tarFileLocation, int temp, const char *fromDate){
	
    char folderPath[500];
    snprintf(folderPath, 500, "%s/%s", getenv("HOME"), "f23Project");
    struct stat st;
    if (stat(folderPath, &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("Folder f23Project already exists.\n");
    } else {
        if (mkdir(folderPath, 0700) == 0) {
            printf("Folder f23Project is created successfully.\n");
        } else {
            fprintf(stderr, "FATAL ERROR! Encountered error while creating folder f23Project.\n");
        }
    }
    char command[MAXIMUM_COMMAND_LENGTH];
    char tarDestination[500];
    snprintf(tarDestination,500,"%s/%s",folderPath,tarFileLocation);
    
    snprintf(command, MAXIMUM_COMMAND_LENGTH, "find %s -type f -newermt %s -print | tar czvf %s -T - >/dev/null 2>&1", dirFileLocation, fromDate,tarDestination);
    system(command);
}

/*Utilise this function to handle client requests.
Execute diverse client instructions.
Verify whether the given size meets the specified criteria.
Verify whether the arguments fall under the specified limit or not.*/
void pclientRequest(int clientSocket){
    
    int fileDescriptor;
    char buff[BUFF_SIZE], command[1000], clientMessage[2000], serverResponse[2000];
	int argumentCount, ac;
    char *arguments[MAXIMUM_ARGS];

    for(;;){
        int readSz = recv(clientSocket, clientMessage, 2000, 0);
        if (readSz == 0){
            printf("Client has been disconnected.\n");
            break;
        }

        clientMessage[readSz] = '\0';
		char tempVariable[1000];
		argumentCount = 0;
		int fileTransfer = 0;
        printf("Client's message: %s", clientMessage);
        sprintf(tempVariable, "%s", clientMessage);
        arguments[argumentCount] = strtok(tempVariable, " \n");
        
        while (arguments[argumentCount] != NULL && argumentCount < MAXIMUM_ARGS - 1){
            argumentCount++;
            arguments[argumentCount] = strtok(NULL, " \n");
        }
        int successFlag = 0;
        char mesage[1000];

        if (strcmp(arguments[0], "getfz") == 0){
			
            fileTransfer = 1;
            long lowerBound = atol(arguments[1]);
            long upperBound = atol(arguments[2]);
			printf("Command getfz is invoked.\n");

            if (lowerBound < 0 || lowerBound > upperBound || upperBound < 0){
                printf("Invalid size od parameters.\n");
                return 0;
            }

			if(lowerBound < 0 ){
			} else {
			}
            char hDir[1024], lowerBoundStr[20], upperBoundStr[20];
			snprintf(hDir, sizeof(hDir), "%s", getenv("HOME"));
            snprintf(lowerBoundStr, sizeof(lowerBoundStr), "%ld", lowerBound);
            snprintf(upperBoundStr, sizeof(upperBoundStr), "%ld", upperBound);
			
			printf("Home Directory: %s\n", hDir);
            printf("Size Range: %s - %s\n", lowerBoundStr, upperBoundStr);
			
			int numberOfFile = 100;
            tarCreation(hDir, "temp.tar.gz", lowerBoundStr, upperBoundStr, numberOfFile);
            fileTransfer = 1;
			
        } else if (strcmp(arguments[0], "getfn") == 0){
            
			fileTransfer = 0;
			char *fileName = arguments[1];
            char *fileLocation = getenv("HOME");
			int bufferSize = 1024, isFilePresent = 0;
			printf("Command getfn is invoked.\n");
			
            searchFile(clientSocket, fileName, fileLocation, &isFilePresent, bufferSize);
            if (isFilePresent == 0){
				
                printf("File not found.\n");
                memset(mesage, 0, sizeof(mesage));
                sprintf(mesage, "File not found.\n");
                mesage[strlen(mesage)] = '\0';
                if (send(clientSocket, mesage, strlen(mesage), 0) < 0){
                    perror("FATAL ERROR! Encoutereed failure while sending tar file.");
                    exit(EXIT_FAILURE);
                }
                memset(mesage, 0, sizeof(mesage));
            }
			
        } else if (strcmp(arguments[0], "getfda") == 0){
            
			struct tm fromDate;
            printf("Command getfda is invoked.\n");
            memset(&fromDate, 0, sizeof(fromDate));
            
			if (strptime(arguments[1], "%Y-%m-%d", &fromDate) == NULL ){
                printf("Invalid date formaat: YYYY-MM-DD. Please provide valid dates.\n");
            } else {
				int temp = 0;
                char hDir[1024], formattedFromDate[20];
				time_t fromDateee = mktime(&fromDate);
                snprintf(hDir, 1024, "%s", getenv("HOME"));
                strftime(formattedFromDate, sizeof(formattedFromDate), "%Y-%m-%d", &fromDate);
                tarCreationAfterDate(hDir, "temp.tar.gz", temp, formattedFromDate);
                fileTransfer = 1;
            }
			
        } else if (strcmp(arguments[0], "getfdb") == 0){
			
            struct tm fromDate;
            printf("Command getfdb is invoked.\n");
            memset(&fromDate, 0, sizeof(fromDate));
            
			if (strptime(arguments[1], "%Y-%m-%d", &fromDate) == NULL ){
                printf("Invalid datee format: YYYY-MM-DD. Please provide valid dates.\n");
            } else {
                char hDir[1024], formattedFromDate[20];
                snprintf(hDir, 1024, "%s", getenv("HOME"));
                fromDate.tm_mday += 1;
                time_t fromDateee = mktime(&fromDate);
                strftime(formattedFromDate, sizeof(formattedFromDate), "%Y-%m-%d", &fromDate);
				printf("\nformattedfrommdate: %s\n",formattedFromDate);
                tarCreationBeforeDate(hDir, "temp.tar.gz", formattedFromDate);
                fileTransfer = 1;
				
            }
        } else if (strcmp(arguments[0], "quitc") == 0){
            printf("Client has requested to exit the program.\n");
            break;
        } else if (strcmp(arguments[0], "getft") == 0){
			printf("Argument count : %d\n",argumentCount);
            
            if (argumentCount > 4 || argumentCount < 2){
                printf("ERROR!! Invallid total number of arguments for getft.\n");
                return;
            }
            
			printf("Comandd getft is invoked.\n");
            char **extensionName = malloc((argumentCount - 1) * sizeof(char *));
            for (int z = 1; z < argumentCount; z++){
                extensionName[z - 1] = arguments[z];
            }
			
            int countOfExtension = argumentCount - 1;
            if (countOfExtension > 3 || countOfExtension < 1){
                printf("Maximum three extensions are allowed. Not more than that.\n");
                free(extensionName);
                return;
            }
			
			//struct sochaddr_in socketAdd;
            char tarFilename[] = "temp.tar.gz";
			char ch = "temp";
            tarCreationByExtension(tarFilename, extensionName, ch, countOfExtension);
            fileTransfer = 1;
			
        }

        if (fileTransfer){
			int byte = 1024;
            printf("Transfering file....\n");
            passingFileByByte(clientSocket, byte, "temp.tar.gz");
        }
    }
    close(clientSocket);
}

int main(){
	int serverSocket, test = 0, clientSocket, clientRequests = 0, option = 1;
	socklen_t clientAddressLength;
	struct sockaddr_in serverSocketAddress, clientSocketAddress;
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1){
        perror("FATAL ERROR! Encountereed error while creating the socket.");
        exit(EXIT_FAILURE);
    }

    memset(&serverSocketAddress, 0, sizeof(serverSocketAddress));
    serverSocketAddress.sin_family = AF_INET;
    serverSocketAddress.sin_addr.s_addr = inet_addr(serverIP);
    serverSocketAddress.sin_port = htons(serverPort);
    if (bind(serverSocket, (struct sockaddr *)&serverSocketAddress, sizeof(serverSocketAddress)) == -1){
        perror("FATAL ERROR! Encountereed error while binding the socket.");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAXIMUM_CONN) == -1){
        perror("FATAL ERROR! Failed to listen the given port.");
        exit(EXIT_FAILURE);
    }

    printf("WAITING!! Mirror is waitingg for the client to send the request.....\n");
    for(;;){
		int sizeOfClientAddress = 0;
        clientAddressLength = sizeof(clientSocketAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientSocketAddress, &clientAddressLength);
        if (clientSocket == -1){
            perror("FATAL ERROR!! Encountered error while accepting the connection.");
            exit(EXIT_FAILURE);
        }
        printf("\nClient is connected successssfully: %s:%d\n", inet_ntoa(clientSocketAddress.sin_addr), ntohs(clientSocketAddress.sin_port));

		char wcMsg[100];
        if (clientRequests < 4 || (clientRequests >= 8 && (clientRequests + 1) % 2 == 1)){

            char message[] = "Hi there! Welcome! Server is responding.....";
            if (send(clientSocket, message, strlen(message), 0) == -1){
                perror("FATAL ERROR! Encountereed error while sending the message to client.");
                exit(EXIT_FAILURE);
            }

            int processId = fork();
            if (processId == 0){
                printf("Client %d is being handled by the server.\n", clientRequests + 1);
                close(serverSocket);
                pclientRequest(clientSocket);
                exit(EXIT_SUCCESS);
            } else if (processId > 0){
				clientRequests++;
                close(clientSocket);
            } else{
                close(clientSocket);
                perror("FATAL ERROR!! Encountereed error while forking the process.");
                continue;
            }
        } else {
            int processId = fork();
            if (processId == 0){
                char message[] = "Hi there! Welcome! Mirror is responding.....";
                if (send(clientSocket, message, strlen(message), 0) == -1){
                    perror("FATAL ERROR! Encountereed error while sending the message to client.");
                    exit(EXIT_FAILURE);
                }
                printf("Client %d is being handled by the mirror.\n", clientRequests + 1);
                close(serverSocket);
                exit(EXIT_SUCCESS);
            } else if (processId > 0){
				clientRequests++;
                close(clientSocket);
                printf("Client connecttion has been closed from %s:%d\n", inet_ntoa(clientSocketAddress.sin_addr), ntohs(clientSocketAddress.sin_port));
            } else {
                close(clientSocket);
                perror("FATAL ERROR!! Encountereed error while forking the process.");
                continue;
            }
        }
    }
    return 0;
}