#include <assert.h>

#include "emu.c"
#include "emu.h"
#include "stdlib.h"

int main(void) {
  Emu *e = malloc(sizeof *e);

  Emu_reset(e);

  e->memory[0] = OP_LDA_IMM;
  e->memory[1] = 0xcd;
  Emu_run(e, 1);
  assert(e->a == 0xcd);

  return 0;
}
