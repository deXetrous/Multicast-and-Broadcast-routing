#ifndef router_H
#define router_H

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

class router
{
    public:
        int portNo;
        int sockid;
        int x;
        int routerID;
        std::ofstream fpToWrite;
        // struct sockaddr_in addrport;
        // struct sockaddr_in clientAddr;
        int max_conn;
        std::vector<int> hostSockID;
        std::map<int,int> routerSockID;
    
        router(int pNo, int maxcon);
        void setSocketAcceptConnections(bool);


        void hostCommunication();
        void routerCommunication();
        void joinConn(int pNo,int);
        void listenConn(int);
        void closeSockets();
        void sendDataToRouter(int, char*,int,int );
        void recvDataFromRouter(int, int, bool);

        //void manageHost(int , int );

        

};
#endif