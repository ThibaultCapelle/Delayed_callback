#ifndef DELAYED_CALLBACK_H
#define DELAYED_CALLBACK_H
#include <iostream>
#include <stdio.h>


class Delayed_callback
{
    public:
        Delayed_callback();
        virtual ~Delayed_callback();
        Delayed_callback(Delayed_callback const& callback);
        Delayed_callback(unsigned long t_ini, unsigned long delay, void (*callback)(void));
        Delayed_callback(unsigned long t_ini, unsigned long delay, void (*callback)(void), int position);
		void Set_callback(void (*callback)(void));
		void callback();
		Delayed_callback* get_next();
		Delayed_callback* get_previous();
        void set_next(Delayed_callback* next);
		void set_previous(Delayed_callback* previous);
		bool check(unsigned long t);
		int get_length();
		void set_position(int pos);
        int get_position();
        void adjust_position_for_deletion();

    protected:

    private:
        int m_position;
        unsigned long m_t_ini;
		unsigned long m_t_final;
		void (*m_callback)(void);
		Delayed_callback *m_next;
		Delayed_callback *m_previous;
};

#endif // DELAYED_CALLBACK_H
