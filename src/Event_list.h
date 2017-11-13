#ifndef EVENT_LIST_H
#define EVENT_LIST_H
#include "Delayed_callback.h"


class Event_list
{
    public:
        Event_list();
        Event_list(void (*callback)(void), unsigned long t_ini, unsigned long delay);
        virtual ~Event_list();

        Delayed_callback* Get_m_top_node() { return m_top_node; }
        void Set_m_top_node(Delayed_callback *val) { m_top_node = val; }

        void check(unsigned long t);
        void add(void (*callback)(void), unsigned long t_ini, unsigned long delay);

        int get_length();

    protected:

    private:
        Delayed_callback *m_top_node;
};

#endif // EVENT_LIST_H
