#include "host.h"
using namespace std;
host::host(int pNo)
{
    portNo = pNo;
    portNoIGMP = portNo+1;
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
    cout << "successfully connected to router " << endl;
}

void host::setSocketIGMP()
{
    struct sockaddr_in addrport, clientAddr;

	struct hostent * server;
	//server = gethostbyname(argv[1]);
	sockidIGMP = socket(PF_INET, SOCK_STREAM, 0);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(portNoIGMP);

	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sockidIGMP, (struct sockaddr*)&addrport, sizeof(addrport)) < 0 )
        error1("error in connect");
    cout << "successfully connected to router For IGMP communication" << endl;
}

void routerCommunication(int sockIGMP, int myID)
{
    while(true)
    {
        char recvbuffer[SIZE];
        int n = recv(sockIGMP, recvbuffer, sizeof(recvbuffer), 0);
        
        if(strcmp(recvbuffer,"memQuery") == 0)
        {
            string memReport;
            if(myID == 6)
                memReport = "4 2 3";
            else
                memReport = "1 2 3";
            
            char msg[5];
            strcpy(msg,memReport.c_str());
            send(sockIGMP, msg, sizeof(msg), 0);
            cout << "Report send : " << memReport << endl;
        }
    }
}
int app;
int sharedIndex = 0;
int totalToPlay = INT_MAX;

void play(int myID)
{

    int x = 0;
    cout << "hi" <<endl;
    while(true)
    {
        if(app-1 > 5)
        {
            
            cout << x << " " << app-1 << " " << totalToPlay <<endl;

            cout << "opening file " << x << endl;
            string fileName = "mpg123";

            cout << app-1 << " " << x+10 <<endl;
            usleep(100);
            int mini = min(app-1,x+10);
            if(x >= totalToPlay)
                break;
            for(int i=x;i<=mini;i++)
            {
                fileName += " "+to_string(myID)+"/"+to_string(i)+"demo.mp3";

            }
            if(mini - x > 2)
            {
                system(fileName.c_str());
                cout << "played from " << x << " to " << mini <<endl;
                x=mini+1;
            }

            if(mini == totalToPlay)
                break;
            
        }

    }

}

int main()
{
    int myID;
    cout << "Enter this host ID : " << endl;
    cin>>myID;
    int pNo;
    cout << "Enter Router port to connect : " <<endl;
    cin>>pNo;
    host *H;

    H = new host(pNo);

    H->setSocket();
    H->setSocketIGMP();

    thread igmpTH(routerCommunication, H->sockidIGMP, myID);
    igmpTH.detach();
    
    fstream fp;
    // fp.open("song.mp3", ios::binary | ios::out);
    int x=0;
    char recvbuffer[SIZE];
    thread th[1];
    if(myID == 3)
        th[0] = thread(play,myID);
    //th[0].detach();
    
    app = 0;

    while(true)
    {

        bzero(recvbuffer,SIZE);
        int n = recv(H->sockid, recvbuffer, sizeof(recvbuffer), 0);
        
       
        //cout << x++ << " received " << sizeof(recvbuffer) << " " << n<< endl;

        if(n==0)
        {
            totalToPlay = app-1;
            break;
        }

        if(sharedIndex % 10 == 0)
        {


            string fileName = "demo.mp3";

            fileName = to_string(myID) + "/" + to_string(app)+fileName; 
            
            
            fp.open(fileName, ios::binary | ios::out);
            app++;
            
            
        }
        for(int i=0;i<sizeof(recvbuffer);i++)
            fp << recvbuffer[i];

        if(sharedIndex%10 == 9)
        {
            cout << "written file " << app-1 << endl;
            fp.close();
            
            
            
        }

        sharedIndex++;
    }
    if(myID == 3)
        th[0].join();
    fp.close();



}