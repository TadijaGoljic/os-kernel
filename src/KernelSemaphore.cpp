#include "../h/KernelSemaphore.hpp"
#include "../h/Scheduler.hpp"

int KernelSemaphore::wait() {
    while (this->val <= 0) {
        KernelThread::running->setSuspend(true);
        Elem *tmp = new Elem(KernelThread::running);
        if (this->first == nullptr) first = tmp;
        else last->next = tmp;
        last = tmp;
        KernelThread::yield();
    }
    this->val--;
    return 0;
}

int KernelSemaphore::signal() {
    ++this->val;
    if (this->first == nullptr) return 0;
    Elem *tmp = first;
    KernelThread *n = tmp->kt;
    first = first->next;
    if (this->first == nullptr) last = nullptr;
    delete tmp;
    n->setSuspend(false);
    Scheduler::put(n);
    return 0;
}

int KernelSemaphore::try_wait() {
    if (this->val <= 0) return 0;
    this->val--;
    return 1;
}
