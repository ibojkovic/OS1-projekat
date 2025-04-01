//
// Created by os on 8/13/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"

FreeMem* MemoryAllocator::fmem_head = nullptr;

void *MemoryAllocator::mem_alloc(size_t size) {

    if(fmem_head == nullptr){
        fmem_head = (FreeMem*)((uint64) HEAP_START_ADDR);
        fmem_head->next = nullptr;
        fmem_head->prev = nullptr;
        fmem_head->size = ((uint64) HEAP_END_ADDR) - ((uint64) HEAP_START_ADDR) - sizeof(FreeMem);
    }

    if (size == 0) {
        return nullptr;
    }

    size = (size % MEM_BLOCK_SIZE) ? (((size / MEM_BLOCK_SIZE) + 1) * MEM_BLOCK_SIZE) : size;

    for(FreeMem* cur = fmem_head; cur!=nullptr; cur = cur->next){
        if(cur->size < size) continue;
        //nasao je slobodan fragmentt
        //ima jos slobodnog preostalog prostora koji je veci od minimalnog sizeof(freemem)
        if(cur->size - size >= sizeof(FreeMem)){
	    FreeMem* newfrgm = (FreeMem*)((char*)cur + size + sizeof(FreeMem));

            if(cur->prev) cur->prev->next = newfrgm;
            else fmem_head = newfrgm;
            if(cur->next) cur->next->prev = newfrgm;
            newfrgm->next = cur->next;
            newfrgm->prev = cur->prev;
            newfrgm->size = cur->size - size;
			cur->size = size;
        }else{
            if(cur->prev) cur->prev->next = cur->next;
            else fmem_head = cur->next;
            if(cur->next) cur->next->prev = cur->prev;
        }

        return (void*)((char*)cur + sizeof(FreeMem));
    }

    return nullptr;

}


int MemoryAllocator::mem_free(void *adr) {


  if(adr==nullptr || adr < HEAP_START_ADDR || adr > HEAP_END_ADDR) return -1;
  FreeMem* cur;
  if(!fmem_head || (char*)adr < (char*)fmem_head) cur = 0;
  else for(cur = fmem_head; cur->next!=0 && (char*)adr>(char*)(cur->next); cur = cur->next){}

  FreeMem* new_frgm = (FreeMem*)((char*)adr - sizeof(FreeMem)); //-sizeof da bismo dosli do pocetka strk koja se mora osloboditi

   if (new_frgm->freed) {
       return -1;
   }
   new_frgm->prev = cur;
   new_frgm->freed = true;
   if(cur)
     new_frgm->next = cur->next;
   else {
     new_frgm->next = fmem_head;
   }

   if(new_frgm->next)
     new_frgm->next->prev = new_frgm;

   if(cur)
     cur->next = new_frgm;
   else
     fmem_head = new_frgm;


   tryToJoin(new_frgm);
   tryToJoin(cur);



   return 0;

}

void MemoryAllocator::tryToJoin(FreeMem* cur) {
  if(!cur) return;
    if(cur->next && (char*)(cur->next)==((char*)cur + cur->size  + sizeof(FreeMem))){

        cur->size += cur->next->size + sizeof(FreeMem) ;
        cur->next = cur->next->next;
        if(cur->next)cur->next->prev=cur;
    }
}/*
#include "../h/MemoryAllocator.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/print.hpp"

FreeMem* MemoryAllocator::fmem_head = nullptr;
void *MemoryAllocator::mem_alloc(size_t size) {
    printString("Requested size: ");
    printInt((uint64)size);
    printString("\n");

    if (fmem_head == nullptr) {
        fmem_head = (FreeMem*)((uint64)HEAP_START_ADDR);
        fmem_head->next = nullptr;
        fmem_head->prev = nullptr;
        fmem_head->size = ((uint64)HEAP_END_ADDR) - ((uint64)HEAP_START_ADDR) - sizeof(FreeMem);
    }

    if (size == 0) return nullptr;

    // Zaokruživanje na MEM_BLOCK_SIZE
    size = (size % MEM_BLOCK_SIZE) ? (((size / MEM_BLOCK_SIZE) + 1) * MEM_BLOCK_SIZE) : size;

    FreeMem* best_fit = nullptr;
    for (FreeMem* cur = fmem_head; cur != nullptr; cur = cur->next) {
        if (cur->size >= size) {  // Proveri da li segment može da primi zahtevanu veličinu
            if (!best_fit || cur->size < best_fit->size) {
                best_fit = cur;  // Nađi najmanji odgovarajući segment
            }
        }
    }

    if (!best_fit) return nullptr;  // Ako nema dovoljno velikog segmenta, vraćamo nullptr

    void* allocated_address = (void*)((char*)best_fit + sizeof(FreeMem));

    // Ako ostaje dovoljno prostora, kreiraj novi slobodan fragment
    if (best_fit->size - size >= sizeof(FreeMem)) {
        FreeMem* new_frag = (FreeMem*)((char*)best_fit + size + sizeof(FreeMem));
        new_frag->size = best_fit->size - size - sizeof(FreeMem);
        new_frag->next = best_fit->next;
        new_frag->prev = best_fit->prev;

        if (best_fit->prev) best_fit->prev->next = new_frag;
        else fmem_head = new_frag;

        if (best_fit->next) best_fit->next->prev = new_frag;
    } else {
        // Ako ne ostaje dovoljno mesta za novi slobodan blok, koristimo ceo segment
        if (best_fit->prev) best_fit->prev->next = best_fit->next;
        else fmem_head = best_fit->next;

        if (best_fit->next) best_fit->next->prev = best_fit->prev;
    }

    return allocated_address;
}


int MemoryAllocator::mem_free(void *adr) {
    if (adr == nullptr || adr < HEAP_START_ADDR || adr > HEAP_END_ADDR) return -1;

    FreeMem* new_frgm = (FreeMem*)((char*)adr - sizeof(FreeMem));

    if (new_frgm->freed) return -1;

    new_frgm->freed = true;

    if (!fmem_head || (char*)adr < (char*)fmem_head) {
        new_frgm->next = fmem_head;
        if (fmem_head) fmem_head->prev = new_frgm;
        fmem_head = new_frgm;
    } else {
        FreeMem* cur;
        for (cur = fmem_head; cur->next && (char*)adr > (char*)(cur->next); cur = cur->next);

        new_frgm->next = cur->next;
        new_frgm->prev = cur;
        if (cur->next) cur->next->prev = new_frgm;
        cur->next = new_frgm;
    }

    tryToJoin(new_frgm);
    if (new_frgm->prev) tryToJoin(new_frgm->prev);

    return 0;
}

void MemoryAllocator::tryToJoin(FreeMem* cur) {
    if (cur->next && (char*)cur->next == (char*)cur + cur->size + sizeof(FreeMem)) {
        cur->size += cur->next->size + sizeof(FreeMem);
        cur->next = cur->next->next;
        if (cur->next) cur->next->prev = cur;
    }
}
*/
