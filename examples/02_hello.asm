!source "io_v1.asm"

lda #'H'
sta PUTC
lda #'e'
sta PUTC
lda #'l'
sta PUTC
lda #'l'
sta PUTC
lda #'o'
sta PUTC
lda #10
sta PUTC
brk
