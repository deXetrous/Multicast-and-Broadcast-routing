#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <bits/stdc++.h>
  
#define PORT     8080 
#define MAXLINE 1024
#define SIZE 1024
using namespace std;
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello"; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n;

      
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    printf("Hello message sent.\n"); 
    
    socklen_t len = sizeof(servaddr);

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 

    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer);

    
    ofstream fp;
    fp.open("song.mp3", ios::binary | ios::out);
    int i=0;
    while(true)
    {
        int n;
        char recvbuffer[SIZE];
        n = recvfrom(sockfd, (char *)recvbuffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        // if(strcmp(recvbuffer,"hello") == 0)
        //     break;
       
        cout << i++ << " received " << sizeof(recvbuffer) << endl;
        
        
    }
  
    close(sockfd); 
    return 0; 
} 