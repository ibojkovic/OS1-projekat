//
// Created by os on 5/17/24.
//

#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"

List<TCB> Scheduler::readyThreadQueue;

TCB* Scheduler::get(){
    TCB* next = readyThreadQueue.removeFirst();  // ili kako već uzima sledeću nit
    return next;
}

void Scheduler::put(TCB* tcb){
    readyThreadQueue.addLast(tcb);
}

