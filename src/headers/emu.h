#ifndef INCLUDE_EMU
#define INCLUDE_EMU

#include <stdint.h>

#define PROG_ENTRY ((uint16_t)0x200)
#define MAX_PROG_SIZE (64 * 1024 - PROG_ENTRY)
// TODO: only for debugging
#define PUTC_DEV ((uint16_t)0xFE10)

typedef enum {
  ADDR_ACC,
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
  ADDR_IND,
  ADDR_REL,

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

// http://www.6502.org/users/obelisk/6502/instructions.html
typedef enum {
  OP_NONE = 0,

  // Load/store operations
  OP_LDA, OP_LDX, OP_LDY, OP_STA, OP_STX, OP_STY,

  // Register transfers
  OP_TAX, OP_TAY, OP_TXA, OP_TYA,

  // Stack operations
  OP_TSX, OP_TXS, OP_PHA, OP_PHP, OP_PLA, OP_PLP,

  // Logical
  OP_AND, OP_EOR, OP_ORA, OP_BIT,

  // Arithmetic
  OP_ADC, OP_SBC, OP_CMP, OP_CPX, OP_CPY,

  // Increments and decrments
  OP_INC, OP_INX, OP_INY, OP_DEC, OP_DEX, OP_DEY,

  // Shifts
  OP_ASL, OP_LSR, OP_ROL, OP_ROR,

  // Jumps
  OP_JMP, OP_JSR, OP_RTS,

  // Branches
  OP_BCC, OP_BCS, OP_BEQ, OP_BMI,
  OP_BNE, OP_BPL, OP_BVC, OP_BVS,

  // Status flags
  OP_CLC, OP_CLD, OP_CLI, OP_CLV, OP_SEC, OP_SED, OP_SEI,

  // System
  OP_BRK, OP_NOP, OP_RTI,
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

  // Register transfers
  [0xAA] = { OP_TAX, ADDR_IMP },
  [0xA8] = { OP_TAY, ADDR_IMP },
  [0x8A] = { OP_TXA, ADDR_IMP },
  [0x98] = { OP_TYA, ADDR_IMP },

  // Stack operations
  [0xBA] = { OP_TSX, ADDR_IMP },
  [0x9A] = { OP_TXS, ADDR_IMP },
  [0x48] = { OP_PHA, ADDR_IMP },
  [0x08] = { OP_PHP, ADDR_IMP },
  [0x68] = { OP_PLA, ADDR_IMP },
  [0x28] = { OP_PLP, ADDR_IMP },

  // AND
  [0x29] = { OP_AND, ADDR_IMM },
  [0x25] = { OP_AND, ADDR_ZP },
  [0x35] = { OP_AND, ADDR_ZPX },
  [0x2D] = { OP_AND, ADDR_AB },
  [0x3D] = { OP_AND, ADDR_ABX },
  [0x39] = { OP_AND, ADDR_ABY },
  [0x21] = { OP_AND, ADDR_IX },
  [0x31] = { OP_AND, ADDR_IY },

  // EOR
  [0x49] = { OP_EOR, ADDR_IMM },
  [0x45] = { OP_EOR, ADDR_ZP },
  [0x55] = { OP_EOR, ADDR_ZPX },
  [0x4D] = { OP_EOR, ADDR_AB },
  [0x5D] = { OP_EOR, ADDR_ABX },
  [0x59] = { OP_EOR, ADDR_ABY },
  [0x41] = { OP_EOR, ADDR_IX },
  [0x51] = { OP_EOR, ADDR_IY },

  // ORA
  [0x09] = { OP_ORA, ADDR_IMM },
  [0x05] = { OP_ORA, ADDR_ZP },
  [0x15] = { OP_ORA, ADDR_ZPX },
  [0x0D] = { OP_ORA, ADDR_AB },
  [0x1D] = { OP_ORA, ADDR_ABX },
  [0x19] = { OP_ORA, ADDR_ABY },
  [0x01] = { OP_ORA, ADDR_IX },
  [0x11] = { OP_ORA, ADDR_IY },

  // BIT
  [0x24] = { OP_BIT, ADDR_ZP },
  [0x2C] = { OP_BIT, ADDR_AB },

  // ADC
  [0x69] = { OP_ADC, ADDR_IMM },
  [0x65] = { OP_ADC, ADDR_ZP },
  [0x75] = { OP_ADC, ADDR_ZPX },
  [0x6D] = { OP_ADC, ADDR_AB },
  [0x7D] = { OP_ADC, ADDR_ABX },
  [0x79] = { OP_ADC, ADDR_ABY },
  [0x61] = { OP_ADC, ADDR_IX },
  [0x71] = { OP_ADC, ADDR_IY },

  // SBC
  [0xE9] = { OP_SBC, ADDR_IMM },
  [0xE5] = { OP_SBC, ADDR_ZP },
  [0xF5] = { OP_SBC, ADDR_ZPX },
  [0xED] = { OP_SBC, ADDR_AB },
  [0xFD] = { OP_SBC, ADDR_ABX },
  [0xF9] = { OP_SBC, ADDR_ABY },
  [0xE1] = { OP_SBC, ADDR_IX },
  [0xF1] = { OP_SBC, ADDR_IY },

  // CMP
  [0xC9] = { OP_CMP, ADDR_IMM },
  [0xC5] = { OP_CMP, ADDR_ZP },
  [0xD5] = { OP_CMP, ADDR_ZPX },
  [0xCD] = { OP_CMP, ADDR_AB },
  [0xDD] = { OP_CMP, ADDR_ABX },
  [0xD9] = { OP_CMP, ADDR_ABY },
  [0xC1] = { OP_CMP, ADDR_IX },
  [0xD1] = { OP_CMP, ADDR_IY },

  // CPX
  [0xE0] = { OP_CPX, ADDR_IMM },
  [0xE4] = { OP_CPX, ADDR_ZP },
  [0xEC] = { OP_CPX, ADDR_AB },

  // CPY
  [0xC0] = { OP_CPY, ADDR_IMM },
  [0xC4] = { OP_CPY, ADDR_ZP },
  [0xCC] = { OP_CPY, ADDR_AB },

  // INC - increment memory
  [0xE6] = { OP_INC, ADDR_ZP },
  [0xF6] = { OP_INC, ADDR_ZPX },
  [0xEE] = { OP_INC, ADDR_AB },
  [0xFE] = { OP_INC, ADDR_ABX },

  [0xE8] = { OP_INX, ADDR_IMP },
  [0xC8] = { OP_INY, ADDR_IMP },

  // DEC - decrement memory
  [0xC6] = { OP_DEC, ADDR_ZP },
  [0xD6] = { OP_DEC, ADDR_ZPX },
  [0xCE] = { OP_DEC, ADDR_AB },
  [0xDE] = { OP_DEC, ADDR_ABX },

  [0xCA] = { OP_DEX, ADDR_IMP },
  [0x88] = { OP_DEY, ADDR_IMP },

  // ASL - arithmetic shift left
  [0x0A] = { OP_ASL, ADDR_ACC },
  [0x06] = { OP_ASL, ADDR_ZP },
  [0x16] = { OP_ASL, ADDR_ZPX },
  [0x0E] = { OP_ASL, ADDR_AB },
  [0x1E] = { OP_ASL, ADDR_ABX },

  // LSR
  [0x4A] = { OP_LSR, ADDR_ACC },
  [0x46] = { OP_LSR, ADDR_ZP },
  [0x56] = { OP_LSR, ADDR_ZPX },
  [0x4E] = { OP_LSR, ADDR_AB },
  [0x5E] = { OP_LSR, ADDR_ABX },

  // ROL
  [0x2A] = { OP_ROL, ADDR_ACC },
  [0x26] = { OP_ROL, ADDR_ZP },
  [0x36] = { OP_ROL, ADDR_ZPX },
  [0x2E] = { OP_ROL, ADDR_AB },
  [0x3E] = { OP_ROL, ADDR_ABX },

  // ROR
  [0x6A] = { OP_ROR, ADDR_ACC },
  [0x66] = { OP_ROR, ADDR_ZP },
  [0x76] = { OP_ROR, ADDR_ZPX },
  [0x6E] = { OP_ROR, ADDR_AB },
  [0x7E] = { OP_ROR, ADDR_ABX },

  // JMP
  [0x4C] = { OP_JMP, ADDR_AB },
  [0x6C] = { OP_JMP, ADDR_IND },

  [0x20] = { OP_JSR, ADDR_AB },
  [0x60] = { OP_RTS, ADDR_IMP },

  // Branches
  [0x90] = { OP_BCC, ADDR_REL },
  [0xB0] = { OP_BCS, ADDR_REL },
  [0xF0] = { OP_BEQ, ADDR_REL },
  [0x30] = { OP_BMI, ADDR_REL },
  [0xD0] = { OP_BNE, ADDR_REL },
  [0x10] = { OP_BPL, ADDR_REL },
  [0x50] = { OP_BVC, ADDR_REL },
  [0x70] = { OP_BVS, ADDR_REL },

  // Flags
  [0x18] = { OP_CLC, ADDR_IMP },
  [0xD8] = { OP_CLD, ADDR_IMP },
  [0x58] = { OP_CLI, ADDR_IMP },
  [0xB8] = { OP_CLV, ADDR_IMP },
  [0x38] = { OP_SEC, ADDR_IMP },
  [0xF8] = { OP_SED, ADDR_IMP },
  [0x78] = { OP_SEI, ADDR_IMP },

  // System
  [0x00] = { OP_BRK, ADDR_IMP },
  [0xEA] = { OP_NOP, ADDR_IMP },
  [0x40] = { OP_RTI, ADDR_IMP },
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
