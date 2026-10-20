#include "../lib/hw.h"
#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"

extern "C" void interrupt();

extern void userMain();

void systemMode() {
    __asm__ volatile ("li a0, 0x75");
    __asm__ volatile ("ecall");
}

void main() {
    __asm__ volatile ("csrw stvec, %0" : : "r" (interrupt));
    __asm__ volatile ("csrs sstatus, 0x02");
    void *stack = mem_alloc(DEFAULT_STACK_SIZE);
    stack = (void *) ((unsigned long) stack + DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv sp, %0" : : "r" (stack));
    KernelThread::running = new KernelThread(stack);
    userMain();
    printString("End!");
    systemMode();
    __asm__ volatile ("csrw stvec, %0" : : "r" (0));
}