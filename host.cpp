/*
    ./host <hostPort>
*/

//#include "host.h"
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

using namespace std;
#define SIZE 1024
int main(int argc, char *argv[])
{
    int sockid;
	struct sockaddr_in addrport, clientAddr;

	struct hostent * server;
	//server = gethostbyname(argv[1]);
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(atoi(argv[1]));

	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int status = connect(sockid, (struct sockaddr*)&addrport, sizeof(addrport));
    cout << "successfully connected to router " << endl;
    int numChannels = 1;

    while(true)
    {
        char recvbuffer[SIZE];
        int n = recv(sockid, recvbuffer, sizeof(recvbuffer), 0);
        
        if(strcmp(recvbuffer,"memQuery") == 0)
        {
            char memReport[] = "1 2 3";
            send(sockid, memReport, sizeof(memReport), 0);
            cout << "Report send : " << memReport << endl;
        }
    }

}