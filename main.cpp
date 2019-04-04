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

void sendPack(router *sender, int MapIndex)
{
    sender->sendDataToRouter(MapIndex);
}
void recPack(router *receiver, int MapIndex)
{
    receiver->recvDataFromRouter(MapIndex);
}

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


    std::thread sendingTo(sendPack, routerVector[0], 1);
    std::thread recevingFrom(recPack, routerVector[1], 0);
    
    sendingTo.join();
    recevingFrom.join();

    std::thread sendingTo1(sendPack, routerVector[1], 3);
    std::thread recevingFrom1(recPack, routerVector[3], 1);
    
    sendingTo1.join();
    recevingFrom1.join();


}