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
    string openFileName = "MemReport/"+to_string(myID)+".txt";
    ifstream infile(openFileName);
    string line,oldline;

    while(true)
    {
        // if the file is finished reading, the last line will be send as memReport
        if(getline(infile,line))
            oldline = line;
        else
            line = oldline;
        
        char recvbuffer[SIZE];
        int n = recv(sockIGMP, recvbuffer, sizeof(recvbuffer), 0);
        
        if(strcmp(recvbuffer,"memQuery") == 0)
        {
            // if(myID == 3)
            //     memReport = "4 2 3";
            // else
            //     memReport = "1 2 3";
            
            char msg[5];
            strcpy(msg,line.c_str());
            send(sockIGMP, msg, sizeof(msg), 0);
            cout << "Report send : " << line << endl;
        }
    }
}
int app; //this represent app0
int app6;

int totalToPlay0 = INT_MAX;
int totalToPlay6 = INT_MAX;

void play0(int myID, char idGroup)
{

    int x = 0;
    cout << "hi" <<endl;
    while(true)
    {
        if(app-1 > 5)
        {
            
            cout << x << " " << app-1 << " " << totalToPlay0 <<endl;

            cout << "opening file " << x << endl;
            string fileName = "mpg123";

            cout << app-1 << " " << x+10 <<endl;
            usleep(100);
            int mini = min(app-1,x+10);
            if(x >= totalToPlay0)
                break;
            for(int i=x;i<=mini;i++)
            {
                fileName += " "+to_string(myID)+"/"+ idGroup + "/" + to_string(i)+"demo.mp3";

            }
            if(mini - x > 2)
            {
                system(fileName.c_str());
                cout << "played from " << x << " to " << mini <<endl;
                x=mini+1;
            }

            if(mini == totalToPlay0)
                break;
            
        }

    }

}
void play6(int myID, char idGroup)
{

    int x = 0;
    cout << "hi" <<endl;
    while(true)
    {
        if(app6-1 > 5)
        {
            
            cout << x << " " << app6-1 << " " << totalToPlay6 <<endl;

            cout << "opening file " << x << endl;
            string fileName = "mpg123";

            cout << app6-1 << " " << x+10 <<endl;
            usleep(100);
            int mini = min(app6-1,x+10);
            if(x >= totalToPlay6)
                break;
            for(int i=x;i<=mini;i++)
            {
                fileName += " "+to_string(myID)+"/"+ idGroup + "/" + to_string(i)+"demo.mp3";

            }
            if(mini - x > 2)
            {
                system(fileName.c_str());
                cout << "played from " << x << " to " << mini <<endl;
                x=mini+1;
            }

            if(mini == totalToPlay6)
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
    fstream fp1;
    // fp.open("song.mp3", ios::binary | ios::out);
    int x=0;
    char groupID;
    char recvbuffer1[SIZE+1];
    char recvbuffer[SIZE];
    thread th[1];
    if(myID == 3)
        th[0] = thread(play6,myID, '6');
    //th[0].detach();
    
    app = 0;
    app6 = 0;
    int sharedIndex0 = 0;
    int sharedIndex6 = 0;
    bool end0 = false, end6 = false;
    int countEnd = 0;
    while((end0 == false || end6 == false)&&countEnd<3)
    {
        bzero(recvbuffer,SIZE);
        bzero(recvbuffer1,SIZE+1);
        int n = recv(H->sockid, recvbuffer1, sizeof(recvbuffer1), 0);
        for(int i=0;i<SIZE;i++)
            recvbuffer[i] = recvbuffer1[i+1]; 
        groupID = recvbuffer1[0];
        cout << " -- " << groupID << " received -- " << n << " end0 : "<< end0 << " end6: " << end6 << endl;
        if(n==0)
        {
            cout << "Terminator received : " << groupID << endl;
            countEnd++;
            totalToPlay0 = app-1;
            totalToPlay6 = app6-1;
            // if(groupID == '0')
            // {
            //     cout << "in-------------" << endl;
            //     totalToPlay0 = app-1;
            //     end0 = true;
            // }
            // else if(groupID == '6')
            // {
            //     cout << "in-------------" << endl;
            //     totalToPlay6 = app6-1;
            //     end6 = true;    
            // }
            continue;
        }

        if(groupID == '0')
        {

            if(sharedIndex0 % 10 == 0)
            {
                string fileName = "demo.mp3";

                fileName = to_string(myID) + "/" + groupID + "/" + to_string(app)+fileName; 
                
                //cout << fileName << endl;
                fp.open(fileName, ios::binary | ios::out);
                app++;
                
                
            }
            for(int i=0;i<sizeof(recvbuffer);i++)
                fp << recvbuffer[i];

            if(sharedIndex0%10 == 9)
            {
                cout << "written file " << app-1 << endl;
                fp.close();
                    
            }
            sharedIndex0++;
        }
        else if(groupID == '6')
        {
            if(sharedIndex6 % 10 == 0)
            {
                string fileName = "demo.mp3";

                fileName = to_string(myID) + "/" + groupID + "/" + to_string(app6)+fileName; 
                
                //cout << fileName << endl;
                fp1.open(fileName, ios::binary | ios::out);
                app6++;
                
                
            }
            for(int i=0;i<sizeof(recvbuffer);i++)
                fp1 << recvbuffer[i];

            if(sharedIndex6%10 == 9)
            {
                cout << "written file " << app6-1 << endl;
                fp1.close();
                
                
                
            }
            sharedIndex6++;
        }
    }
    if(myID == 3)
        th[0].join();
    fp.close();
    fp1.close();



}