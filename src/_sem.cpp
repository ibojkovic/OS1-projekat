//
// Created by os on 3/6/25.
//

#include "../h/_sem.hpp"
#include "../test/printing.hpp"


_sem* _sem::sem_open(unsigned init) {

    return new _sem(init);
}

int _sem::sem_close(sem_t id) {
    if(!id) return -1;
    if(id->closed) return -2;

    while (id->waitingTCB.peekFirst()){
        TCB* tcb = id->waitingTCB.removeFirst();
        tcb->setBlocked(false);
        Scheduler::put(tcb);
    }
    id->closed = true;
    //MemoryAllocator::mem_free(id); //????????????????????????//
    return 0;
}

int _sem::sem_wait(sem_t id) {
    if(!id) return -1;
    if(id->closed) return -2;


    if(id->value > 0){
      id->value--;
    }else{
      TCB* tcb = TCB::running;

      id->waitingTCB.addLast(tcb);
      TCB::running->setBlocked(true);

      TCB::dispatch();

      // Kada se nit probudi, proveravamo da li je semafor zatvoren
      if (id->closed) return -1;
    }

    return 0;
}

int _sem::sem_signal(sem_t id) {
    if(!id) return -1;
    if(id->closed) return -2;

    if(id->waitingTCB.peekFirst()){ //znaci da neka nit ceka
      TCB* tcb = id->waitingTCB.removeFirst();
      tcb->setBlocked(false);
      Scheduler::put(tcb);
    }else{
      id->value++;
    }

    return 0;
}

int _sem::sem_trywait(sem_t id) {

    if (id->value>0) {
        return 1;
    }
    else return 0;
    /*if(!id) return -1;
    if(id->closed) return -2;

    if(id->value == 0){
        return 0;
    }else{
      id->value--;
      return 1;
    }*/

}