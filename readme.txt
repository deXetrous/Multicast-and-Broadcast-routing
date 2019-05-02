Before running, create directory for each host:
Eg. if hosts are 7
create directory named "0", "1","2", "3","4", "5","6", "7" corresponding to host ids. 0 to 7
Each directory would contain directories to store file for each group:
E.g. create directory named "0", "6" inside each one.

To run router and main program :

g++ router.cpp main.cpp -std=c++11 -lpthread
./a.out

Connect hosts equal to number of routers
port starting from:
4000 with increments of 50
id starting from 0:

g++ host.cpp -o host -std=c++11 -lpthread
./host


Sample input :
7

host ids from 0 to 6 each with port no from 4000 increments of 50
Then enter the graph topology:


topology(7*7):
0 1 1 0 0 0 0
1 0 1 1 0 0 0
1 1 0 0 1 1 0
0 1 0 0 1 0 1
0 0 1 1 0 1 0
0 0 1 0 1 0 0
0 0 0 1 0 0 0


Host with id 3 will play the temp2.mp3 file and all others along with it will write the file received in their corresponding directories create above:
The audio file written to the directories can be tested using mpg123 0demo.mp3 1demo.mp3... system command

Other sample topologies:

4*4
0 1 1 0
1 0 0 1
1 0 0 0
0 1 0 0
