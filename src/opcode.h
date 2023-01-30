// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Opcodes and utility functions
// -----------------------------------------
/// @file opcode.h

#ifndef OPCODE_H
#define OPCODE_H

#include "cpu.h"

typedef struct dispatch_opcode_s {
    xed_iform_enum_t iform;
    void (*func)(void);
} dispatch_opcode_t;

enum ScaleFactor {
    times_1 = 0,
    times_2 = 1,
    times_4 = 2,
    times_8 = 3,
    times_int_size = times_4,

    times_half_system_pointer_size = times_2,
    times_system_pointer_size = times_4,

    times_tagged_size = times_4,
};

int* opcode_build_lookup_table(profiler_result_t* profiler_result, dispatch_entry_t* dispatch_table);

// Summary: "Push Word, Doubleword or Quadword Onto the Stack"
// Operand 1 (r): Register (RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15)
// Operand 2 (r/w, suppressed): Register (RSP)
// Operand 3 (w, suppressed): Memory
void push_GPRv_50(void* REG0, void* REG1, void* MEM0, uint32_t BASE0);

// Summary: "Move"
// Operand 1 (w): Register (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D)
// Operand 2 (r): Register (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D)
void mov_GPRv_GPRv_89(int32_t* REG0, int32_t* REG1); // 89+rb

// Summary: "Move"
// Operand 1 (w): Register (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D)
// Operand 2 (r): Memory
void mov_GPRv_MEMv(int32_t* REG0, uint32_t* MEM0, unsigned short disp);

// Summary: "Signed Multiply"
// Operand 1 (r/w): Register (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D)
// Operand 2 (r): Register (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D)
// Operand 3 (w, suppressed): Flags (AF: undef, CF: w, OF: w, PF: undef, SF: undef, ZF: undef)
void imul_GPRv_GPRv(int32_t* REG0, int32_t* REG1, uint64_t* RFLAGS);

// Summary: "Pop a Value from the Stack"
// Operand 1 (w): Register (RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15)
// Operand 2 (r/w, suppressed): Register (RSP)
// Operand 3 (r, suppressed): Memory
void pop_GPRv_58(void* REG0, void* REG1, void* MEM0, uint32_t BASE0);

// Summary: "Return from Procedure"
// Operand 1 (r/w, suppressed): Register (RSP)
// Operand 2 (w, suppressed): Register (RIP)
// Operand 3 (r, suppressed): Memory
void ret_NEAR(void* REG0, void* REG1, void* MEM0, uint32_t BASE0);

#endif