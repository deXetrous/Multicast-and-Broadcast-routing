#include "router.h"

router::router(int pNo, int maxcon)
{
    portNo = pNo;
    max_conn = maxcon;
}

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
        std::cout << "Membership Query mssg sent to host : " << hostID << std::endl;
        char rcvBuf[SIZE];
        if(recv(socketHostID, rcvBuf, sizeof(rcvBuf), 0) < 0)
            error("Receive Failed");
        std::cout << "Membership Query Report recv from host : " << hostID << " " << rcvBuf << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

void router::joinConn(int pNo)
{
    struct sockaddr_in addrport;

	int newsockid = socket(PF_INET, SOCK_STREAM, 0);
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

void router::listenConn()
{
    // int status = listen(sockid, 1);
    struct sockaddr_in clientAddr;
    socklen_t clilen = sizeof(clientAddr);
    routerSockID.push_back(accept(sockid, (struct sockaddr*)&clientAddr, &clilen));

    std::cout << "Successfully connected " << routerSockID[routerSockID.size()-1] <<std::endl;
    // char msg[] = "I received your connection..Reply me back";
    //     send(routerSockID, msg, sizeof(msg), 0);
    // std::cout << "hi " << routerSockID << std::endl;
    // char rcvBuf[SIZE];
    // if(recv(routerSockID, rcvBuf, sizeof(rcvBuf), 0) < 0)
    //         error("Receiving Failed");
    // std::cout << "Received this : " << rcvBuf << std::endl;

}

void router::setSocketAcceptConnections(bool to)
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

void router::hostCommunication()
{
    std::cout << "i am in " << std::endl;
    std::cout << max_conn << " " << std::endl;
    std::cout << hostSockID[0] << std::endl;
    std::thread thForHost[max_conn];
    for(int i=0;i<max_conn;i++)
        thForHost[i] = std::thread(manageHost, hostSockID[i], i+1);

    for(int i=0;i<max_conn;i++)
        thForHost[i].join();
}

