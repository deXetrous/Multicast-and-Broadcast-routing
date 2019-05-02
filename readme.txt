To run router and main program :

g++ router.cpp main.cpp -std=c++11 -lpthread
./a.out

Connect hosts equal to number of routers
port starting from:
4000 with increments of 50
id starting from 0:

g++ host.cpp -o host -std=c++11 -lpthread
./host



topology(7*7):
0 1 1 0 0 0 0
1 0 1 1 0 0 0
1 1 0 0 1 1 0
0 1 0 0 1 0 1
0 0 1 1 0 1 0
0 0 1 0 1 0 0
0 0 0 1 0 0 0

4*4
0 1 1 0
1 0 0 1
1 0 0 0
0 1 0 0