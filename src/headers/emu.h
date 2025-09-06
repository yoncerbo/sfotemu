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
  ADDR_ZPY,
  ADDR_AB,
  ADDR_ABX,
  ADDR_ABY,
  ADDR_IX,
  ADDR_IY,

  ADDR_COUNT,
} Addressing;

static const uint8_t INST_SIZE[ADDR_COUNT] = {
  [ADDR_IMP] = 1,
  [ADDR_IMM] = 2,
  [ADDR_ZP] = 2,
  [ADDR_ZPX] = 2,
  [ADDR_ZPY] = 2,
  [ADDR_AB] = 3,
  [ADDR_ABX] = 3,
  [ADDR_ABY] = 3,
  [ADDR_IX] = 2,
  [ADDR_IY] = 2,
};

typedef enum {
  OP_NONE = 0,

  OP_BRK,

  OP_LDA,
  OP_LDX,
  OP_LDY,

  OP_STA,
  OP_STX,
  OP_STY,

  OP_TAX,
  OP_TAY,
  OP_TXA,
  OP_TYA,
} Opcode;

typedef struct {
  Opcode opcode;
  Addressing addr;
} Inst;

static const Inst INSTRUCTIONS[256] = {
  // BRK
  [0x00] = { OP_BRK, ADDR_IMP },

  // LDA
  [0xA9] = { OP_LDA, ADDR_IMM },
  [0xA5] = { OP_LDA, ADDR_ZP },
  [0xB5] = { OP_LDA, ADDR_ZPX },
  [0xAD] = { OP_LDA, ADDR_AB },
  [0xBD] = { OP_LDA, ADDR_ABX },
  [0xB9] = { OP_LDA, ADDR_ABY },
  [0xA1] = { OP_LDA, ADDR_IX },
  [0xB1] = { OP_LDA, ADDR_IY },

  // LDX
  [0xA2] = { OP_LDX, ADDR_IMM },
  [0xA6] = { OP_LDX, ADDR_ZP },
  [0xB6] = { OP_LDX, ADDR_ZPY },
  [0xAE] = { OP_LDX, ADDR_AB },
  [0xBE] = { OP_LDX, ADDR_ABY },

  // LDY
  [0xA0] = { OP_LDY, ADDR_IMM },
  [0xA4] = { OP_LDY, ADDR_ZP },
  [0xB4] = { OP_LDY, ADDR_ZPX },
  [0xAC] = { OP_LDY, ADDR_AB },
  [0xBC] = { OP_LDY, ADDR_ABX },

  // STA
  [0x85] = { OP_STA, ADDR_ZP },
  [0x95] = { OP_STA, ADDR_ZPX },
  [0x8D] = { OP_STA, ADDR_AB },
  [0x9D] = { OP_STA, ADDR_ABX },
  [0x99] = { OP_STA, ADDR_ABY },
  [0x81] = { OP_STA, ADDR_IX },
  [0x91] = { OP_STA, ADDR_IY },

  // STX
  [0x86] = { OP_STX, ADDR_ZP },
  [0x96] = { OP_STX, ADDR_ZPY },
  [0x8E] = { OP_STX, ADDR_AB },

  // STY
  [0x84] = { OP_STY, ADDR_ZP },
  [0x94] = { OP_STY, ADDR_ZPX },
  [0x8C] = { OP_STY, ADDR_AB },

  // TAX
  [0xAA] = { OP_TAX, ADDR_IMP },

  // TAY
  [0xA8] = { OP_TAY, ADDR_IMP },

  // TXA
  [0x8A] = { OP_TXA, ADDR_IMP },

  // TYA
  [0x98] = { OP_TYA, ADDR_IMP },
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
