!source "io_v1.asm"

  ldx #0

loop:
  inx
  stx PUTD
  cpx #10
  bne loop
