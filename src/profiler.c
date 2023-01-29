// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Utilities to adjust emulator
// -----------------------------------------
/// @file profiler.c

#include <stdio.h>
#include <stdlib.h>
#include "profiler.h"

// Initialize a new profile with max values because some unique registers and displacements may be 0
profiler_result_t* profiler_init_profile() {
    profiler_result_t* result = malloc(sizeof(profiler_result_t));
    result->unique_register_count = 0;
    result->unique_displacement_count = 0;
    for (int i = 0; i < 256; i++) {
        result->unique_registers[i] = XED_REG_LAST;
        result->unique_displacements[i] = INT64_MAX;
    }
    return result;
}

void profiler_free_profile(profiler_result_t *result) {
    free(result);
}

// Add the register to our profile if it is unique and our profile is not full
int profiler_add_register(profiler_result_t *result, xed_reg_enum_t reg) {
    if (result->unique_displacement_count < UINT16_MAX) {
        for (int i = 0; i <= result->unique_register_count; i++) {
            if (result->unique_registers[i] == XED_REG_LAST) {
                result->unique_register_count++;
                result->unique_registers[i] = reg;
                return 0;
            }
            if (result->unique_registers[i] == reg) {
                return 1;
            }
        }
    } else {
        printf("Error: Too many registers\n");
        return 1;
    }
}

// Add the displacement to our profile if it is unique and our profile is not full
int profiler_add_displacement(profiler_result_t *result, xed_int64_t disp) {
    if (result->unique_register_count < UINT16_MAX) {
        for (int i = 0; i <= result->unique_displacement_count; i++) {
            if (result->unique_displacements[i] == INT64_MAX) {
                result->unique_displacement_count++;
                result->unique_displacements[i] = disp;
                return 0;
            }
            if (result->unique_displacements[i] == disp) {
                return 1;
            }
        }
    } else {
        printf("Error: Too many memory displacements\n");
        return 1;
    }
}

// Get the maximum displacement in the profile
int profiler_displacement_max(profiler_result_t *result) {
    xed_int64_t max = 0;
    for (int i = 0; i < result->unique_displacement_count; i++) {
        if (result->unique_displacements[i] > max) {
            max = result->unique_displacements[i];
        }
    }
    return max;
}

// Process the decoded instruction and add any registers or displacements to the profile
int profiler_process_instruction(profiler_result_t *result, xed_decoded_inst_t *xedd) {
    const xed_inst_t* xi = xed_decoded_inst_inst(xedd);
    unsigned int noperands = xed_inst_noperands(xi);
    for (int i = 0; i < noperands; i++) {
        // If the operand type is a register, add it to the result
        const xed_operand_t* op = xed_inst_operand(xi,i);
        xed_operand_enum_t op_name = xed_operand_name(op);
        xed_operand_type_enum_t op_type = xed_operand_name(op);
        if (81 < op_type < 93) {
            profiler_add_register(result, xed_decoded_inst_get_reg(xedd, op_name));
        }
    }
    unsigned int i, memops = xed_decoded_inst_number_of_memory_operands(xedd);
    for (i = 0; i < memops; i++) {
        // If instruction has displacement and width, get the displacement and add it to result
        if (xed_operand_values_has_memory_displacement(xedd)) {
            if (xed_decoded_inst_get_memory_displacement_width(xedd, i)) {
                profiler_add_displacement(result, xed_decoded_inst_get_memory_displacement(xedd, i));
            }
        }
    }
    return 0;
}

int profiler_print_result(profiler_result_t *result) {
    printf("Registers: ");
    printf("%d\n", result->unique_register_count);
    printf("Displacements: ");
    printf("%d\n", result->unique_displacement_count);
    printf("Max Displacement: ");
    printf("%d\n", profiler_displacement_max(result));
    return 0;
}