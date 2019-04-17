#include "router.h"

router::router(int pNo, int maxcon)
{
    portNo = pNo;
    portNoIGMP = portNo+1;
    max_conn = maxcon;
    toBeShown = true;
}

void error(const char* msg)
{
	perror(msg);
	exit(1);
}

void manageHost(int routerID, int* toBeShown, int socketHostID, int routerid, bool *finishProgram)
{
    while(*finishProgram == false)
    {
        char msg[] = "memQuery";
        send(socketHostID, msg, sizeof(msg), 0);
        std::cout << "Membership Query mssg sent by router : " << routerid << std::endl;
        char rcvBuf[SIZE];
        if(recv(socketHostID, rcvBuf, sizeof(rcvBuf), 0) < 0)
            error("Receive Failed");
        std::cout << "Membership Query Report recv by router : " << routerid << " " << rcvBuf << std::endl;
        std::string s(rcvBuf);
        std::cout << routerID << " ---- " << rcvBuf << std::endl;
        if(s.find("1") == std::string::npos)
            *toBeShown = false;
        if(routerID == 6)
            std::cout << s << " @@@@ " << *toBeShown <<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

void router::hostIGMPCommunication(bool *finishProgram)
{
    std::thread thForHost[max_conn];
    for(int i=0;i<max_conn;i++)
        thForHost[i] = std::thread(manageHost, routerID, &toBeShown, hostSockIDIGMP[i], routerID, finishProgram);

    for(int i=0;i<max_conn;i++)
        thForHost[i].join();
}

void router::sendDataToRouter(int index, char *sendBuffer, int packNo, int sizeBuffer)
{
    // char rcvBuf[SIZE];
    // if(recv(newsockid, rcvBuf, sizeof(rcvBuf), 0) < 0)
    //         error("Received Failed");
    // std::cout << "Message received " << rcvBuf << std::endl;
    
    //char msg[] = "Sending mssg..Hello123\n";

    // if(packNo == 0)
    // std::cout << "SEnding reallY : " <<sizeof(sendBuffer) << " " << sizeBuffer<< " " << sendBuffer << std::endl;
    send(routerSockID[index], sendBuffer, sizeBuffer, 0);
    //std::cout << "Message Sending " << std::endl;

}
using namespace std;
void router::recvDataFromRouter(int index, int packNo, bool islastPacket)
{
    char rcvBuf[SIZE];
    bzero(rcvBuf,SIZE);
    if(recv(routerSockID[index], rcvBuf, sizeof(rcvBuf), 0) < 0)
            error("Received Failed");
    
    send(hostSockID[0], rcvBuf, sizeof(rcvBuf),0);
    //std::cout << "Message received routerID " << routerID << " " << packNo << " " << sizeof(rcvBuf) << std::endl;
    
    // if(packNo % 10 == 0)
    // {
    //     std::string fileName = std::to_string(routerID) + "/";
    //     fileName += std::to_string(packNo/10)+ "demo.mp3";  
    //     fpToWrite.open(fileName, std::ios::binary | std::ios::out);
        
    // }
    // for(int i=0;i<sizeof(rcvBuf);i++)
    //     fpToWrite << rcvBuf[i];
    // if(packNo%10 == 9)
    // {
    //     fpToWrite.close();
    // }
    // if(islastPacket)
    //     fpToWrite.close();
}

void router::joinConn(int pNo, int i)
{
    struct sockaddr_in addrport;

	int newsockid = socket(PF_INET, SOCK_STREAM, 0);
    routerSockID[i] = newsockid;
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(pNo);

	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(newsockid, (struct sockaddr*)&addrport, sizeof(addrport)) < 0 )
        error("error in connect");
    std::cout << "successfully connected to previous router " << newsockid << std::endl;
    // char rcvBuf[SIZE];
    // if(recv(newsockid, rcvBuf, sizeof(rcvBuf), 0) < 0)
    //         error("Received Failed");
    // std::cout << "Message received " << rcvBuf << std::endl;
    // char msg[] = "Replying back..fuck off\n";
    //     send(newsockid, msg, sizeof(msg), 0);

}


void router::listenConn(int j)
{
    // int status = listen(sockid, 1);
    struct sockaddr_in clientAddr;
    socklen_t clilen = sizeof(clientAddr);
    routerSockID[j] = accept(sockid, (struct sockaddr*)&clientAddr, &clilen);

    std::cout << "Successfully connected " << routerSockID[j] <<std::endl;
    // char msg[] = "I received your connection..Reply me back";
    //     send(routerSockID, msg, sizeof(msg), 0);
    // std::cout << "hi " << routerSockID << std::endl;
    // char rcvBuf[SIZE];
    // if(recv(routerSockID, rcvBuf, sizeof(rcvBuf), 0) < 0)
    //         error("Receiving Failed");
    // std::cout << "Received this : " << rcvBuf << std::endl;

}

void router::setSocketAcceptConnections()
{
    struct sockaddr_in addrport, clientAddr;
	sockid = socket(PF_INET, SOCK_STREAM, 0);
    int t = 1;
    setsockopt(sockid,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(portNo);
    addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addrport.sin_addr.s_addr = htonl(INADDR_ANY);
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
    addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addrport.sin_addr.s_addr = htonl(INADDR_ANY);
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



