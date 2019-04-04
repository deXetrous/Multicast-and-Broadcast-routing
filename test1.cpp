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
using namespace std;
int main()
{
    for(int i=0;i<5;i++)
    {
        int *x;
        x = new int;
        cout << &x << " " << x << endl;

    }

}