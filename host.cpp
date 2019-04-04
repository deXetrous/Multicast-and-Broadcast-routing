#include "host.h"

host::host(int pNo)
{
    portNo = pNo;
}

void error1(const char* msg)
{
	perror(msg);
	exit(1);
}

void host::setSocket()
{
    struct sockaddr_in addrport, clientAddr;

	struct hostent * server;
	//server = gethostbyname(argv[1]);
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(portNo);

	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sockid, (struct sockaddr*)&addrport, sizeof(addrport)) < 0 )
        error1("error in connect");
    std::cout << "successfully connected to router " << std::endl;
}

void host::routerCommunication()
{
    while(true)
    {
        char recvbuffer[SIZE];
        int n = recv(sockid, recvbuffer, sizeof(recvbuffer), 0);
        
        if(strcmp(recvbuffer,"memQuery") == 0)
        {
            memReport = "1 2 3";
            char msg[5];
            strcpy(msg,memReport.c_str());
            send(sockid, msg, sizeof(msg), 0);
            std::cout << "Report send : " << memReport << std::endl;
        }
    }
}