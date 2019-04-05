#include "host.h"
#include "router.h"


void acceptRouter(host *one)
{

    one->setSocket();
    //one.routerCommunication();

}

void acceptHost(router *first)
{

    first->setSocketAcceptConnections(false);
        
    
    //first.hostCommunication();
}

void connectClientRouter(router *second, int pNo, int i)
{
    second->joinConn(pNo, i);
    

}

void connectServerRouter(router *first, int j)
{
    first->listenConn(j);
}

void sendPack(router *sender, int MapIndex, char *sendBuffer,int sizeBuffer, int packNo)
{
    sender->sendDataToRouter(MapIndex, sendBuffer, packNo, sizeBuffer);
}
void recPack(router *receiver, int MapIndex, int packNo, bool islastPacket)
{
    receiver->recvDataFromRouter(MapIndex, packNo, islastPacket);
}

void sendingMulticastPackets(int noRouters, std::vector<std::vector<int>>spanningTree, std::vector<router*> routerVector, char *sendBuffer,int sizeBuffer, int packNo, bool islastPacket)
{
    std::queue<int> queueSender;
    queueSender.push(0);
    std::vector<int> vect(noRouters, 0);
    vect[0] = 1;
    while(!queueSender.empty())
    {
        int top = queueSender.front();
        queueSender.pop();
        // check the starting of i to avoid packet in a loop
        for(int i=0;i<noRouters;i++)
        {
            if(top!=i && vect[i] ==0 && spanningTree[top][i] == 1)
            {
                vect[i] = 1;
                queueSender.push(i);
                //std::cout << "sending data from " << top << " to " << i << std::endl;
                std::thread sendingTo(sendPack, routerVector[top], i, sendBuffer, sizeBuffer, packNo);
                std::thread recevingFrom(recPack, routerVector[i], top, packNo, islastPacket);
                sendingTo.join();
                recevingFrom.join();
            }
        }
    }
}
using namespace std;
int main()
{
    
    

    // router first(3000,2);
    // // router second(4000,1);
    // std::thread aac(acceptHost, &first);
    // // std::thread aac1(acceptHost, &second);

    // // sleep(1);
    // host one(3000);
    // host two(3000);
    // std::thread acc(acceptRouter, &one);
    // std::thread acc1(acceptRouter, &two);

    // // aac1.join();

    // acc.join();
    // acc1.join();
    
    // aac.join();


    // std::thread con0(newOne, &first);
    // sleep(2);


    // std::thread con1(acceptAnotherRouter, &second);

    // con0.join();
    // con1.join();

    
    
    // sleep(5);

    // std::cout << "done\n";




    int noRouters;
    std::cout << "Enter no of routers : " << std::endl;
    std::cin >> noRouters;
    std::map<router*, std::vector<host*>> mapRouterToHost;
    std::vector<router*> routerVector;
    int index = 3000;
    int jump = 50;
    for(int i=0;i<noRouters;i++)
    {
        int hosts;
        std::cout << "Enter no. of hosts : " << std::endl;
        std::cin>>hosts;
        router* newRouter;
        newRouter = new router(index, hosts);
        newRouter->routerID = i;
        routerVector.push_back(newRouter);
        
        std::thread routerTH(acceptHost, routerVector[routerVector.size()-1]);

        std::vector<host*> hostVector;
        std::thread hostTH[hosts];
        
        for(int j=0;j<hosts;j++)
        {
            sleep(1);
            host *newHost;
            newHost = new host(index);
            hostVector.push_back(newHost);

            hostTH[j] = std::thread(acceptRouter, hostVector[hostVector.size()-1]);
        }
        routerTH.join();
        for(int j=0;j<hosts;j++)
            hostTH[j].join();

        mapRouterToHost[routerVector[routerVector.size()-1]] = hostVector;
        index += jump;
        std::cout << "Connected " << hosts << " host for router with pNO = " << index-jump << std::endl;
        
    }

    for(int i=0;i<noRouters;i++)
    {
        std::cout << routerVector[i] << std::endl;
    }

    // std::thread con0(connectServerRouter, routerVector[0]);
    // sleep(2);


    // std::thread con1(connectClientRouter, routerVector[1], 3000);

    // con0.join();
    // con1.join();


    std::cout << "Enter router topology : \n";
    int graphRouter[noRouters][noRouters];
    for(int i=0;i<noRouters;i++)
    {
        for(int j=0;j<noRouters;j++)
        {
            std::cin >> graphRouter[i][j];
            
        }
    }

    for(int i=0;i<noRouters;i++)
    {
        for(int j=0;j<noRouters;j++)
        {
            if(i<j && graphRouter[i][j] == 1)
            {
                std::thread serverRouterTH(connectServerRouter, routerVector[i], j);
                int portToConnect = index-(noRouters-i)*jump;
                std::thread clientRouterTH(connectClientRouter, routerVector[j], portToConnect, i);

                serverRouterTH.join();
                clientRouterTH.join();
                std::cout << "Router " << i << " and " << j << " connected" << std::endl;
            }
        }
    }


    for(int i=0;i<noRouters;i++)
    {
        std::cout << "Printing for router :" << i << std::endl;
        std::map<int, int>::iterator itr; 
        for (itr = routerVector[i]->routerSockID.begin(); itr != routerVector[i]->routerSockID.end(); ++itr)
        { 
            std::cout << '\t' << itr->first 
                << '\t' << itr->second << '\n'; 
        } 
    }


    // std::thread sendingTo(sendPack, routerVector[0], 1);
    // std::thread recevingFrom(recPack, routerVector[1], 0);
    
    // sendingTo.join();
    // recevingFrom.join();

    // std::thread sendingTo1(sendPack, routerVector[1], 3);
    // std::thread recevingFrom1(recPack, routerVector[3], 1);
    
    // sendingTo1.join();
    // recevingFrom1.join();




    //creating our spanning tree

    // int *spanningTree[noRouters];
    // for(int i = 0; i < noRouters; i++)
    //     spanningTree[i] = new int[noRouters];

    std::vector<std::vector<int>> spanningTree;
    

    for(int i=0;i<noRouters;i++)
    {
        std::vector<int> temp;
        for(int j=0;j<noRouters;j++)
        {
            int x;
            std::cin >> x;
            temp.push_back(x);  
        }
        spanningTree.push_back(temp);
    }

    std::ifstream fp;
    fp.open("temp1.mp3", ios::binary|ios::in);
    int packNo=0;
    bool islastPacket = false;
    while(fp.eof() == false)
    {
        char sendBuffer[SIZE];
        fp.read(sendBuffer, SIZE);
        // if(packNo == 0)
        //     cout << "Sending :" << sendBuffer << endl;
        if(fp.eof() == true)
            islastPacket = true;
        sendingMulticastPackets(noRouters, spanningTree, routerVector, sendBuffer,sizeof(sendBuffer), packNo, islastPacket);
        // std::thread sendingTo(sendPack, routerVector[0], 1, sendBuffer, sizeof(sendBuffer),packNo);
        // std::thread recevingFrom(recPack, routerVector[1], 0, packNo, islastPacket);
        // sendingTo.join();
        // recevingFrom.join();

        packNo++;
        //std::cout << packNo << " sending " << sizeof(sendBuffer) << std::endl;
        
    }

    

    sleep(5);


}