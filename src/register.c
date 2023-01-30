// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Registers and utility functions
// -----------------------------------------
/// @file register.c

#include "register.h"

dispatch_register_t register_dispatch_table[] = {
    {XED_REG_EAX, {.dword = 0}},
    {XED_REG_ECX, {.dword = 0}},
    {XED_REG_EDX, {.dword = 0}},
    {XED_REG_EBX, {.dword = 0}},
    {XED_REG_ESP, {.dword = 0}},
    {XED_REG_EBP, {.dword = 0}},
    {XED_REG_ESI, {.dword = 0}},
    {XED_REG_EDI, {.dword = 0}},
    {XED_REG_EIP, {.dword = 0}},
    {XED_REG_FLAGS, {.word = 0}},
    {XED_REG_EFLAGS, {.dword = 0}},
    {XED_REG_RFLAGS, {.qword = 0}},
    {XED_REG_STACKPUSH, {.dword = 0}},
    {XED_REG_STACKPOP, {.dword = 0}}
}

uint16_t* register_build_lookup_table(profiler_result_t* profiler_result) {
    const int dispatch_table_size = sizeof(dispatch_table) / sizeof(dispatch_register_t);
    unsigned short count = profiler_result->unique_register_count;
    uint16_t register_lookup_table[XED_REG_LAST] = UINT16_MAX;
    for (int i = 0; i < count; i++) {
        xed_reg_enum_t lookup_reg = profiler_result->unique_registers[i];
        for (int j = 0; j < dispatch_table_size; j++) {
            if (dispatch_table[j]->reg == lookup_reg) {
                register_lookup_table[lookup_reg] = j;
            }
        }
    }
    return register_lookup_table;
}
