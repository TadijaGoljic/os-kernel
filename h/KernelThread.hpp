#ifndef _thread_
#define _thread_

#include "../lib/hw.h"
#include "../h/syscall_cpp.hpp"

struct Registers {
    uint64 sstatusreg;
    uint64 sepcreg;
};

class Thread;
class KernelSemaphore;
class KernelThread {

    friend class Thread;
    friend class KernelSemaphore;

    using Body = void (*)(void *);

public:

    KernelThread(thread_t *handle, void(*start)(void *), void *args, void *stack);

    KernelThread(void *stack) {
        stack_space = stack;
    }

    ~KernelThread();

    bool isFinished() const {
        return finished;
    }

    void setFinished(bool value) {
        finished = value;
    }

    bool isSuspended() const {
        return suspended;
    };

    void setSuspend(bool value) {
        suspended = value;
    }

    static void wrapper();

    static void yield();

    static KernelThread *running;

    static void startThread(Thread *thr);

private:

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Context context;
    bool finished = false;
    bool suspended = false;

    thread_t *handle;

    void (*start_routine)(void *);

    void *arg;
    void *stack_space;

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    void userMode();

};

#endif