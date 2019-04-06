#include <bits/stdc++.h>
using namespace std;
vector<vector<int>> inputmatrix(int x){
	cout << "Enter the graph as adjacency matrix\n";
	vector<vector<int>> v(x, vector<int>(x));
	for (int i = 0; i < x; ++i){
		for (int j = 0; j < x; ++j){
			cin >> v[i][j];
		}
	}
	for (int i = 0; i < x; ++i)
	{
		for (int j = i+1; j < x; ++j){
			v[j][i] = v[i][j];
		}
	}
	return v;
}
void printmatrix(vector<vector<int>> v,int x){
	cout << " the graph  is \n";
	for (int i = 0; i < x; ++i){
		for (int j = 0; j < x; ++j)
			cout << v[i][j];
		cout << "\n";
	}
}

bool BFS(vector<vector<int>> v, int src, int dest, int x, 
                            int pred[], int dist[]) 
{ 
    // a queue to maintain queue of vertices whose 
    // adjacency list is to be scanned as per normal 
    // DFS algorithm 
    list<int> queue; 
  
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

vector<int> printShortestDistance(vector<vector<int>> v, int s, int dest, int x) 
{ 
    // predecessor[i] array stores predecessor of 
    // i and distance array stores distance of i 
    // from s 
    int pred[x], dist[x]; 
  // vector path stores the shortest path 
    vector<int> path;
    if (BFS(v, s, dest, x, pred, dist) == false) 
    { 
        cout << s << " and " << dest 
             << " are not connected\n"; 
        return path; 
    } 
    
    int crawl = dest; 
    path.push_back(crawl); 
    while (pred[crawl] != -1) { 
        path.push_back(pred[crawl]); 
        crawl = pred[crawl]; 
    } 
  
    // distance from source is in distance array 
    cout << "\nShortest path length from " << char(s+65) << " to " << char(dest+65) << 
    " =  " << dist[dest] <<endl; 
  
    // printing path from source to destination 
    cout << "\nPath is::\n"; 
    for (int i = path.size() - 1; i >= 0; i--) 
        cout << char(path[i]+65) << " "; 

    cout << endl;
    return path;
} 

int main(int argc, char const *argv[])
{
	int x,centre;
	map<int,vector<int>> path;
	cout << "Enter number of routers (matrix size)\n";
	cin >> x ;

	vector<vector<int>> v =inputmatrix(x);
	printmatrix(v,x);
	
	cout << "Enter the centre vertex \n" ;
	cin >> centre;

	// now to find path from all other vertices to the centre vertex
	for (int i = 0; i < x; ++i)
	{
		if(i==x)continue;
		path[i] = printShortestDistance(v,i,centre,x);
	}


	return 0;
}