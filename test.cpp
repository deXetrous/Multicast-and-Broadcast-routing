/*
    ./router <routerPort>
*/
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

void error(const char* msg)
{
	perror(msg);
	exit(1);
}


void manageHost(int socketHostID, int hostID)
{
    while(true)
    {
        char msg[] = "memQuery";
        send(socketHostID, msg, sizeof(msg), 0);
        cout << "Membership Query mssg sent to host : " << hostID << endl;
        char rcvBuf[SIZE];
        if(recv(socketHostID, rcvBuf, sizeof(rcvBuf), 0) < 0)
            error("Receive Failed");
        cout << "Membership Query Report recv from host : " << hostID << " " << rcvBuf << endl;
        this_thread::sleep_for(chrono::milliseconds(3000));
    }
}

void acceptHost()
{
	int sockid;
	struct sockaddr_in addrport, clientAddr;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
    int t = 1;
    setsockopt(sockid,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(3000);
	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport)) < 0)
        error("Binding failed");
    
    int max_conn=1;
    // cout << "Enter number of host connections for this router : " << endl;
    // cin>> max_conn;
    int status = listen(sockid, max_conn);
    socklen_t clilen = sizeof(clientAddr);

    vector<int> hostSockID;

    for(int i=0;i<max_conn;i++)
    {
        hostSockID.push_back(accept(sockid, (struct sockaddr*)&clientAddr, &clilen));
        if(hostSockID[i] < 0)
			error("Error on accept");
        cout << "successfully connected to host " << endl;
    }

    cout << "Accepted all the hosts " << endl;
    for(int i=0;i<max_conn;i++)
        close(hostSockID[i]);
	close(sockid);

}

void acceptRouter()
{
    int sockid;
	struct sockaddr_in addrport, clientAddr;

	struct hostent * server;
	//server = gethostbyname(argv[1]);
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(3000);

	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sockid, (struct sockaddr*)&addrport, sizeof(addrport)) < 0 )
        error("error in connect");
    cout << "successfully connected to router " << endl;
    
}

int main(int argc, char *argv[])
{
    thread aac(acceptHost);
    sleep(1);
    thread acc(acceptRouter);
    aac.join();
    acc.join();


   

}