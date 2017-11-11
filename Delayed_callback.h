#ifndef DELAYED_CALLBACK_H
#define DELAYED_CALLBACK_H


class Delayed_callback
{
    public:
        Delayed_callback();
        virtual ~Delayed_callback();
        Delayed_callback(unsigned long t_ini, unsigned long t_final, void (*callback)(void));
		void Set_callback(void (*callback)(void));
		void callback();
		Delayed_callback* get_next();
		Delayed_callback* get_previous();
        void set_next(Delayed_callback* next);
		void set_previous(Delayed_callback* previous);

    protected:

    private:
        unsigned long m_t_ini;
		unsigned long m_t_final;
		void (*m_callback)(void);
		Delayed_callback *m_next;
		Delayed_callback *m_previous;
};

#endif // DELAYED_CALLBACK_H
