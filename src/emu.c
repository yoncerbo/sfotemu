#include "emu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint8_t imm8(const Emu *e) {
  return e->memory[e->pc + 1];
}

static inline uint16_t imm16(const Emu *e) {
  return (uint16_t)e->memory[e->pc + 2] & ((uint16_t)e->memory[e->pc + 1] << 2);
}

void Emu_reset(Emu *e) {
}

void Emu_run(Emu *e, uint32_t fuel) {
  for (uint32_t i = 0; i < fuel; ++i) {
    Opcode opcode = e->memory[e->pc];
    switch (opcode) {
      case OP_LDA_IMM:
        e->a = imm8(e);
        break;
      case OP_LDA_ZP:
        e->a = e->memory[imm8(e)];
        break;
      case OP_LDA_ZPX:
        e->a = e->memory[imm8(e) + e->x];
        break;
      case OP_LDA_AB:
        e->a = e->memory[imm16(e)];
        break;
      case OP_LDA_ABX:
        e->a = e->memory[imm16(e) + (uint16_t)e->x];
        break;
      default:
        fprintf(stderr, "Unknown instruction: 0x%x\n", opcode);
        exit(1);
    }
    e->pc += INST_SIZE[opcode];
  }
}
