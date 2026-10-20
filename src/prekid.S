.extern prekidnaRutinaC
.global interrupt
.align 4

interrupt:
    addi sp, sp, -256

    sd x1, 0x00(sp)
    sd x2, 0x16(sp)
    sd x3, 0x24(sp)
    sd x4, 0x32(sp)
    sd x5, 0x40(sp)
    sd x6, 0x48(sp)
    sd x7, 0x56(sp)
    sd x8, 0x64(sp)


    call prekidnaRutinaC

    ld x1, 0x00(sp)
    ld x2, 0x16(sp)
    ld x3, 0x24(sp)
    ld x4, 0x32(sp)
    ld x5, 0x40(sp)
    ld x6, 0x48(sp)
    ld x7, 0x56(sp)
    ld x8, 0x64(sp)


    addi sp, sp, 256

    sret