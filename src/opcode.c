// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Opcodes and utility functions
// -----------------------------------------
/// @file opcode.c

#include "opcode.h"

dispatch_opcode_t opcode_dispatch_table[] = {
    {XED_IFORM_PUSH_GPRv_50, push_GPRv_50},
    {XED_IFORM_MOV_GPRv_GPRv_89, mov_GPRv_GPRv_89},
    {XED_IFORM_MOV_GPRv_MEMv, mov_GPRv_MEMv},
    {XED_IFORM_IMUL_GPRv_GPRv, imul_GPRv_GPRv},
    {XED_IFORM_POP_GPRv_58, pop_GPRv_58},
    {XED_IFORM_RET_NEAR, ret_NEAR}
};

int* opcode_build_lookup_table(profiler_result_t* profiler_result) {
    const int dispatch_table_size = sizeof(dispatch_table) / sizeof(dispatch_opcode_t);
    unsigned short count = profiler_result->unique_opcode_count;
    uint16_t opcode_lookup_table[XED_IFORM_LAST] = UINT16_MAX;
    for (int i = 0; i < count; i++) {
        xed_iform_enum_t lookup_op = profiler_result->unique_opcodes[i];
        for (int j = 0; j < dispatch_table_size; j++) {
            if (dispatch_table[j]->iform == lookup_op) {
                opcode_lookup_table[lookup_op] = j;
            }
        }
    return opcode_lookup_table;
}

void push_GPRv_50(void* REG0, void* REG1, void* MEM0, uint32_t BASE0) {
    // Implement
}

void mov_GPRv_GPRv_89(int32_t* REG0, int32_t* REG1) {
    // Implement
}

void mov_GPRv_MEMv(int32_t* REG0, uint32_t* MEM0, unsigned short disp) {
    // Implement
}

void imul_GPRv_GPRv(int32_t* REG0, int32_t* REG1, uint64_t* RFLAGS) {
    // Implement
}

void pop_GPRv_58(void* REG0, void* REG1, void* MEM0, uint32_t BASE0) {
    // Implement
}

void ret_NEAR(void* REG0, void* REG1, void* MEM0, uint32_t BASE0) {
    // Implement
}