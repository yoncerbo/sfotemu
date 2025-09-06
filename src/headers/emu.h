#ifndef INCLUDE_EMU
#define INCLUDE_EMU

#include <stdint.h>

#define PROG_ENTRY ((uint16_t)0x200)
#define MAX_PROG_SIZE (64 * 1024 - PROG_ENTRY)
// TODO: only for debugging
#define PUTC_DEV ((uint16_t)0xFE10)

typedef enum {
  ADDR_IMP,
  ADDR_IMM,
  ADDR_ZP,
  ADDR_ZPX,
  ADDR_AB,
  ADDR_ABX,
  ADDR_ABY,
  ADDR_IX,
  ADDR_IY,
} Addressing;

typedef enum {
  OP_BRK,
  OP_LDA,
  OP_STA,
} Opcode;

typedef struct {
  Opcode opcode;
  Addressing addr;
  uint8_t size;
} Inst;

const Inst INSTRUCTIONS[256] = {
  [0x00] = { OP_BRK, ADDR_IMP, 1 },

  [0xA9] = { OP_LDA, ADDR_IMM, 2 },
  [0xA5] = { OP_LDA, ADDR_ZP, 2 },
  [0xB5] = { OP_LDA, ADDR_ZPX, 2 },
  [0xAD] = { OP_LDA, ADDR_AB, 3 },
  [0xBD] = { OP_LDA, ADDR_ABX, 3 },
  [0xB9] = { OP_LDA, ADDR_ABY, 3 },
  [0xA1] = { OP_LDA, ADDR_IX, 2 },
  [0xB1] = { OP_LDA, ADDR_IY, 2 },

  [0x85] = { OP_STA, ADDR_ZP, 2 },
  [0x95] = { OP_STA, ADDR_ZPX, 2 },
  [0x8D] = { OP_STA, ADDR_AB, 3 },
  [0x9D] = { OP_STA, ADDR_ABX, 3 },
  [0x99] = { OP_STA, ADDR_ABY, 3 },
  [0x81] = { OP_STA, ADDR_IX, 2 },
  [0x91] = { OP_STA, ADDR_IY, 2 },
};

typedef enum {
  CF = 1 << 0, // carry
  ZF = 1 << 1, // zero
  IF = 1 << 2, // interrupt disable
  DF = 1 << 3, // decimal mode
  BF = 1 << 4, // break command
  OF = 1 << 6, // overflow
  NF = 1 << 7, // negative
} CPUFlags;

typedef struct {
  uint16_t pc;
  uint8_t sp, a, x, y;
  CPUFlags flags;
  uint8_t memory[64 * 1024];
} Emu;

void Emu_reset(Emu *e, Str program);
void Emu_run(Emu *e, uint32_t fuel);

#endif
