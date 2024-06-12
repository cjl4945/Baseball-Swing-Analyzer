#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
//#include <dos.h>
using namespace std;
#define PORT 8080
#define pyPORT 12345

int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);

int py_server_fd, py_new_socket, py_valread;
struct sockaddr_in py_address;
int py_opt = 1;
int py_addrlen = sizeof(py_address);

char data[100] = {0};

char absAngle[10] = {0};
char absmax_Speed[10] = {0}; 
char identity[10] = {0};

char hip_absAngle[10], hip_absmax_Speed[10] = {0};
char shoulder_absAngle[10], shoulder_absmax_Speed[10] = {0};

int fd,df;
char  myfifo[] = "/tmp/fifopipe";
char readfifo[] = "/home/hulk/myProgram.fifo";
char msg [100];


int err;
char key_pressed;
int userKey;

int sockets[2];
int pySOCK;
//int 



enum State {start, idle, displayData, receiveData};

enum State currState;
enum State nextState;

#define MAX_BUF 100

int connect(void){
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		return -1;
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		return -1;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		return -1;
	}
	
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		return -1;
	}
	puts("Waiting for incoming connections for sockets...");
	for (int i = 0; i < 2; i++){
		if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))< 0) {
			perror("accept");
			return -1;
		}
		sockets[i] = new_socket;
		printf("connected %d", i);
	}
	return 0;
}

int pyConnect(void){
	if ((py_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		return -1;
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(py_server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &py_opt,
				sizeof(py_opt))) {
		perror("setsockopt");
		return -1;
	}
	py_address.sin_family = AF_INET;
	py_address.sin_addr.s_addr = INADDR_ANY;
	py_address.sin_port = htons(pyPORT);

	// Forcefully attaching socket to the port 8080
	if (bind(py_server_fd, (struct sockaddr*)&py_address,
			sizeof(py_address))
		< 0) {
		perror("bind failed");
		return -1;
	}
	
	if (listen(py_server_fd, 3) < 0) {
		perror("listen");
		return -1;
	}
	puts("Waiting for incoming connections on webpage...");
	if ((py_new_socket = accept(py_server_fd, (struct sockaddr*)&py_address,(socklen_t*)&py_addrlen))< 0) {
		perror("accept");
		return -1;
	}
	printf("Connected to Python\n");

			
	
	
	
	return 0;
	
	
	}



void stateMachine(void){
	printf("In State Machine\n");
	switch(currState){
		case (start):
			
				
			err = connect();
			while (err != 0){
				err = connect();}
			err = pyConnect();
			while (err != 0){
				err = pyConnect();}	
				
			nextState = idle;
			break;
		case(idle):
			send(py_new_socket,"go", 2,0);
			printf("go Message Sent\n");
			memset(data, '\0', sizeof(data));
			valread=recv(py_new_socket, data, 100,0);
			printf("%s\n", data);
			while ((strcmp("p",data)) != 0){
				valread=recv(py_new_socket, data, 100,0);}
			printf("P recieved from python");
			//printf("Press 'p' to continue: ");
			//scanf("%c", &key_pressed);
			//while (key_pressed != 'p'){scanf("%c", &key_pressed);
				//printf("Key pressed: '%c'\r\n", key_pressed );}
			std::cout << "got key\r\n";
			for (int i = 0; i < 2; i++){
				send(sockets[i], "p", 1, 0); 
			}
			std::cout << "sent message\r\n";
			nextState = receiveData;
		  break;
		case(receiveData): //Scan the data to send to user
			std::cout << "In receiveData \r\n";
			memset(data, '\0', sizeof(data));
			memset(absAngle, '\0', sizeof(absAngle));
			memset(absmax_Speed, '\0', sizeof(absmax_Speed));
			for (int i = 0; i < 2; i++){
				if (recv(sockets[i], data, 100,0) < 0){
					printf("Recieve failed\n");
				}
				printf("Received Data\r\n");
				sscanf(data, "%s %s %s", identity, absmax_Speed, absAngle);
				printf("This is read from Sensor: '%s'\n\r",data);
				printf("This is speed from Sensor: '%s'\n\r",absmax_Speed);
				printf("This is angle from Sensor: '%s'\n\r",absAngle);
				if (!strcmp(identity, "hip")){
					strcpy(hip_absAngle, absAngle);
					strcpy(hip_absmax_Speed,absmax_Speed);
				}
				else if (!strcmp(identity, "shoulder")){
					strcpy(shoulder_absAngle, absAngle);
					strcpy(shoulder_absmax_Speed, absmax_Speed);
				}
				else{
					std::cout << "Got some unknown identity: ";
					std::cout << identity;
					std::cout << "\r\n";
				}
				memset(data, '\0', sizeof(data));
				memset(absAngle, '\0', sizeof(absAngle));
				memset(absmax_Speed, '\0', sizeof(absmax_Speed));
				
			}
		  nextState = displayData;
		  break;
		case(displayData):	
			std::cout << "In displayData\r\n";
			printf("HIPS: \r\n");
			printf("The absolute angle was: %s \r\n",hip_absAngle);
			printf("The absolute max speed was: %s \r\n",hip_absmax_Speed);
			printf("\r\n\r\n");
			
			printf("SHOULDER: \r\n");
			printf("The absolute angle was: %s \r\n",shoulder_absAngle);
			printf("The absolute max speed was: %s \r\n",shoulder_absmax_Speed);
			memset(msg, '\0', sizeof(msg));
			valread = sprintf(msg, "hip %s %s\r", hip_absAngle, hip_absmax_Speed);
			send(py_new_socket,msg,strlen(msg),0);
			memset(data, '\0', sizeof(data));
			valread=recv(py_new_socket, data, 4,0);
			while ((strcmp("good",data)) != 0){
				valread=recv(py_new_socket, data, 4,0);}
			memset(msg, '\0', sizeof(msg));
			valread = sprintf(msg, "shoulder %s %s\r", shoulder_absAngle, shoulder_absmax_Speed);
			send(py_new_socket,msg,strlen(msg),0);
			memset(data, '\0', sizeof(data));
			valread=recv(py_new_socket, data, 4,0);
			while ((strcmp("good",data)) != 0){
				valread=recv(py_new_socket, data, 4,0);}
			memset(data, '\0', sizeof(data));	
			
			sleep(4);
			nextState = idle;
		  break;
		}
	currState = nextState;
}

int main(int argc, char const* argv[]){
	//printf("ST");
	currState = start;
	//pyConnect();
	
	while(1){
		stateMachine();
	}
	
	//return 0;
}





	

