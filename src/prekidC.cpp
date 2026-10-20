#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/Allocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"
#include "../h/KernelConsole.hpp"
#include "../h/KernelSemaphore.hpp"


extern "C" void prekidnaRutinaC() {
    uint64 scause;
    uint64 sepc;
    uint64 stval;
    __asm__ volatile ("csrr %0, sepc" : "=r" (sepc));
    __asm__ volatile ("csrr %0, scause" : "=r" (scause));
    __asm__ volatile ("csrr %0, stval" : "=r" (stval));
    if (scause == (0x01UL << 63 | 0x09)) {
        int code = plic_claim();
        plic_complete(code);
    } else if (scause == 0x02) {
        printString("Ilegalna instrukcija!\n");
        printInt(sepc, 16);
        printString("\n");
    } else if (scause == (0x00UL << 63 | 0x05)) {
        printString("Nedozvoljena adresa citanja!\n");
        printInt(sepc, 16);
        printString("\n");
        printInt(stval, 16);
        printString("\n");
    } else if (scause == (0x00UL << 63 | 0x07)) {
        printString("Nedozvoljena adresa upisa!\n");
        printInt(sepc, 16);
        printString("\n");
    } else if (scause == 0x08 || scause == 0x09) {
        uint64 sepcPom;
        __asm__ volatile("csrr %0, sepc" : "=r" (sepcPom));
        sepcPom += 4;
        __asm__ volatile ("csrw sepc, %0" : : "r" (sepcPom));
        Registers registri;
        __asm__ volatile ("csrr %0, sepc" : "=r" (registri.sepcreg));
        __asm__ volatile ("csrr %0, sstatus" : "=r" (registri.sstatusreg));
        int broj;
        __asm__ volatile ("mv %0, a0" : "=r" (broj));
        if (broj == 0x01) {
            size_t size;
            __asm__ volatile ("mv %0, a1" : "=r" (size));
            void *mem = Allocator::allocate(size);
            __asm__ volatile ("mv a0, %0" : : "r" (mem));
        } else if (broj == 0x02) {
            void *pok;
            __asm__ volatile ("mv %0, a1" : "=r" (pok));
            int dmem = Allocator::deallocate(pok);
            __asm__ volatile ("mv a0, %0" : : "r" (dmem));
        } else if (broj == 0x03) {
            int size = Allocator::getFreeSpace();
            __asm__ volatile ("mv a0, %0" : : "r" (size));
        } else if (broj == 0x04) {
            int size = Allocator::getLargestFree();
            __asm__ volatile ("mv a0, %0" : : "r" (size));
        } else if (broj == 0x11) {
            thread_t *handle;
            void (*start_routine)(void *);
            void *arg;
            void *stack_space;
            __asm__ volatile ("mv %0, a6" : "=r" (stack_space));
            __asm__ volatile ("mv %0, a3" : "=r" (arg));
            __asm__ volatile ("mv %0, a2" : "=r" (start_routine));
            __asm__ volatile ("mv %0, a1" : "=r" (handle));
            __asm__ volatile ("csrr %0, sepc" : "=r" (registri.sepcreg));
            __asm__ volatile ("csrr %0, sstatus" : "=r" (registri.sstatusreg));
            KernelThread *thr = new KernelThread(handle, start_routine, arg, stack_space);
            __asm__ volatile ("csrw sepc, %0"::"r" (registri.sepcreg));
            registri.sstatusreg &= (~(1 << 8));
            __asm__ volatile ("csrw sstatus, %0" : : "r" (registri.sstatusreg));
            __asm__ volatile ("mv a0, %0" : : "r" (thr));
        } else if (broj == 0x12) {
            KernelThread::running->setFinished(true);
        } else if (broj == 0x13) {
            KernelThread::yield();
        } else if (broj == 0x21) {
            sem_t *handle;
            unsigned init;
            __asm__ volatile ("mv %0, a1" : "=r" (handle));
            __asm__ volatile ("mv %0, a2" : "=r" (init));
            __asm__ volatile ("csrr %0, sepc" : "=r" (registri.sepcreg));
            __asm__ volatile ("csrr %0, sstatus" : "=r" (registri.sstatusreg));
            *handle = (sem_t) (new KernelSemaphore(handle, init));
            __asm__ volatile ("csrw sepc, %0"::"r" (registri.sepcreg));
            __asm__ volatile ("csrw sstatus, %0"::"r" (registri.sstatusreg));
        } else if (broj == 0x22) {
            sem_t *handle;
            __asm__ volatile ("mv %0, a1" : "=r" (handle));
            delete (KernelSemaphore *) handle;
        } else if (broj == 0x23) {
            sem_t id;
            __asm__ volatile ("mv %0, a1" : "=r" (id));
            ((KernelSemaphore *) id)->wait();
        } else if (broj == 0x24) {
            sem_t id;
            __asm__ volatile ("mv %0, a1" : "=r" (id));
            ((KernelSemaphore *) id)->signal();
        } else if (broj == 0x41) {
            char c = KernelConsole::getchar();
            __asm__ volatile ("mv a1, %0" : : "r" (c));
        } else if (broj == 0x42) {
            char karakter;
            __asm__ volatile ("mv %0, a1" : "=r" (karakter));
            __putc(karakter);
            console_handler();
        } else if (broj == 0x71) {
            Thread* thread = nullptr;
            __asm__ volatile ("mv %0, a1" : "=r" (thread));
            KernelThread::startThread(thread);
        } else if (broj == 0x75) {
            registri.sstatusreg |= (1 << 8);
        }
        __asm__ volatile ("csrw sepc, %0"::"r" (registri.sepcreg));
        __asm__ volatile ("csrw sstatus, %0"::"r" (registri.sstatusreg));

    }

    __asm__ volatile("csrc sip, 0x02");

}
