#include "../h/syscall_c.h"
#include "../lib/hw.h"
#include "../h/KernelThread.hpp"

void* mem_alloc (size_t size) {
    size = (size - 1) / MEM_BLOCK_SIZE + 1;
    __asm__ volatile ("mv a1, %0" : : "r" (size));
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("ecall");
    void* mem;
    __asm__ volatile ("mv %0, a0" : "=r" (mem));
    return mem;
}

int mem_free (void* pok) {
    __asm__ volatile ("mv a1, %0" : : "r" (pok));
    __asm__ volatile ("li a0, 0x02");
    __asm__ volatile ("ecall");
    int dmem;
    __asm__ volatile ("mv %0, a0" : "=r" (dmem));
    return dmem;
}

size_t mem_get_free_space() {
    __asm__ volatile ("li a0, 0x03");
    __asm__ volatile ("ecall");
    int result;
    __asm__ volatile ("mv %0, a0" : "=r" (result));
    return result;
}

size_t mem_get_largest_free_block() {
    __asm__ volatile ("li a0, 0x04");
    __asm__ volatile ("ecall");
    int result;
    __asm__ volatile ("mv %0, a0" : "=r" (result));
    return result;
}

void putc (char karakter) {
    __asm__ volatile ("mv a1, %0" : : "r" (karakter));
    __asm__ volatile ("li a0, 0x42");
    __asm__ volatile ("ecall");
    return;
}

char getc () {
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile ("ecall");
    char c;
    __asm__ volatile ("mv %0, a1" : "=r" (c));
    return c;
}

void thread_dispatch () {
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
    return;
}


int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg) {
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_space == nullptr) return -1;
    __asm__ volatile ("mv a6, %0" : : "r" (stack_space));
    __asm__ volatile ("mv a3, %0" : : "r" (arg));
    __asm__ volatile ("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %0, a0" : "=r" (handle));
    return 0;
}

int thread_exit () {
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");
    return 0;
}

int sem_open (sem_t* handle, unsigned init) {
    if(handle == nullptr) return -2;
    __asm__ volatile ("mv a2, %0" : : "r" (init));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");
    return 0;
}

int sem_close (sem_t handle) {
    if(handle == nullptr) return -3;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");
    return 0;
}

int sem_wait (sem_t id) {
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");
    return 0;
}

int sem_signal (sem_t id) {
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");
    return 0;
}

int sem_trywait(sem_t id) {
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("li a0, 0x26");
    __asm__ volatile ("ecall");
    return 0;
}

int sem_timedwait(sem_t id, time_t timeout) {
    return 0;
}

int time_sleep (time_t) {
    return 0;
}

int thread_start(Thread* thread) {
    __asm__ volatile ("mv a1, %0" : : "r" (thread));
    __asm__ volatile ("li a0, 0x71");
    __asm__ volatile ("ecall");
    return 0;
}