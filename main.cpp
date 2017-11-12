#include <iostream>
#include "Delayed_callback.h"
#include "Event_list.h"
#include <ctime>
#include <stdio.h>

using namespace std;

void hello(){
    cout<<"hello"<< endl;
    //events.add(&hello, 2);
}

Event_list events(&hello, time(0), 7);


void coucou(){
    printf("bonjour\n");
    //events.add(&coucou, time(0), 2);
}

int main()
{
    //Delayed_callback test(time(0),2,&hello);
    //cout << time(0) << endl;
    //test.callback();
    events.add(&coucou, time(0), 3);
    events.add(&coucou, time(0), 3);
    events.add(&hello, time(0), 3);
    events.add(&hello, time(0), 10);
    events.add(&coucou, time(0), 3);
    //events.add(&coucou, 7.5);
    while(events.get_length()>0){
        printf("length of events is %d\n",events.get_length());
        events.check(time(0));
    }
    printf("length of events is %d\n",events.get_length());
    printf("everything went fine\n");
    return 0;
}
