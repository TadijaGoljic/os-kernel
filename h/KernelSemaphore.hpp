#ifndef _semaphore
#define _semaphore

#include "../h/syscall_cpp.hpp"

class KernelSemaphore {
public:

    KernelSemaphore (sem_t* handle, int init) {
        handleSem = handle;
        val = init;
    }

    int wait();

    int signal();

    int try_wait();

private:

    int val;
    sem_t* handleSem;

    struct Elem {
        Elem* next;
        KernelThread* kt;
        Elem(KernelThread* kt1, Elem* next1 = nullptr) {
            next = next1;
            kt = kt1;
        }
    };

    Elem* first = nullptr, *last = nullptr;
};


#endif