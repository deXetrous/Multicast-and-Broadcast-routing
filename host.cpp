#include "host.h"
using namespace std;
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
    cout << "successfully connected to router " << endl;
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
            cout << "Report send : " << memReport << endl;
        }
    }
}
int app;
int sharedIndex = 0;
bool finish = false;
void play(int myID)
{

    int x = -1;
    cout << "hi" <<endl;
    while(!finish)
    {
        if(app>3)
        {
            x++;
            cout << x << " " << app <<endl;

            cout << "opening file " << x << endl;
            string fileName = "mpg123";

            cout << app-1 << " " << x+15 <<endl;
            int mini = min(app-1,x+15);
            
            for(int i=x;i<mini;i++)
            {
                fileName += " "+to_string(myID)+"/"+to_string(i)+"demo.mp3";

            }
            
            x=mini-1;
            system(fileName.c_str());
            
            cout << "played till " << x <<endl;
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

    fstream fp;
    // fp.open("song.mp3", ios::binary | ios::out);
    int x=0;
    char recvbuffer[SIZE];
    thread th[1];
    if(myID == 1)
        th[0] = thread(play,myID);
    //th[0].detach();
    
    app = 0;
    while(true)
    {

        bzero(recvbuffer,SIZE);
        int n = recv(H->sockid, recvbuffer, sizeof(recvbuffer), 0);
        
        if(strcmp(recvbuffer,"hello") == 0)
            break;
       
        //cout << x++ << " received " << sizeof(recvbuffer) << " " << n<< endl;

        if(n==0)
        {
            finish = true;
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
    if(myID == 1)
        th[0].join();
    fp.close();



}