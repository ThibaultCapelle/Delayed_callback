#include <iostream>
#include "Delayed_callback.h"
#include <ctime>

using namespace std;

void hello(){
    cout<<time(0)<< endl;
}

int main()
{
    Delayed_callback test(0,0,&hello);
    cout << time(0) << endl;
    test.callback();
    return 0;
}
