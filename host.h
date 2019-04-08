#ifndef host_H
#define host_H

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

class host
{
    public:
        int portNo;
        int portNoIGMP;
        int sockid;
        int sockidIGMP;
        int numChannels;
        std::string memReport;
        std::vector<int> channels;
        
        host(int pNo);


        void setSocket();
        void setSocketIGMP();

        void routerCommunication();
    
};

#endif
