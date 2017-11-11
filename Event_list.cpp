#include "Event_list.h"

using namespace std;

Event_list::Event_list()
{
    m_top_node = 0;
    //ctor
}

Event_list::Event_list(void (*callback)(void), unsigned long t){
    m_top_node = new Delayed_callback(time(0), t, callback);
    (*m_top_node).set_position(0);
}


Event_list::~Event_list()
{
    //dtor
}

void Event_list::check(unsigned long t){
    if (m_top_node==0){
        return;
    }
    else{
        Delayed_callback *i = m_top_node;
        bool res = (*i).check(t);
        printf("check was tested in Event_list\n");
        while(res&&(i!=0)){
            printf("there is a check in Event_list\n");
            if ((*i).get_next()==0){
                printf("Something is deleted");
                delete i;
                m_top_node = 0;
                return;
            }
            else{
                m_top_node = (*i).get_next();
                printf("Something is deleted");
                delete (*m_top_node).get_previous();
                (*m_top_node).set_previous(0);
                i=m_top_node;
                if (i!=0){
                res=(*i).check(t);}
                else{
                    return;
                }
            }
        }
        if(res){
            printf("there is a check but it is a problem\n");
        }

        while((*i).get_next()!=0){

            i=(*i).get_next();
            res=(*i).check(t);

            if(res){
                printf("There is a check for a middle or end queue\n");
                printf("position of this check is %d\n", (*i).get_position());
                //Delayed_callback *j=i;
                if((*i).get_next()==0){
                    printf("we are in queue\n");
                    if ((*i).get_previous()==0){printf("why the hell this guy is dead ?\n");}
                    (*(*i).get_previous()).set_next(0);
                }
                else{
                    (*(*i).get_previous()).set_next((*i).get_next());
                }
                printf("now we are getting rid of our queue\n");
                delete i;
            }
        }
        return;

    }
    Delayed_callback i = *(*this).m_top_node;

}

void Event_list::add(void (*callback)(void), unsigned long t){
    if (m_top_node==0){
        m_top_node = new Delayed_callback(time(0), t, callback, 0);
        return;
    }
    Delayed_callback *i = m_top_node;
    while((*i).get_next()!=0){
        cout<<"get_next is not zero"<<endl;
        i = (*i).get_next();
    }
    cout<<"get_next is zero"<<endl;
    (*i).set_next(new Delayed_callback(time(0), t, callback, (*i).get_position()+1));
}

int Event_list::get_length(){
    if(m_top_node==0){
        return 0;
    }
    else{
        return (*m_top_node).get_length();
    }
}
