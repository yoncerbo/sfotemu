#include "common.h"
#include "emu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static inline uint8_t imm8(const Emu *e) {
  return e->memory[e->pc + 1];
}

static inline uint16_t imm16(const Emu *e) {
  return (uint16_t)e->memory[e->pc + 1] | ((uint16_t)e->memory[e->pc + 2] << 8);
}


uint16_t Emu_load(const Emu *e, Addressing addressing) {
  switch (addressing) {
    case ADDR_IMP:
      return 0;
    case ADDR_IMM:
      return imm8(e);
    case ADDR_ZP:
      return e->memory[imm8(e)];
    case ADDR_ZPX:
      return e->memory[imm8(e) + e->x];
    case ADDR_AB:
      return e->memory[imm16(e)];
    case ADDR_ABX:
      return e->memory[imm16(e) + e->x];
    case ADDR_ABY:
      return e->memory[imm16(e) + e->y];
    default:
      fprintf(stderr, "Wrong addressing for load: %d\n", addressing);
      exit(1);
  }
}

void Emu_store(Emu *e, Addressing addressing, uint8_t value) {
  uint16_t addr = 0;
  switch (addressing) {
    case ADDR_ZP:
      addr = imm8(e);
      break;
    case ADDR_ZPX:
      addr = imm8(e) + e->x;
      break;
    case ADDR_AB:
      addr = imm16(e);
      break;
    case ADDR_ABX:
      addr = imm16(e) + e->x;
      break;
    case ADDR_ABY:
      addr = imm16(e) + e->y;
      break;
    default:
      fprintf(stderr, "Wrong addressing for load: %d\n", addressing);
      exit(1);
  }
  if (addr == PUTC_DEV) putchar(value);
  else e->memory[addr] = value;
}

static inline uint8_t set_flags_nz(Emu *e, uint8_t value) {
  CPUFlags not_affected_flags = e->flags & ~(NF | ZF);
  e->flags = not_affected_flags | (value & NF) | ((value == 0) << 1);
  return value;
}

void Emu_reset(Emu *e, Str program) {
  assert(program.len < MAX_PROG_SIZE);
  memcpy(&e->memory[PROG_ENTRY], program.ptr, program.len);
  e->pc = PROG_ENTRY;
}

void Emu_run(Emu *e, uint32_t fuel) {
  for (uint32_t i = 0; i < fuel; ++i) {
    Inst inst = INSTRUCTIONS[e->memory[e->pc]];
    if (inst.opcode == OP_NONE) {
      fprintf(stderr, "Unknown instruction: 0x%x\n", e->memory[e->pc]);
      exit(1);
    }
    switch (inst.opcode) {
      case OP_BRK:
        return;

      case OP_LDA:
        e->a = set_flags_nz(e, Emu_load(e, inst.addr));
        break;
      case OP_LDX:
        e->x = set_flags_nz(e, Emu_load(e, inst.addr));
        break;
      case OP_LDY:
        e->y = set_flags_nz(e, Emu_load(e, inst.addr));
        break;

      case OP_STA:
        Emu_store(e, inst.addr, e->a);
        break;
      case OP_STX:
        Emu_store(e, inst.addr, e->x);
        break;
      case OP_STY:
        Emu_store(e, inst.addr, e->y);
        break;
      
      case OP_TAX:
        e->x = set_flags_nz(e, e->a);
        break;
      case OP_TAY:
        e->y = set_flags_nz(e, e->a);
        break;
      case OP_TXA:
        e->a = set_flags_nz(e, e->x);
        break;
      case OP_TYA:
        e->a = set_flags_nz(e, e->y);
        break;

      default:
        fprintf(stderr, "Unknown instruction: 0x%x\n", e->memory[e->pc]);
        exit(1);
    }
    e->pc += INST_SIZE[inst.addr];
  }
}
