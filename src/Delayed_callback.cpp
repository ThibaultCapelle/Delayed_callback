#include "Delayed_callback.h"

using namespace std;

Delayed_callback::Delayed_callback()
{
    m_next=0;
    m_previous=0;
    m_position=0;
    m_t_final=0;
    m_t_ini=0;
    m_callback=0;
    //ctor
}

Delayed_callback::~Delayed_callback()
{
    adjust_position_for_deletion();
    //dtor
}

Delayed_callback::Delayed_callback(Delayed_callback const& call):m_t_final(call.m_t_final), m_t_ini(call.m_t_ini), m_callback(call.m_callback)
{
    m_next= new Delayed_callback((*call.m_next).m_t_ini, (*call.m_next).m_t_final-(*call.m_next).m_t_ini, (*call.m_next).m_callback);
    m_previous = new Delayed_callback((*call.m_previous).m_t_ini, (*call.m_previous).m_t_final-(*call.m_previous).m_t_ini, (*call.m_previous).m_callback);
    m_position = call.m_position;
}

Delayed_callback::Delayed_callback(unsigned long t_ini, unsigned long delay, void (*callback)(void)){
		m_t_ini = t_ini;
		m_t_final = t_ini+delay;
		m_callback = callback;
		m_next=0;
		m_previous=0;
		m_position=0;
}

Delayed_callback::Delayed_callback(unsigned long t_ini, unsigned long delay, void (*callback)(void), int position){
		m_t_ini = t_ini;
		m_t_final = t_ini+delay;
		m_callback = callback;
		m_next=0;
		m_previous=0;
		m_position=position;
}

void Delayed_callback::Set_callback(void (*callback)(void)){
		m_callback = callback;
}

void Delayed_callback::callback(){
	(*m_callback)();
}

Delayed_callback* Delayed_callback::get_next(){
    return m_next;
}

Delayed_callback* Delayed_callback::get_previous(){
    return m_previous;
}

void Delayed_callback::set_next(Delayed_callback* next){
    m_next=next;
    if(next!=0){
    (*m_next).set_previous(this);}
}

void Delayed_callback::set_previous(Delayed_callback* previous){
    m_previous=previous;
}

bool Delayed_callback::check(unsigned long t){
    if (t>m_t_final){
        callback();
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

void Delayed_callback::set_position(int pos){
    m_position = pos;
}

int Delayed_callback::get_position(){
    return m_position;
}

void Delayed_callback::adjust_position_for_deletion(){
    if (m_next==0){
        m_position = m_position-1;
        return;
    }
    else{
        m_position = m_position-1;
        (*m_next).adjust_position_for_deletion();
    }
}

