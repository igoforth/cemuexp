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

int profiler_add_register(profiler_result_t *result, xed_reg_enum_t reg) {
    if (result->unique_displacement_count < 256) {
        for (int i = 0; i < result->unique_register_count; i++) {
            if (result->unique_registers[i] == 0) {
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

int profiler_add_displacement(profiler_result_t *result, xed_int64_t disp) {
    if (result->unique_register_count < 256) {
        for (int i = 0; i < result->unique_displacement_count; i++) {
            if (result->unique_displacements[i] == 0) {
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

int profiler_displacement_max(profiler_result_t *result) {
    xed_int64_t max = 0;
    for (int i = 0; i < result->unique_displacement_count; i++) {
        if (result->unique_displacements[i] > max) {
            max = result->unique_displacements[i];
        }
    }
    return max;
}

int profiler_process_instruction(profiler_result_t *result, xed_decoded_inst_t *xedd) {
    const xed_inst_t* xi = xed_decoded_inst_inst(xedd);
    unsigned int noperands = xed_inst_noperands(xi);
    for (int i = 0; i < noperands; i++) {
        // Get operand and type
        const xed_operand_t* op = xed_inst_operand(xi,i);
        xed_operand_enum_t op_name = xed_operand_name(op);
        xed_operand_type_enum_t op_type = xed_operand_name(op);
        // if operand is register type, find register
        if (op_type == XED_OPERAND_TYPE_REG) {
            xed_reg_enum_t profiler_reg = xed_decoded_inst_get_reg(xedd, op_name);
            // add register to result
            if (profiler_add_register(result, profiler_reg)) {
                return 1;
            }
        }
    }
    unsigned int i, memops = xed_decoded_inst_number_of_memory_operands(xedd);
    for (i = 0; i < memops; i++) {
        // if instruction has displacement, width, continue
        if (xed_operand_values_has_memory_displacement(xedd)) {
            if (xed_decoded_inst_get_memory_displacement_width(xedd, i)) {
                // get displacement
                xed_int64_t profiler_disp = xed_decoded_inst_get_memory_displacement(xedd, i);
                if (profiler_add_displacement(result, profiler_disp)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int profiler_print_result(profiler_result_t *result) {
    printf("Registers: ");
    for (int i = 0; i < result->unique_register_count; i++) {
        printf("%s ", xed_reg_enum_t2str(result->unique_registers[i]));
    }
    printf("\nDisplacements: ");
    for (int i = 0; i < result->unique_displacement_count; i++) {
        printf("%ld ", result->unique_displacements[i]);
    }
    printf("\n");
    return 0;
}