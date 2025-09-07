!source "io_v1.asm"

  ldx 0

loop:
  lda hello, x
  sta PUTC
  inx
  cpx hello_len
  bne loop

  brk

hello: !text "Hello, World!",10
hello_len = 13
