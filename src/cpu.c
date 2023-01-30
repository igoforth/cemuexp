// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// 32-bit CPU emulating a bit of x86 ISA
// -----------------------------------------
/// @file cpu.c

#include "cpu.h"
#include "opcode.h"
#include "register.h"

#define MAX_INSTRUCTIONS 1000
#define MAX_MEMORY 1000

emulator_state_t* emulator_init(profiler_result_t* profiler_result) {
    emulator_state_t* state = malloc(sizeof(emulator_state_t));
    if (!state->memory = emulator_allocate_memory(profiler_result)) {
        printf("Error allocating memory\n");
        return NULL;
    }
    if (!state->opcode_lookup_table = opcode_build_lookup_table(profiler_result)) {
        printf("Error building opcode lookup table\n");
        return NULL;
    }
    if (!state->register_lookup_table = register_build_lookup_table(profiler_result)) {
        printf("Error building register lookup table\n");
        return NULL;
    }
    return state;
}

void emulator_free(emulator_state_t* state) {
    free(state->memory);
    free(state->registers);
    free(state->opcode_lookup_table);
    free(state->register_lookup_table);
    free(state);
}

// Allocate memory
char* emulator_allocate_memory(profiler_result_t* profiler_result) {
    unsigned int mem_size = 8 * profiler_result->total_instruction_count * profiler_displacement_max(profiler_result);
    if (mem_size > MAX_MEMORY) {
        printf("Memory size %d exceeds maximum memory size %d\n", mem_size, MAX_MEMORY);
        return NULL;
    }
    return malloc(MAX_MEMORY);
}

int emulator_process_instruction(emulator_state_t* state, xed_decoded_inst_t* xedd) {
    xed_iform_enum_t opcode = xed_decoded_inst_get_iform_enum(xedd);
    if(state->opcode_lookup_table[opcode] == UINT16_MAX) {
        printf("Opcode %d not implemented");
        return 1;
    }
    // Fix this to make some sort of sense
    opcode_function_t opcode_function = opcode_dispatch_table[state->opcode_lookup_table[opcode]](xedd);
}

void emulator_print_state(emulator_state_t* state) {
    // Implement this
}


