#include <iostream>
#include "src/Delayed_callback.h"
#include "src/Event_list.h"
#include <ctime>
#include <stdio.h>


using namespace std;

void hello(){
    cout<<"hello"<< endl;
    //events.add(&hello, 2);
}

//Event_list events(&hello, time(0), 7);

Event_list events;
void coucou(){
    printf("bonjour\n");
}

void coucou(int a){
    printf("yo\n");
}

int main()
{
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();

    //Delayed_callback test(time(0),2,&hello);
    //cout << time(0) << endl;
    //test.callback();
    events.add(&coucou, time(0), 2);
    events.add(&coucou, time(0), 3);
    events.add(&hello, time(0), 3);
    events.add(&hello, time(0), 2);
    events.add(&coucou, time(0), 3);
    //events.add(&coucou, 7.5);
    time_t t_ini = time(0);
    while(events.get_length()>0){
        //printf("length of events is %d\n",events.get_length());
        events.check(time(0));
    }
    printf("The execution time is %d s\n", time(0)-t_ini);
    //printf("length of events is %d\n",events.get_length());
    printf("everything went fine\n");
    return 0;
}
