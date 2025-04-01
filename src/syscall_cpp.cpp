//
// Created by os on 2/3/25.
//

#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"


////////////////////////////////////////////// NEW I DELETE //////////////////////////////////////////////
void* operator new(size_t size){
  return mem_alloc(size);
}
void* operator new[](size_t size){
  return mem_alloc(size);
}

void operator delete(void* adr) noexcept{
  mem_free(adr);
}
void operator delete[](void* adr) noexcept{
  mem_free(adr);
}

////////////////////////////////////////////// THREAD //////////////////////////////////////////////
Thread::Thread(void (*body)(void *), void *arg) : myHandle(nullptr), body(body), arg(arg){
 /* thread_create(&myHandle,body, arg); //zove se iz syscall_c - SISTEMSKI POZIV
  printString("myhandler ");
    printInt((uint64)myHandle);
    printString("\n");*/
}

Thread::~Thread() {    // nema potrebe za dealokacijom jer nit se sama gasi
}

int Thread::start() {
    if (myHandle == nullptr) {
      return thread_create(&myHandle, body, arg);
    }
    return 0;
    /*if (myHandle != nullptr) {
        return 0;  // nit je već kreirana
    }
    printString("NEEEEEEE TREEEEEBA");
    printString("\n");
    return thread_create(&myHandle, body, arg);*/
}

void Thread::dispatch() {
    thread_dispatch();
}


Thread::Thread()  : myHandle(nullptr), body(Thread::threadRunWrapper), arg(this){
}

void Thread::threadRunWrapper(void* thr) {
  if(thr){
    	Thread* thread = (Thread*) thr;
    	thread->run();
   }
}

////////////////////////////////////////////// CONSOLE //////////////////////////////////////////////

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

////////////////////////////////////////////// SEMAPHORE //////////////////////////////////////////////

Semaphore::Semaphore (unsigned init){
  sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}