//
// Created by os on 5/17/24.
//

#ifndef TCB_HPP
#define TCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

class TCB{
public:

    using Body = void (*)(void*);
    static TCB *createthread(Body body, void* arg, uint64* stack);

    //konstruktor
    TCB(Body body, void* arg, uint64* stack) :
        body(body),
        arg(arg),
        stack(body != nullptr ? stack : nullptr),
        finished(false),
        blocked(false),
        context({ (uint64)&threadwrapper,
                   stack != nullptr ? (uint64) stack + DEFAULT_STACK_SIZE : 0})
    {
        if(body != nullptr) {
          Scheduler::put(this);
        }
    }

    ~TCB(){ MemoryAllocator::mem_free(stack);}

    bool isfinished() const { return finished; }
    void setfinish(bool blok){ TCB::finished = blok; }

    bool isBlocked() const { return blocked; }
    void setBlocked(bool value) { blocked = value; }

    static void yield(); //pre nego sto preda preda procesor mora da se sacuva kontekst procesora
    static TCB* running; //proces koji se trenutno izvrsava

    static int thread_exit ();
    static void dispatch();//za poremnu konteksta
    static void threadwrapper();

    bool ready = false;

private:


    Body body; //za svaku nit pratimo telo
    void* arg;
    uint64 *stack; //niz koji koristimo kao stek
    bool finished; //da li se zavrsio proces
    bool blocked;
    struct Context{
        uint64 ra;//cuvamo mesto na koje treba da s evrati tj PC
        uint64 sp; //cuvamo SP
    };
    Context context; //kontekst za neku nit



	static void contextswitch(Context* oldcon, Context* runncon);




};



#endif //TCB_HPP
