 
#include "router.h"
#include <atomic>

// void acceptRouter(host *one)
// {

//     one->setSocket();
//     //one.routerCommunication();

// }
std::atomic<int> finisher(0);
bool BFS(std::vector<std::vector<int>> v, int src, int dest, int x, 
                            int pred[], int dist[]) 
{ 
    // a queue to maintain queue of vertices whose 
    // adjacency list is to be scanned as per normal 
    // DFS algorithm 
    std::list<int> queue; 
  
    // boolean array visited[] which stores the 
    // information whether ith vertex is reached 
    // at least once in the Breadth first search 
    bool visited[x]; 
  
    // initially all vertices are unvisited 
    // so v[i] for all i is false 
    // and as no path is yet constructed 
    // dist[i] for all i set to infinity 
    for (int i = 0; i < x; i++) { 
        visited[i] = false; 
        dist[i] = INT_MAX; 
        pred[i] = -1; 
    } 
  
    // now source is first to be visited and 
    // distance from source to itself should be 0 
    visited[src] = true; 
    dist[src] = 0; 
    queue.push_back(src); 
  
    // standard BFS algorithm 
    while (!queue.empty()) { 
        int u = queue.front(); 
        queue.pop_front(); 
        for (int i = 0; i < x; i++) {
        	if(i==u)continue;                     // same row and column no. WE dont have self loop
        	if(v[u][i] ==0)continue;          // not A NEIGHBOUR 
            if (visited[i] == false) { 
                visited[i] = true; 
                dist[i] = dist[u] + 1; 
                pred[i] = u; 
                queue.push_back(i); 
  
                // We stop BFS when we find 
                // destination. 
                if (i == dest) 
                   return true; 
            } 
        } 
    } 
  
    return false; 
}

std::vector<int> printShortestDistance(std::vector<std::vector<int>> v, int s, int dest, int x) 
{ 
    // predecessor[i] array stores predecessor of 
    // i and distance array stores distance of i 
    // from s 
    int pred[x], dist[x]; 
    
    std::vector<int> path;
    if (BFS(v, s, dest, x, pred, dist) == false) 
    { 
        return path; 
    } 
    
    int crawl = dest; 
    path.push_back(crawl); 
    while (pred[crawl] != -1) { 
        path.push_back(pred[crawl]); 
        crawl = pred[crawl]; 
    } 

    return path;
}

std::map<int,std::vector<int>> findUnicastPath(int noRouters, int centre, std::vector<std::vector<int>> graphRouter)
{
    std::map<int,std::vector<int>> path;
    for (int i = 0; i < noRouters; ++i)
	{
		path[i] = printShortestDistance(graphRouter,i,centre,noRouters);
	}
    return path;
}

void acceptHost(router *first)
{

    first->setSocketAcceptConnections();
    //first.hostCommunication();
}

void acceptHostIGMP(router * first)
{
    first->setSocketAcceptConnectionsIGMP();
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

void sendingMulticastPackets(int centre, int noRouters, std::vector<std::vector<int>>spanningTree, std::vector<router*> routerVector, char *sendBuffer,int sizeBuffer, int packNo, bool islastPacket)
{
    std::queue<int> queueSender;
    queueSender.push(centre);
    std::vector<int> vect(noRouters, 0);
    vect[centre] = 1;
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
                std::cout << "sending data from " << top << " to " << i << " packet no = " << packNo << "centre = " << centre << std::endl;
                std::thread sendingTo(sendPack, routerVector[top], i, sendBuffer, sizeBuffer, packNo);
                std::thread recevingFrom(recPack, routerVector[i], top, packNo, islastPacket);
                sendingTo.join();
                recevingFrom.join();
            }
        }
    }
}

void hostIGMPfunc(router *first, bool *finishProgram)
{
    first->hostIGMPCommunication(finishProgram);
    
}

std::mutex mtx;
void createBranch(int index, int centre, std::vector<int> path, std::vector<std::vector<int>> &spanningTree)
{
    int prev = index;
    for(int i=path.size()-2;i>=0;i--)
    {
        mtx.lock();
        if((spanningTree[path[i]][prev]) == 1)
        {
            mtx.unlock();
            break;
        }
        else
        {
            spanningTree[path[i]][prev] = 1;
            spanningTree[prev][path[i]] = 1;
        }
        mtx.unlock();
        prev = path[i];
    }
}

using namespace std;
void generateFutureSpanningTree(std::vector<std::vector<int>> &spanningTree, int noRouters, int centre, std::map<int,std::vector<int>> unicastPath, std::vector<router*> routerVector)
{
    while(true)
    {
        vector<vector<int>> newSpanningTree;
        for(int i=0;i<noRouters;i++)
        {
            vector<int> temp;
            for(int j=0;j<noRouters;j++)
                temp.push_back(0);  
            newSpanningTree.push_back(temp);
        }
        
        
        vector<thread>spanningTreeTH;
        for(int i=0;i<noRouters;i++)
        {
            if(centre == 0)
            {
                if(i != centre && routerVector[i]->toBeShown0 == true)
                {
                    thread th =  thread(createBranch, i, centre, unicastPath[i], ref(newSpanningTree));
                    spanningTreeTH.push_back(move(th));
                }
            }
            else if(centre == 6)
            {
                if(i != centre && routerVector[i]->toBeShown6 == true)
                {
                    thread th =  thread(createBranch, i, centre, unicastPath[i], ref(newSpanningTree));
                    spanningTreeTH.push_back(move(th));
                }
            }
        }
        
        for(int i=0;i<spanningTreeTH.size();i++)
        {
            spanningTreeTH[i].join();
        }

        spanningTree = newSpanningTree;

    }
}


void manageGroup(int, int, std::vector<std::vector<int>>,std::vector<router*> , string);
int main()
{
    int noRouters;
    std::cout << "Enter no of routers : " << std::endl;
    std::cin >> noRouters;
    //std::map<router*, std::vector<host*>> mapRouterToHost;
    std::vector<router*> routerVector;
    int index = 4000;
    int jump = 50;
    thread routerTH[noRouters];
    thread routerTHIGMP[noRouters];
    for(int i=0;i<noRouters;i++)
    {

        router* newRouter;
        newRouter = new router(index, 1);
        newRouter->routerID = i;
        routerVector.push_back(newRouter);
        
        routerTH[i] = std::thread(acceptHost, routerVector[routerVector.size()-1]);
        routerTHIGMP[i] = std::thread(acceptHostIGMP, routerVector[i]);


        index += jump;
        
    }

    for(int i=0;i<noRouters;i++)
        routerTH[i].join();


    
    for(int i=0;i<noRouters;i++)
        routerTHIGMP[i].join();

    for(int i=0;i<noRouters;i++)
    {
        std::cout << routerVector[i] << std::endl;
    }



    std::cout << "Enter router topology : \n";
    std::vector<std::vector<int>> graphRouter;
    for(int i=0;i<noRouters;i++)
    {
        std::vector<int> temp;
        for(int j=0;j<noRouters;j++)
        {
            int x;
            std::cin >> x;
            temp.push_back(x);  
        }
        graphRouter.push_back(temp);
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

    
    std::thread manageGroupThread1(manageGroup, noRouters, 0, graphRouter, routerVector,"temp1.mp3");
    std::thread manageGroupThread2(manageGroup, noRouters, 6, graphRouter, routerVector,"temp2.mp3");
    
    cout << "hi1" << endl;
    bool finishProgram = false;
    thread IGMP_thread[noRouters];
    for(int i=0;i<noRouters;i++)
    {
        std::cout << "Calling for router " << routerVector[i]->routerID << std::endl;
        IGMP_thread[i] = std::thread(hostIGMPfunc, routerVector[i], &finishProgram);
        //IGMP_thread[i].detach();
    }
    cout << "hi2" << endl;
    sleep(1);

    
    manageGroupThread1.join();
    manageGroupThread2.join();
    
    cout << "hi3" << endl;
    cout << "hi4" << endl;
    cout << "hi5" << endl;
    cout << "hi6" << endl;

    finishProgram = true;
    for(int i=0;i<noRouters;i++)
        IGMP_thread[i].join();
}


void manageGroup(int noRouters, int centre, std::vector<std::vector<int>> graphRouter,std::vector<router*> routerVector, string fileToBeSend)
{

    map<int,vector<int>> unicastPath;


    //path is stored in reverse fashion i.e. from centre node to host node
    unicastPath = findUnicastPath(noRouters, centre, graphRouter);

    map<int, vector<int>>::iterator itr; 
    for(itr = unicastPath.begin();itr!= unicastPath.end();itr++)
    {
        cout << itr->first << endl;
        for(int i = 0; i<itr->second.size();i++)
            cout << itr->second[i] << " -- ";
        cout << endl;

    }

    //creating our spanning tree using centre based approach

    vector<vector<int>> spanningTree;
    for(int i=0;i<noRouters;i++)
    {
        vector<int> temp;
        for(int j=0;j<noRouters;j++)
            temp.push_back(0);  
        spanningTree.push_back(temp);
    }
    
    
    thread spanningTreeTH[noRouters];
    for(int i=0;i<noRouters;i++)
    {
        if(i != centre)
            spanningTreeTH[i] = thread(createBranch, i, centre, unicastPath[i], ref(spanningTree));
    }
    
    for(int i=0;i<noRouters;i++)
    {
        if(i != centre)
            spanningTreeTH[i].join();
    }


    cout << "Printing the spanning Tree generated : " << endl;
    for(int i = 0; i < noRouters; i++)
    {
        for(int j = 0; j < noRouters; j++)
            cout << spanningTree[i][j] << "  ";
        cout << endl;
    }

    thread genFutSpanTreeThread(generateFutureSpanningTree, ref(spanningTree), noRouters, centre, unicastPath, routerVector);
    genFutSpanTreeThread.detach();

    std::ifstream fp;
    fp.open(fileToBeSend, ios::binary|ios::in);
    int packNo=0;
    bool islastPacket = false;
    while(fp.eof() == false)
    {
        cout << "centre :" << centre << " packNo : " << packNo << endl; 
        // if(packNo == 450)
        // {
        //     spanningTree[1][3] = 1;
        //     spanningTree[3][1] = 1;
        // }
        char sendBuffer1[SIZE];
        fp.read(sendBuffer1, SIZE);
        char sendBuffer[SIZE+1];
        if(centre == 0)
            sendBuffer[0]='0';
        else
            sendBuffer[0]='6';
        
        for(int i=1;i<SIZE+1;i++)
            sendBuffer[i]=sendBuffer1[i-1];
        // if(packNo == 0)
        //     cout << "Sending :" << sendBuffer << endl;
        if(fp.eof() == true)
            islastPacket = true;
        sendingMulticastPackets(centre, noRouters, spanningTree, routerVector, sendBuffer,sizeof(sendBuffer), packNo, islastPacket);
        // std::thread sendingTo(sendPack, routerVector[0], 1, sendBuffer, sizeof(sendBuffer),packNo);
        // std::thread recevingFrom(recPack, routerVector[1], 0, packNo, islastPacket);
        // sendingTo.join();
        // recevingFrom.join();

        packNo++;
        //std::cout << packNo << " sending " << sizeof(sendBuffer) << std::endl; 
    }
    finisher++;
    while(finisher != 2){}
    cout << "finished sending " << fileToBeSend << endl;
}