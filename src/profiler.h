// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Bytecode analysis and profiling
// -----------------------------------------
/// @file profiler.h

#ifndef PROFILER_H
#define PROFILER_H

#include <stdio.h>
#include <stdint.h>
#include "xed/xed-interface.h"

#define UNIQUE_OPCODE_MAX XED_ICLASS_LAST
#define UNIQUE_REGISTER_MAX XED_REG_LAST
#define UNIQUE_DISPLACEMENT_MAX 256

typedef struct opcode_s {
    xed_iclass_enum_t iclass;
    unsigned int iform_offset;
} opcode_t;

typedef struct profiler_result_s {
    unsigned short total_instruction_count;                     // total number of instructions
    unsigned short max_reg_count;                               // maximum number of registers in an instruction
    unsigned short unique_opcode_count;                         // number of unique opcodes
    opcode_t unique_opcodes[UNIQUE_OPCODE_MAX];                 // a list of unique opcodes [iclass][iform offset]
    unsigned short unique_register_count;                       // number of unique registers
    xed_reg_enum_t unique_registers[UNIQUE_REGISTER_MAX];       // a list of unique registers
    unsigned short unique_displacement_count;                   // number of unique memory displacements
    xed_int64_t unique_displacements[UNIQUE_DISPLACEMENT_MAX];  // a list of unique memory displacements
} profiler_result_t;

// Initialize a new profile with max values because some unique registers and displacements may be 0
profiler_result_t* profiler_init_profile();

// Free the profile from the heap
void profiler_free_profile(profiler_result_t *result);

// Add the opcode to our profile if it is unique and our profile is not full
int profiler_add_opcode(profiler_result_t *result, xed_iclass_enum_t iclass, unsigned int iform_offset);

// Add the register to our profile if it is unique and our profile is not full
int profiler_add_register(profiler_result_t *result, xed_reg_enum_t reg);

// Add the displacement to our profile if it is unique and our profile is not full
int profiler_add_displacement(profiler_result_t *result, xed_int64_t disp);

// Set the maximum number of registers in an instruction
int profiler_set_max_reg_count(profiler_result_t *result, unsigned short reg_count);

// Get the maximum displacement in the profile
xed_int64_t profiler_displacement_max(profiler_result_t *result);

// Process the decoded instruction and add any registers or displacements to the profile
int profiler_process_instruction(profiler_result_t *result, xed_decoded_inst_t *xedd);

// Prints the profile to stdout
int profiler_print_result(profiler_result_t *result);

#endif