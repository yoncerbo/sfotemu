!source "io_v1.asm"

lda #SYS_PRINT
ldx #<hello
ldy #>hello
brk

lda #SYS_EXIT
brk

hello: !text "Hello, World!",10,0
