#include "../h/Scheduler.hpp"

List<KernelThread> Scheduler::kts;
int Scheduler::n = 0;

KernelThread *Scheduler::get() {
    n--;
    return kts.removeFirst();
}

void Scheduler::put(KernelThread *kt) {
    n++;
    kts.addLast(kt);
}