#ifndef INCLUDE_EMU
#define INCLUDE_EMU

#include <stdint.h>

typedef enum {
  OP_LDA_IMM  = 0xA9,
  OP_LDA_ZP   = 0xA5,
  OP_LDA_ZPX  = 0xB5,
  OP_LDA_AB   = 0xAD,
  OP_LDA_ABX  = 0xBD,
  OP_LDA_ABY  = 0xB9,
  OP_LDA_IX   = 0xA1,
  OP_LDA_IY   = 0xB1,

  OP_COUNT = 255,
} Opcode;

typedef enum {
  CF = 1 << 0, // carry
  ZF = 1 << 1, // zero
  IF = 1 << 2, // interrupt disable
  DF = 1 << 3, // decimal mode
  BF = 1 << 4, // break command
  OF = 1 << 6, // overflow
  NF = 1 << 7, // negative
} CPUFlags;

const uint8_t INST_SIZE[OP_COUNT] = {
  [OP_LDA_IMM]  = 2,
  [OP_LDA_ZP]   = 2,
  [OP_LDA_ZPX] = 2,
  [OP_LDA_AB]  = 3,
  [OP_LDA_ABX] = 3,
  [OP_LDA_ABY] = 3,
  [OP_LDA_IX]  = 2,
  [OP_LDA_IY]  = 2,
};

typedef struct {
  uint16_t pc;
  uint8_t sp, a, x, y;
  CPUFlags flags;
  uint8_t memory[64 * 1024];
} Emu;

void Emu_reset(Emu *e);
void Emu_run(Emu *e, uint32_t fuel);

#endif
