#ifndef scheduler
#define scheduler

#include "../lib/hw.h"
#include "../h/KernelThread.hpp"
#include "../h/list.hpp"

class KernelThread;

class Scheduler {
public:

    static KernelThread *get();

    static void put(KernelThread *kt);

    static int n;

private:

    static List<KernelThread> kts;

};

#endif
