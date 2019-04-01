#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <bits/stdc++.h>
#include <thread>
#include <vlc/vlc.h>


#define SIZE 1024
using namespace std;
int sharedIndex = 0;
int sharedAdr = 0;
int app;
void play()
{
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    inst = libvlc_new(0, NULL);
    int x = 0;
    while(true)
    {
        
        if(app>3)
        {
            if(x >= app-1)
            break;
            // fp.close();

            // load the vlc engine


            // create a new item
            //m = libvlc_media_new_path(inst, (const char*)collection[i]->payload);
            //cout << " in " << sharedIndex << endl;
            cout << "opening file " << x << endl;
            string fileName = "mpg123 ";
            for(int i=x;i<x+15;i++)
            {
                fileName += to_string(i)+"demo.mp3 ";
                
            }
            x+=15;
            system(fileName.c_str());
            //if(sharedIndex == 1)
            // m = libvlc_media_new_path(inst, fileName.c_str());
            // // else
            // // {
            // //     m = libvlc_media_new_path(inst, "/home/harshit/Documents/NetworksProg/Project/demo12.mp3");
            // // }
            

            // // create a media play playing environment
            // mp = libvlc_media_player_new_from_media(m);


            // // no need to keep the media now
            // //libvlc_media_release(m);

            // // play the media_player
            // libvlc_media_player_play(mp);

            // unsigned int microseconds = 200000;
            // this_thread::sleep_for(chrono::microseconds(microseconds));
            

            // // stop playing
            // libvlc_media_player_stop(mp);

        }

    }
    // libvlc_media_release(m);

    // libvlc_media_player_release(mp);

    // libvlc_release(inst);
}

int main(int argc, char *argv[])
{
	int sockid;
	struct sockaddr_in addrport, clientAddr;

	struct hostent * server;
	//server = gethostbyname(argv[1]);
	sockid = socket(PF_INET, SOCK_STREAM, 0);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(3543);

	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int status = connect(sockid, (struct sockaddr*)&addrport, sizeof(addrport));
	char msg[1024], recvBuf[1024];

    ofstream fp;
    // fp.open("song.mp3", ios::binary | ios::out);
    int x=0;
    char recvbuffer[SIZE];
    thread th[1];
    th[0] = thread(play);
    //th[0].detach();
    int index = 0;
    app = 0;
    while(true)
    {

        bzero(recvbuffer,SIZE);
        int n = recv(sockid, recvbuffer, sizeof(recvbuffer), 0);
        
        if(strcmp(recvbuffer,"hello") == 0)
            break;
       
        cout << x++ << " received " << sizeof(recvbuffer) << " " << n<< endl;
        if(n==0)
            break;

        if(index % 10 == 0)
        {
            // while(sharedAdr == 1)
            // {}
            string fileName = "demo.mp3";
        
            fileName = to_string(app)+fileName;  
            fp.open(fileName, ios::binary | ios::out);
            app++;
            
            
        }
        for(int i=0;i<sizeof(recvbuffer);i++)
            fp << recvbuffer[i];

        if(index%10 == 9)
        {
            cout << "written file " << app-1 << endl;
            fp.close();
            
            // if((index+1)%10 == 0)
            //     sharedIndex = 2;
            // else
            //     sharedIndex = 1;

            // sharedAdr = 1;
            
        }

        index++;
    }

    th[0].join();
    fp.close();
	close(sockid);
}



    