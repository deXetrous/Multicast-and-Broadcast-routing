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

#define SIZE 1024
using namespace std;

int main()
{
	int sockid;
	struct sockaddr_in addrport, clientAddr;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
    int t = 1;
    setsockopt(sockid,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(3543);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport))!= -1) 
	{
		int status = listen(sockid, 1);
		socklen_t clilen = sizeof(clientAddr);
		int s = accept(sockid, (struct sockaddr*)&clientAddr, &clilen);
		char msg[1024], recvBuf[1024];
        ifstream fp;
        fp.open("temp1.mp3", ios::binary|ios::in);

        int i=0;
        while(fp.eof() == false)
        {
            char sendbuffer[SIZE];
            fp.read(sendbuffer, SIZE);
            cout << i++ << " sending " << sizeof(sendbuffer) << endl;
            //sendto(sockfd, (const char *)sendbuffer, sizeof(sendbuffer), 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
            send(s, sendbuffer, sizeof(sendbuffer), 0);
        }

		close(s);
	}
	
	close(sockid);
}