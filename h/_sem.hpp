//
// Created by os on 3/6/25.
//

#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "../h/list.hpp"
#include "../h/tcb.hpp"
#include "../h/MemoryAllocator.hpp"

typedef class _sem* sem_t;

class _sem {
public:
  _sem(unsigned init) : value(init), closed(false) {}
  ~_sem();

  static _sem* sem_open(unsigned init);
  static int sem_close(sem_t handle);
  static int sem_wait(sem_t id);
  static int sem_signal(sem_t id);
  //static int sem_timedwait(_sem* id, time_t timeout);
  static int sem_trywait(sem_t id);



private:
  int value;
  bool closed;
  List<TCB> waitingTCB;  //red niti koje cekaju

};



#endif //SEMAPHORE_HPP
