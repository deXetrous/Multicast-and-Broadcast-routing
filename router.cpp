#include "router.h"

router::router(int pNo, int maxcon)
{
    portNo = pNo;
    portNoIGMP = portNo+1;
    max_conn = maxcon;
    toBeShown0 = true;
    toBeShown6 = true;
}

void error(const char* msg)
{
	perror(msg);
	exit(1);
}

void manageHost(int routerID, int* toBeShown0, int* toBeShown6, int socketHostID,  bool *finishProgram)
{
    
    while(*finishProgram == false)
    {
        char msg[] = "memQuery";
        send(socketHostID, msg, sizeof(msg), 0);
        std::cout << "Membership Query mssg sent by router : " << routerID << std::endl;
        char rcvBuf[SIZE];
        if(recv(socketHostID, rcvBuf, sizeof(rcvBuf), 0) < 0)
            error("Receive Failed");
        std::cout << "Membership Query Report recv by router : " << routerID << " " << rcvBuf << std::endl;
        std::string s(rcvBuf);

        if(s.find("1") == std::string::npos)
            *toBeShown0 = false;
        else
            *toBeShown0 = true;

        if(s.find("2") == std::string::npos)
            *toBeShown6 = false;
        else
            *toBeShown6 = true;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    std::cout << "here" << std::endl;
}

void router::hostIGMPCommunication(bool *finishProgram)
{
    std::thread thForHost[max_conn];
    for(int i=0;i<max_conn;i++)
        thForHost[i] = std::thread(manageHost, routerID, &toBeShown0, &toBeShown6, hostSockIDIGMP[i], finishProgram);

    for(int i=0;i<max_conn;i++)
        thForHost[i].join();
}
using namespace std;
void router::sendDataToRouter(int index, char *sendBuffer, int packNo, int sizeBuffer)
{
    send(routerSockID[index], sendBuffer, sizeBuffer, 0);
}

void router::recvDataFromRouter(int index, int packNo, bool islastPacket)
{
    char rcvBuf[SIZE+1];
    bzero(rcvBuf,SIZE+1);
    if(recv(routerSockID[index], rcvBuf, sizeof(rcvBuf), 0) < 0)
            error("Received Failed");
    //cout << "========"  << strlen(rcvBuf) << sizeof(rcvBuf) << endl;
    if(rcvBuf[0] == '0' && toBeShown0 == true)
    {
        //cout << "Forwarding for 0" << endl;
        // if(islastPacket == true)
        //     rcvBuf[0] = '9';
        send(hostSockID[0], rcvBuf, sizeof(rcvBuf),0);
    }
    else if(rcvBuf[0] == '6' && toBeShown6 == true)
    {
        // if(islastPacket == true)
        //     rcvBuf[0] = '8';
        //cout << "Forwarding for 6" << endl;
        send(hostSockID[0], rcvBuf, sizeof(rcvBuf),0);
    }
}

void router::joinConn(int pNo, int i)
{
    struct sockaddr_in addrport;

	int newsockid = socket(PF_INET, SOCK_STREAM, 0);
    routerSockID[i] = newsockid;
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(pNo);

	//addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrport.sin_addr.s_addr = htons(INADDR_ANY);
    if(connect(newsockid, (struct sockaddr*)&addrport, sizeof(addrport)) < 0 )
        error("error in connect");
    std::cout << "successfully connected to previous router " << newsockid << std::endl;

}


void router::listenConn(int j)
{
    // int status = listen(sockid, 1);
    struct sockaddr_in clientAddr;
    socklen_t clilen = sizeof(clientAddr);
    routerSockID[j] = accept(sockid, (struct sockaddr*)&clientAddr, &clilen);

    std::cout << "Successfully connected " << routerSockID[j] <<std::endl;
    
}

void router::setSocketAcceptConnections()
{
    struct sockaddr_in addrport, clientAddr;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
    int t = 1;
    setsockopt(sockid,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(portNo);
    //addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrport.sin_addr.s_addr = htons(INADDR_ANY);
	if(bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport)) < 0)
        error("Binding failed");
    
    int status = listen(sockid, max_conn+1);
    socklen_t clilen = sizeof(clientAddr);
    for(int i=0;i<max_conn;i++)
    {
        hostSockID.push_back(accept(sockid, (struct sockaddr*)&clientAddr, &clilen));
        if(hostSockID[i] < 0)
			error("Error on accept");
        std::cout << "successfully connected to host " << std::endl;
    }
    x = 10;
    std::cout << "Accepted all the hosts " << hostSockID[0] << std::endl;
}

void router::setSocketAcceptConnectionsIGMP()
{
    struct sockaddr_in addrport, clientAddr;
	sockidIGMP = socket(PF_INET, SOCK_STREAM, 0);
    int t = 1;
    setsockopt(sockidIGMP,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(portNoIGMP);
    //addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrport.sin_addr.s_addr = htons(INADDR_ANY);
	if(bind(sockidIGMP, (struct sockaddr *) &addrport, sizeof(addrport)) < 0)
        error("Binding failed");
    
    int status = listen(sockidIGMP, max_conn+1);
    socklen_t clilen = sizeof(clientAddr);
    for(int i=0;i<max_conn;i++)
    {
        hostSockIDIGMP.push_back(accept(sockidIGMP, (struct sockaddr*)&clientAddr, &clilen));
        if(hostSockIDIGMP[i] < 0)
			error("Error on accept");
        std::cout << "successfully connected to host " << std::endl;
    }
    x = 10;
    std::cout << "Accepted all the hosts for IGMP communication"  << std::endl;
    


}



