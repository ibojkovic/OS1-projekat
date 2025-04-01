//
// Created by os on 5/17/24.
//

#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/tcb.hpp"
#include "../h/_sem.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"


void Riscv::handleSupervisorTrap(){
    uint64 scause = r_scause();
    if(scause == 0x9UL || scause == 0x8UL){ //call form s mode

      uint64 volatile syscall_num = Riscv::r_a0();
      uint64 volatile sepc = r_sepc();
      uint64 volatile sstatus = r_sstatus();


      switch(syscall_num){
		case 0x01: {  //mem_alloc
                size_t size;
                uint64 addr;
                __asm__ volatile("mv %0, a1" : "=r"(size));
                size = size * MEM_BLOCK_SIZE;
                addr = (uint64)MemoryAllocator::mem_alloc(size);
                __asm__ volatile ("sd %0, 80(fp)" : : "r"(addr));
                break;
        }
        case 0x02: {  //mem_free
                void* ptr;
                uint64 ret;
                __asm__ volatile ("mv %0, a1" : "=r" (ptr));
                ret = MemoryAllocator::mem_free(ptr);
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x11: {  //thread_create
                TCB* thread;
                int ret = 0;
                TCB** handle;
                TCB::Body body;
                void* arg;
                uint64* stack;
                __asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (handle));
                __asm__ volatile ("ld %0, 12 * 8(fp)" : "=r" (body));
                __asm__ volatile ("ld %0, 13 * 8(fp)" : "=r" (arg));
                __asm__ volatile ("ld %0, 14 * 8(fp)" : "=r" (stack));

                thread = TCB::createthread(body, arg, stack);
		    /*printString("thread create ");
		    printInt((uint64)thread);
		    printString("\n");*/
                if (thread) {
                    *handle = thread;  // Dodeljujemo pokazivač na kreiranu nit
                } else {
                    ret = -1; // Greška ako nije kreirana nit
                }
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x12: { //thread_exit
                int ret = 0;
                ret = TCB::thread_exit();
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x13: {  //thread_dispatch
                TCB::dispatch();
                break;
        }
        case 0x21: {  //sem_open
                _sem* sem;
                int ret = 0;
                _sem** handle;    //a1
                unsigned init;   //a2
		        __asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (handle));
		        __asm__ volatile ("ld %0, 12 * 8(fp)" : "=r" (init));
				sem = _sem::sem_open(init);
                if (sem) {
                    *handle = sem;  // Dodeljujemo pokazivač na kreiran semafor
                } else {
                    ret = -1; // Greška ako nije kreirana nit
                }
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x22: {  //sem_close
                uint64 semHandle;
                int ret = 0;
                __asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (semHandle));
                if(semHandle) {
                    ret = _sem::sem_close((sem_t)semHandle);
                }
                else ret = -2;
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x23: {  //sem_wait
                uint64 semHandle;
                int ret = 0;
                __asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (semHandle));
                if(semHandle) {
                    ret = _sem::sem_wait((sem_t)semHandle);
                }
                else ret = -2;
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x24: {  //sem_signal
                uint64 semHandle;
                int ret = 0;
                __asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (semHandle));
                if(semHandle) {
                    ret = _sem::sem_signal((sem_t)semHandle);
                }
                else ret = -2;
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x26: {  //sem_trywait
                uint64 semHandle;
                int ret = 0;
                __asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (semHandle));
                if(semHandle) {
                    ret = _sem::sem_trywait((sem_t)semHandle);
                }
                else ret = -2;
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (ret));
                break;
        }
        case 0x41: { //getc
          		char chr;
                chr = __getc();
                __asm__ volatile ("sd %0, 10 * 8(fp)" : : "r" (chr));
                break;
        }
        case 0x42: { //putc
          		char c;
				__asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (c));
                __putc(c);
                break;
        }

      }
      Riscv::w_sepc(sepc + 4);
      Riscv::w_sstatus(sstatus);
    }else if (scause == 0x8000000000000001UL){
        Riscv::mc_sip(SIP_SSIE);
    }else if (scause== 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }else
    {
        // unexpected trap cause
        printString("ERROR! SCAUSE:");
        printInt(scause);
        printString("\n");
    }

}

void Riscv::popSppSpie(){
    __asm__ __volatile__("csrw sepc, ra");
	__asm__ volatile("csrw sstatus, %0" :: "r"(0x100));

    __asm__ __volatile__("sret");
}

