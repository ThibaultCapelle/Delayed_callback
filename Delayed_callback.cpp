#include "Delayed_callback.h"

using namespace std;

Delayed_callback::Delayed_callback()
{
    //ctor
}

Delayed_callback::~Delayed_callback()
{
    //dtor
}

Delayed_callback::Delayed_callback(Delayed_callback const& call):m_t_final(call.m_t_final), m_t_ini(call.m_t_ini), m_callback(call.m_callback)
{
    m_next= new Delayed_callback((*call.m_next).m_t_ini, (*call.m_next).m_t_final-(*call.m_next).m_t_ini, (*call.m_next).m_callback);
    m_previous = new Delayed_callback((*call.m_previous).m_t_ini, (*call.m_previous).m_t_final-(*call.m_previous).m_t_ini, (*call.m_previous).m_callback);
}

Delayed_callback::Delayed_callback(unsigned long t_ini, unsigned long delay, void (*callback)(void)){
		m_t_ini = t_ini;
		m_t_final = t_ini+delay;
		m_callback = callback;
		m_next=0;
		m_previous=0;
}

void Delayed_callback::Set_callback(void (*callback)(void)){
		m_callback = callback;
}

void Delayed_callback::callback(){
	(*m_callback)();
}

Delayed_callback* Delayed_callback::get_next(){
    cout<<"get_next"<<endl;
    return m_next;
}

Delayed_callback* Delayed_callback::get_previous(){
    return m_previous;
}

void Delayed_callback::set_next(Delayed_callback* next){
    cout<<"set next"<<endl;
    m_next=next;
    (*next).set_previous(this);
}

void Delayed_callback::set_previous(Delayed_callback* previous){
    cout<<"set_previous"<<endl;
    m_previous=previous;
}

bool Delayed_callback::check(unsigned long t){
    if (t>m_t_final){
        callback();
        printf("there is a check in Delayed callback\n");
        return true;
    }
    else{
        return false;
    }
}

int Delayed_callback::get_length(){
    if(m_next==0){
        return 1;
    }
    else{
        return 1+(*m_next).get_length();
    }
}


