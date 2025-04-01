.global _ZN5Riscv7pushregEv
.type _ZN5Riscv7pushregEv, @function
_ZN5Riscv7pushregEv:
    addi sp, sp, -256 //posto stek raste ka nizim adrresama mi alociramo prostor tamo sto oduzimamoo od spa
    .irp index,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr
    ret

.global _ZN5Riscv6popregEv
.type _ZN5Riscv6popregEv, @function
_ZN5Riscv6popregEv:
    .irp index,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    ret
