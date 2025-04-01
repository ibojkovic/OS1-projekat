//
// Created by os on 2/3/25.
//

#include "../h/syscall_c.hpp"

////////////////////////////////////////////// MEMORY //////////////////////////////////////////////

void* mem_alloc(size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    void* address;
	__asm__ volatile ("mv a1, %0" : : "r" (size_in_blocks));
    __asm__ volatile ("mv a0, %0" : : "r" (0x01));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (address));
    return address;
}

int mem_free(void* ptr) {
     int status;
    __asm__ volatile ("mv a1, %0" : : "r" (ptr));
    __asm__ volatile ("mv a0, %0" : : "r" (0x02));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

////////////////////////////////////////////// THREAD //////////////////////////////////////////////

int thread_create(thread_t* handle, void(*start_routine) (void*), void* arg) {
    uint64* stack = (uint64*)mem_alloc(DEFAULT_STACK_SIZE);

    if (!stack) return -1;
    int status;
    __asm__ volatile ("mv a4, %0" : : "r" (stack));
    __asm__ volatile ("mv a3, %0" : : "r" (arg));
    __asm__ volatile ("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (0x11));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int thread_exit() {
    int status;
    __asm__ volatile ("mv a0, %0" : : "r" (0x12));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

void thread_dispatch() {
    __asm__ volatile ("mv a0, %0" : : "r" (0x13));

    __asm__ volatile ("ecall");
}

////////////////////////////////////////////// CONSOLE //////////////////////////////////////////////
char getc() {
  	uint64 chr;
    __asm__ volatile ("mv a0, %0" : : "r" (0x41));

    __asm__ volatile ("ecall");

	__asm__ volatile("mv %0, a0" : "=r" (chr));
    return (char)chr;
}

void putc(char c) {
    __asm__ volatile ("mv a1, %0" : : "r" (c));
    __asm__ volatile ("mv a0, %0" : : "r" (0x42));

    __asm__ volatile ("ecall");
}

////////////////////////////////////////////// SEMAPHORE //////////////////////////////////////////////

int sem_open (sem_t* handle, unsigned init){
    int status;
    __asm__ volatile ("mv a2, %0" : : "r" (init));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (0x21));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int sem_close (sem_t handle){
    int status;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (0x22));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int sem_wait (sem_t handle){
    int status;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (0x23));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int sem_signal (sem_t handle){
    int status;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (0x24));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int sem_trywait (sem_t handle){
    int status;
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (0x26));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}