!source "io_v1.asm"

  ldx #0
  cpx #1
  bne else
  ldx #1
  stx PUTD
  lda #SYS_EXIT
  brk
else:
  ldx #0
  stx PUTD
  lda #SYS_EXIT
  brk
