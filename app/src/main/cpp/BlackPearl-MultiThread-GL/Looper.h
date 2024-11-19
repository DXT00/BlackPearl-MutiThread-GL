
#ifndef _BYTEFLOW_NATIVE_LOOPER_H_
#define _BYTEFLOW_NATIVE_LOOPER_H_

#ifdef GE_PLATFORM_ANDRIOD
#include <pthread.h>
#include <semaphore.h>

#endif
#include <sys/types.h>

struct LooperMessage {
    int what;
    int arg1;
    int arg2;
    void *obj;
    LooperMessage *next;
    bool quit;
};

class Looper {

public:
    Looper();
    Looper&operator=(const Looper& ) = delete;
    Looper(Looper&) = delete;
    virtual ~Looper();

    void postMessage(int what, bool flush = false);
    void postMessage(int what, void *obj, bool flush = false);
    void postMessage(int what, int arg1, int arg2, bool flush = false);
    void postMessage(int what, int arg1, int arg2, void *obj, bool flush = false);

    void quit();

    virtual void handleMessage(LooperMessage *msg);

private:
    void addMessage(LooperMessage *msg, bool flush);

    static void *trampoline(void *p);

    void loop(void);

    LooperMessage *head;
#ifdef GE_PLATFORM_ANDRIOD

    pthread_t worker;
    sem_t headWriteProtect;
    sem_t headDataAvailable;
#endif
    bool running;

};


#endif //_NATIVE_LOOPER_H_
