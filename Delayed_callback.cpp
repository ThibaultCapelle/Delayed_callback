#include "Delayed_callback.h"

Delayed_callback::Delayed_callback()
{
    //ctor
}

Delayed_callback::~Delayed_callback()
{
    //dtor
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
    return m_next;
}

Delayed_callback* Delayed_callback::get_previous(){
    return m_previous;
}

void Delayed_callback::set_next(Delayed_callback* next){
    m_next=next;
}

void Delayed_callback::set_previous(Delayed_callback* previous){
    m_previous=previous;
}
