//
// Created by os on 2/3/25.
//

#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP



#include "../lib/hw.h"

//0x01
void* mem_alloc(size_t size);
//0x02
int mem_free(void*);

//0x11
class _thread {};
typedef _thread* thread_t;
int thread_create(thread_t* handle, void(*start_routine) (void*), void* arg);
//0x12
int thread_exit ();
//0x13
void thread_dispatch ();

//0x21
class _sem;
typedef _sem* sem_t;
int sem_open (sem_t* handle, unsigned init);
//0x22
int sem_close (sem_t handle);
//0x23
int sem_wait (sem_t id);
//0x24
int sem_signal (sem_t id);
//0x25
int sem_timedwait(sem_t id,time_t timeout);
//0x26
int sem_trywait(sem_t id);

//0x31
typedef unsigned long time_t;
int time_sleep(time_t);

//0x41
const int EOF = -1;
char getc();
//0x42
void putc(char);


#endif //SYSCALL_C_HPP
