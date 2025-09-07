!source "io_v1.asm"

ldx #0

lda hello, x
sta PUTC
inx

lda hello, x
sta PUTC
inx

lda hello, x
sta PUTC
inx

lda hello, x
sta PUTC
inx

lda #10
sta PUTC

brk

hello !text "Hello",10
