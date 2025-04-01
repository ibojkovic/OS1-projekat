//
// Created by os on 5/17/24.
//

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler {
private:
    static List<TCB> readyThreadQueue;

public:
  static bool hasRunnableThreads();
    static TCB* get();
    static void put(TCB* tcb);

};



#endif //SCHEDULER_HPP
