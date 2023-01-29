// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Utilities to adjust emulator
// -----------------------------------------
/// @file profiler.h

#ifndef PROFILER_H
#define PROFILER_H

#include <stdio.h>
#include <stdint.h>
#include "xed/xed-interface.h"

typedef struct profiler_result_s {
    unsigned short unique_register_count;           // number of unique registers
    xed_reg_enum_t unique_registers[256];           // a list of unique registers
    unsigned short unique_displacement_count;       // number of unique memory displacements
    xed_int64_t unique_displacements[256];          // a list of unique memory displacements
} profiler_result_t;

profiler_result_t* profiler_init_profile();
void profiler_free_profile(profiler_result_t *result);
int profiler_add_register(profiler_result_t *result, xed_reg_enum_t reg);
int profiler_add_displacement(profiler_result_t *result, xed_int64_t disp);
int profiler_displacement_max(profiler_result_t *result);
int profiler_process_instruction(profiler_result_t *result, xed_decoded_inst_t *xedd);
int profiler_print_result(profiler_result_t *result);

#endif