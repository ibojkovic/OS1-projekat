//
// Created by os on 5/17/24.
//

#include "../h/print.hpp"
#include "../lib/console.h"

void printStringMoj(char const *string){
    while(*string != '\0'){
        __putc(*string);
        string++;
    }
}

void printIntMoj(uint64 value) {
    char buffer[20]; // Maksimalna veličina uint64 broja je 20 cifara
    int i = 0;

    // Ako je broj 0, direktno ga ispišemo
    if (value == 0) {
        __putc('0'); // Funkcija za ispis jednog karaktera
        return;
    }

    // Konvertovanje broja u niz karaktera unazad
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Ispis broja u pravilnom redosledu
    while (i > 0) {
        __putc(buffer[--i]);
    }
}

void printHexMoj(uint64 value) {
    char hexChars[] = "0123456789ABCDEF";
    __putc('0'); __putc('x'); // Prefiks za heksadecimalne brojeve

    for (int i = 15; i >= 0; i--) {
        uint8 nibble = (value >> (i * 4)) & 0xF; // Uzmi po 4 bita
        __putc(hexChars[nibble]);
    }
}

