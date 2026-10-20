#include "../h/KernelThread.hpp"
#include "../h/riscv.hpp"
#include "../h/Scheduler.hpp"

KernelThread *KernelThread::running = nullptr;

KernelThread::KernelThread(thread_t *handle, void(*start_rou)(void *), void *args, void *stack) {
    *handle = (thread_t) this;
    start_routine = start_rou;
    arg = args;
    stack_space = (void *) ((unsigned long) stack + DEFAULT_STACK_SIZE);
    this->context.sp = (uint64) stack_space;
    if (start_rou != nullptr) {
        this->context.ra = (uint64) &KernelThread::wrapper;
        Scheduler::put(this);
    }
}

void KernelThread::yield() {
    Riscv::pushRegisters();
    KernelThread::dispatch();
    Riscv::popRegisters();
}

void KernelThread::dispatch() {
    KernelThread *old = running;
    if (old->isFinished() == false && old->isSuspended() == false) Scheduler::put(old);
    running = Scheduler::get();
    KernelThread::contextSwitch(&old->context, &running->context);
}

void KernelThread::wrapper() {
    running->userMode();
    running->start_routine(running->arg);
    running->finished = true;
    thread_dispatch();
}

KernelThread::~KernelThread() {
    mem_free((void *) ((unsigned long) stack_space - DEFAULT_STACK_SIZE));
}

void KernelThread::startThread(Thread *thr) {
    KernelThread *kt = (KernelThread *) thr->myHandle;
    kt->handle = (thread_t *) thr;
    kt->context.ra = (uint64) &Thread::wrapperThr;
    Scheduler::put(kt);
}

void KernelThread::userMode() {
    uint64 sstatus;
    __asm__ volatile ("csrr %0, sstatus" : "=r" (sstatus));
    sstatus &= (~256);
    __asm__ volatile ("csrw sstatus, %0" : : "r" (sstatus));
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}
