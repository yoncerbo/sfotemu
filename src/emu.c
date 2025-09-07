#include "common.h"
#include "emu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define STACK_BOT ((uint16_t)0x0100)

static inline uint8_t imm8(const Emu *e) {
  return e->memory[e->pc + 1];
}

static inline uint16_t imm16(const Emu *e) {
  return (uint16_t)e->memory[e->pc + 1] | ((uint16_t)e->memory[e->pc + 2] << 8);
}

static inline void Emu_branch(Emu *e) {
  e->pc += (int8_t)imm8(e);
}


uint16_t Emu_load(const Emu *e, Addressing addressing) {
  switch (addressing) {
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

static inline void Emu_stack_push(Emu *e, uint8_t value) {
  e->memory[STACK_BOT + e->sp--] = value;
}

static inline uint8_t Emu_stack_pop(Emu *e) {
  return e->memory[STACK_BOT + ++e->sp];
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
  switch (addr) {
    case PUTC_DEV:
      putchar(value);
      break;
    case PUTD_DEV:
      printf("%d\n", value);
      break;
    default:
      e->memory[addr] = value;
  }
}

static inline uint8_t set_flags_nz(Emu *e, uint8_t value) {
  CPUFlags not_affected_flags = e->flags & ~(NF | ZF);
  e->flags = not_affected_flags | (value & NF) | ((value == 0) << 1);
  return value;
}

static inline uint8_t set_flags_nzc(Emu *e, uint16_t value) {
  CPUFlags not_affected_flags = e->flags & ~(NF | ZF | CF);
  e->flags = not_affected_flags | ((uint8_t)value & NF) |
    (((uint8_t)value == 0) << 1) | (value > 255);
  return value;
}

void Emu_reset(Emu *e, Str program) {
  assert(program.len < MAX_PROG_SIZE);
  memcpy(&e->memory[PROG_ENTRY], program.ptr, program.len);
  e->pc = PROG_ENTRY;
  e->sp = 0xfe;
}

void Emu_run(Emu *e, uint32_t fuel) {
  for (uint32_t i = 0; i < fuel; ++i) {
    Inst inst = INSTRUCTIONS[e->memory[e->pc]];
    if (inst.opcode == OP_NONE) {
      fprintf(stderr, "Unknown instruction: 0x%x, offset=%d\n", e->memory[e->pc],
            e->pc - PROG_ENTRY);
      exit(1);
    }
    switch (inst.opcode) {
      case OP_BRK:
        switch (e->a) {
          case SYS_PRINT:
            uint16_t ptr = (e->y << 8) | e->x;
            printf("%s", &e->memory[ptr]);
            break;
          case SYS_EXIT:
            return;
          default:
            fprintf(stderr, "Unknown system call: %d, pc=%d\n",
                e->a, e->pc - PROG_ENTRY);
            exit(1);
        }
        break;

      // Loads and stores
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
      
      // Register transfers
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

      // Stack operations
      case OP_TSX:
        e->x = set_flags_nz(e, e->sp);
        break;
      case OP_TXS:
        e->sp = e->x;
        break;
      case OP_PHA:
        Emu_stack_push(e, e->a);
        break;
      case OP_PHP:
        Emu_stack_push(e, e->flags);
        break;
      case OP_PLA:
        e->a = set_flags_nz(e, Emu_stack_pop(e));
        break;
      case OP_PLP:
        e->flags = Emu_stack_pop(e);
        break;

      // Logical operations
      case OP_AND:
        e->a &= set_flags_nz(e, Emu_load(e, inst.addr));
        break;
      case OP_EOR:
        e->a ^= set_flags_nz(e, Emu_load(e, inst.addr));
        break;
      case OP_ORA:
        e->a |= set_flags_nz(e, Emu_load(e, inst.addr));
        break;

      case OP_CMP:
        set_flags_nzc(e, e->a - Emu_load(e, inst.addr));
        break;
      case OP_CPX:
        set_flags_nzc(e, e->x - Emu_load(e, inst.addr));
        break;
      case OP_CPY:
        set_flags_nzc(e, e->y - Emu_load(e, inst.addr));
        break;

      case OP_INC:
        Emu_store(e, inst.addr, set_flags_nz(e, Emu_load(e, inst.addr) + 1));
        break;
      case OP_INX:
        set_flags_nz(e, ++e->x);
        break;
      case OP_INY:
        set_flags_nz(e, ++e->y);
        break;

      case OP_BEQ:
        if (e->flags & ZF) Emu_branch(e);
        break;
      case OP_BNE:
        if (e->flags ^ ZF) Emu_branch(e);
        break;

      // TODO: BIT
      default:
        fprintf(stderr, "Unknown instruction: 0x%x, offset=%d\n", e->memory[e->pc],
            e->pc - PROG_ENTRY);
        exit(1);
    }
    e->pc += INST_SIZE[inst.addr];
  }
}
