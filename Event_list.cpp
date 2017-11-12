#include "Event_list.h"

using namespace std;

Event_list::Event_list()
{
    m_top_node = 0;
    //ctor
}

Event_list::Event_list(void (*callback)(void), unsigned long t_ini, unsigned long delay){
    m_top_node = new Delayed_callback(t_ini, delay, callback);
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
        while(res&&(i!=0)){
            if ((*i).get_next()==0){
                delete i;
                m_top_node = 0;
                return;
            }
            else{
                m_top_node = (*i).get_next();
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
        while((*i).get_next()!=0){

            i=(*i).get_next();
            res=(*i).check(t);

            if(res){
                if((*i).get_next()==0){
                    (*(*i).get_previous()).set_next(0);
                }
                else{
                    (*(*i).get_previous()).set_next((*i).get_next());
                }
                delete i;
            }
        }
        return;

    }
    Delayed_callback i = *(*this).m_top_node;

}

void Event_list::add(void (*callback)(void), unsigned long t_ini, unsigned long delay){
    if (m_top_node==0){
        m_top_node = new Delayed_callback(t_ini, delay, callback, 0);
        return;
    }
    Delayed_callback *i = m_top_node;
    while((*i).get_next()!=0){
        i = (*i).get_next();
    }
    (*i).set_next(new Delayed_callback(t_ini, delay, callback, (*i).get_position()+1));
}

int Event_list::get_length(){
    if(m_top_node==0){
        return 0;
    }
    else{
        return (*m_top_node).get_length();
    }
}
