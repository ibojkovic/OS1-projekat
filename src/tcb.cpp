//
// Created by os on 5/17/24.
//

#include "../h/riscv.hpp"
#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"
#include "../test/printing.hpp"


TCB *TCB::running = nullptr;

TCB *TCB::createthread(Body body, void* arg, uint64* stack){
    return new TCB(body, arg, stack);
}
/*
void TCB::yield(){
    //cuvamo registre na steku
    Riscv::pushreg();
    //menjamo kontekst
    TCB::dispatch();
    //izbacujemo registre sa steka
    Riscv::popreg();
}*/

void TCB::threadwrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

int TCB::thread_exit(){
  TCB::running->setfinish(true);
  TCB::dispatch();
  return 0;
}

void TCB::dispatch(){
    //ovo ovde izvrsava stari proces
    TCB* old = running;

    if(!old->isfinished() && !old->isBlocked()) { Scheduler::put(old); } //ako ovaj proces nije gotov ostavljamo ga u scheduler za kasnije izvlacenje
    TCB::running = Scheduler::get();

    //promena konteksta
    TCB::contextswitch(&old->context, &running->context);
}

