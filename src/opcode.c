// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Opcodes and utility functions
// -----------------------------------------
/// @file opcode.c

#include "opcode.h"

dispatch_opcode_t dispatch_table[] = {
    {XED_IFORM_PUSH_GPRv_50, push_GPRv_50},
    {XED_IFORM_MOV_GPRv_GPRv_89, mov_GPRv_GPRv_89},
    {XED_IFORM_MOV_GPRv_MEMv, mov_GPRv_MEMv},
    {XED_IFORM_IMUL_GPRv_GPRv, imul_GPRv_GPRv},
    {XED_IFORM_POP_GPRv_58, pop_GPRv_58},
    {XED_IFORM_RET_NEAR, ret_NEAR}
};

// const int dispatch_table_size = sizeof(dispatch_table) / sizeof(dispatch_entry_t);

// int opcode_count[dispatch_table_size] = {0};
// // simulate the results of a profiler
// opcode_count[0] = 3;
// opcode_count[1] = 5;
// opcode_count[2] = 1;

// int opcode_lookup_table[256] = {0};

// build the opcode lookup table based on the results of the profiler
// for (int i = 0; i < dispatch_table_size; i++) {
//     int count = opcode_count[i];
//     int opcode = dispatch_table[i].opcode;
//     for (int j = 0; j < count; j++) {
//         opcode_lookup_table[opcode + j] = i;
//     }
// }

// int opcode = 5;
// int index = opcode_lookup_table[opcode];
// dispatch_table[index].func();

int* opcode_build_lookup_table(profiler_result_t* profiler_result, dispatch_entry_t* dispatch_table) {

}

void push_GPRv_50(void* REG0, void* REG1, void* MEM0, uint32_t BASE0) {

}

void mov_GPRv_GPRv_89(int32_t* REG0, int32_t* REG1) {

}

void mov_GPRv_MEMv(int32_t* REG0, uint32_t* MEM0, unsigned short disp) {

}

void imul_GPRv_GPRv(int32_t* REG0, int32_t* REG1, uint64_t* RFLAGS) {

}

void pop_GPRv_58(void* REG0, void* REG1, void* MEM0, uint32_t BASE0) {

}

void ret_NEAR(void* REG0, void* REG1, void* MEM0, uint32_t BASE0) {

}