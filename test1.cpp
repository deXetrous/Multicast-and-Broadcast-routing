#include <iostream>
#include <thread>
// class a
// {
//     public:
//     int b;
//     void foo();
// };

// void a::foo()
// {
//     b = 30;
// }
// void f(a *A)
// {
//     A->foo();
// }
// void f1(a A)
// {
//     A.foo();
// }
// int main()
// {
//     a A;
//     f1(A);
//     //std::thread th(f, &A);
//     // A.foo();
//     //th.join();
//     std::cout << A.b ;
// }
// using namespace std;
// int main()
// {
//     for(int i=0;i<5;i++)
//     {
//         int *x;
//         x = new int;
//         cout << &x << " " << x << endl;

//     }

// }

#include<bits/stdc++.h> 
using namespace std; 
   
// The vect is passed by reference and changes 
// made here reflect in main() 
// void func(vector<vector<int>> &vect) 
// { 
   
// } 
   
// int main() 
// { 
//     vector<vector<int>> vect; 
    
   
//     thread th (func,ref(vect)); 
  
    
   
//     return 0; 
// } 

void f1()
{}

int main()
{
    vector<thread> x;
    thread th1(f1);
    x.push_back(move(th1));
    thread th2(f1);
    x.push_back(move(th2));
    x[0].join();
    x[1].join();

}