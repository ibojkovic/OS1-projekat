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
}
