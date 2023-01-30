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
    state->memory = malloc(MAX_MEMORY);
    state->registers = malloc(sizeof(dispatch_register_t) * NUM_REGISTERS);
    state->opcode_lookup_table = opcode_build_lookup_table(profiler_result, dispatch_table);
    state->register_lookup_table = register_build_lookup_table(profiler_result, dispatch_table);
    return state;
}

void emulator_free(emulator_state_t* state) {
    free(state->memory);
    free(state->registers);
    free(state->opcode_lookup_table);
    free(state->register_lookup_table);
    free(state);
}

int emulator_process_instruction(emulator_state_t* state, xed_decoded_inst_t* xedd) {
    xed_uint_t opcode = xed_decoded_inst_get_iclass(xedd);
    if(opcode >= MAX_INSTRUCTIONS) {
        printf("Opcode %d not implemented");
}

void emulator_print_state(emulator_state_t* state) {
    
}
