// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Registers and utility functions
// -----------------------------------------
/// @file register.c

#include "register.h"

dispatch_register_t dispatch_table[] = {
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

int* register_build_lookup_table(profiler_result_t* profiler_result, dispatch_entry_t* dispatch_table) {

}
