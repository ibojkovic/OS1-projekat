#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"


//
// Created by os on 6/4/24.
//


#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../test/printing.hpp"
#include "../h/_sem.hpp"

/*sem_t sem; // Globalni _semafor

void worker(void* arg) {
    int id = *(int*)arg;
    printString("Thread ");
    printInt(id);
    printString(" is waiting on Semaphore.\n");

    sem_wait(sem); // Čekamo na semafor

    printString("Thread ");
    printInt(id);
    printString(" passed the semaphore.\n");

    // Simulacija rada niti
    for (volatile int i = 0; i < 100000; i++);

    printString("Thread ");
    printInt(id);
    printString(" is signaling semaphore.\n");

    sem_signal(sem); // Oslobađamo semafor
}

void testSemaphores() {
    sem_open(&sem, 1); // Semafor inicijalizovan sa 1 (binarni semafor)

    const int THREAD_COUNT = 3;
    TCB* threads[THREAD_COUNT];
    int ids[THREAD_COUNT] = {1, 2, 3};

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = TCB::createthread(worker, &ids[i], (uint64*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE));
    }

    TCB::dispatch(); // Početak izvršavanja niti

    for (int i = 0; i < THREAD_COUNT; i++) {
        while (!threads[i]->isfinished()); // Čekamo da se sve niti završe
    }

    sem_close(sem); // Zatvaranje semafora
}

int main() {
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    TCB mainThread(nullptr, nullptr, nullptr);  // Main nit nema telo ni stek
    TCB::running = &mainThread;
    printString("Starting semaphore test...\n");
    testSemaphores();
    printString("Semaphore test completed.\n");
    return 0;
}*/

extern void userMain();



int main() {

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    TCB mainThread(nullptr, nullptr, nullptr);  // Main nit nema telo ni stek
    TCB::running = &mainThread;

    userMain();

    return 0;
}

// TESTNI PROGRAM
/*void worker(void* arg) {
  printString("USOOOO");
        printString("\n");
    for (int i = 0; i < 5; i++) {
        printString("Thread ");
        printInt((uint64)arg);
        printString("\n");
        Thread::dispatch();
    }
}



int main() {
      Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

      TCB mainThread(nullptr, nullptr, nullptr);  // Main nit nema telo ni stek
	  TCB::running = &mainThread;


    Thread t1(worker, (void*)0);
    Thread t2(worker, (void*)1);
    Thread t3(worker, (void*)2);

    t1.start();
    t2.start();
    t3.start();

        // Inicijalizacija main niti

    while (true) {
        Thread::dispatch();
    }

    return 0;
}




void complex_workload() {
    volatile unsigned long result = 1;
    for (int i = 1; i <= 1000; i++) {
        result *= i;
        result %= 1000000007; // Da izbegnemo prevelike brojeve
    }
}

void worker(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < 10; i++) {
        printString("Nit ");
        printInt(id);
        printString(" radi iteraciju ");
        printInt(i);
        printString("\n");

        complex_workload(); // Složeni zadatak

        Thread::dispatch();
    }

    printString("Nit ");
    printInt(id);
    printString(" završava rad.\n");
    thread_exit();
}

int main()
{
    printString("Pokrecem main\n");
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB mainThread(nullptr, nullptr, nullptr);  // Main nit nema telo ni stek
    TCB::running = &mainThread;

    const int num_threads = 10; // Test 1: Kreiramo 10 niti
    Thread* threads[num_threads];
    int thread_ids[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        threads[i] = new Thread(worker, &thread_ids[i]);
        if (threads[i]->start() != 0) {
            printString("Greška pri pokretanju niti ");
            printInt(i + 1);
            printString("\n");
        }
    }

    for (int i = 0; i < 11; i++) {
        printString("Main nit radi iteraciju ");
        printInt(i);
        printString("\n");
        Thread::dispatch();
    }

    printString("Main nit završava rad.\n");
    return 0;
}*/
