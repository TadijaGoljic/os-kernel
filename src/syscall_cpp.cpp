#include "../h/syscall_cpp.hpp"
#include "../h/KernelThread.hpp"


void* operator new (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* pok) {
    mem_free(pok);
    return;
}

char Console ::getc(){
    char c = getc();
    return c;
}

void Console ::putc (char karakter){
     putc (karakter);
     return;
}

void Thread::dispatch () {
    thread_dispatch();
    return;
}

Thread::Thread(void (*body)(void*), void* arg) {
    thread_create(&myHandle, body, arg);
}

Thread::Thread () {
    thread_create(&myHandle, nullptr, nullptr);
}

Thread::~Thread() {
    thread_exit();
}

int Thread::sleep(time_t) {
    return 6;
}

int Thread::start() {
    return thread_start(this);
}

void Thread::wrapperThr() {
    Thread* t = (Thread*)KernelThread::running->handle;
    t->run();
    ((KernelThread*) t->myHandle)->setFinished(true);
    thread_dispatch();
}

Semaphore::Semaphore (unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore:: ~Semaphore () {
    sem_close(myHandle);
}

int Semaphore::wait () {
    return sem_wait(myHandle);
}

int Semaphore::signal () {
    return sem_signal(myHandle);
}

int Semaphore::timedWait (time_t timeout) {
    return sem_timedwait(myHandle, timeout);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}