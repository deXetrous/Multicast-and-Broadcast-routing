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
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int n;
    socklen_t len = sizeof(cliaddr); 
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len);

    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 
    sendto(sockfd, (const char *)hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 
    printf("Hello message sent.\n"); 

    ifstream fp;

    fp.open("temp1.mp3", ios::binary|ios::in);
    
    int i=0;
    while(fp.eof() == false)
    {
        char sendbuffer[SIZE];
        fp.read(sendbuffer, SIZE);
        cout << i++ << " sending " << sizeof(sendbuffer) << endl;
        sendto(sockfd, (const char *)sendbuffer, sizeof(sendbuffer), 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
    }
    // sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
    // cout << "send hello " << strlen(hello) << endl;
    while(1){}
    close(sockfd);
    return 0; 
} 