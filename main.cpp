#include <iostream>
#include "Delayed_callback.h"
#include "Event_list.h"
#include <ctime>
#include <stdio.h>

using namespace std;

time_t t=time(0);
void hello(){
    cout<<"hello"<< endl;
}

void coucou(){
    printf("bonjour\n");
}

int main()
{
    Event_list events(&hello, 7);
    //Delayed_callback test(time(0),2,&hello);
    //cout << time(0) << endl;
    //test.callback();
    events.add(&coucou, 2);
    events.add(&coucou, 3);
    events.add(&hello, 5);
    events.add(&hello, 10);
    events.add(&coucou, 3);
    //events.add(&coucou, 7.5);
    while(events.get_length()>0){
        printf("length of events is %d\n",events.get_length());
        events.check(time(0));
    }
    printf("length of events is %d\n",events.get_length());
    printf("everything went fine\n");
    return 0;
}
