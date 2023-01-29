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

profiler_result_t* profiler_init_profile() {
    profiler_result_t* result = malloc(sizeof(profiler_result_t));
    result->total_instruction_count = 0;
    result->max_reg_count = 0;
    result->unique_opcode_count = 0;
    result->unique_register_count = 0;
    result->unique_displacement_count = 0;
    for (int i = 0; i < UNIQUE_OPCODE_MAX; i++) {
        result->unique_opcodes[i].iclass = XED_ICLASS_LAST;
        result->unique_opcodes[i].iform_offset = 0;
    }
    for (int i = 0; i < UNIQUE_REGISTER_MAX; i++) {
        result->unique_registers[i] = XED_REG_LAST;
    }
    for (int i = 0; i < UNIQUE_DISPLACEMENT_MAX; i++) {
        result->unique_displacements[i] = INT64_MAX; // max displacement is 32 bits, max address calculation is 64 bits
    }
    return result;
}

void profiler_free_profile(profiler_result_t *result) {
    free(result);
}

int profiler_add_opcode(profiler_result_t *result, xed_iclass_enum_t iclass, unsigned int iform_offset) {
    if (result->unique_opcode_count < UINT16_MAX) {
        for (int i = 0; i <= result->unique_opcode_count; i++) {
            if (result->unique_opcodes[i].iclass == iclass && result->unique_opcodes[i].iform_offset == iform_offset) {
                return 1;
            }
        }
        result->unique_opcodes[result->unique_opcode_count].iclass = iclass;
        result->unique_opcodes[result->unique_opcode_count].iform_offset = iform_offset;
        result->unique_opcode_count++;
        return 0;
    } else {
        printf("Error: Too many opcodes");
        return 1;
    }
}

int profiler_add_register(profiler_result_t *result, xed_reg_enum_t reg) {
    if (result->unique_displacement_count < UINT16_MAX) {
        for (int i = 0; i <= result->unique_register_count; i++) {
            if (result->unique_registers[i] == reg) {
                return 1;
            }
        }
        result->unique_registers[result->unique_register_count] = reg;
        result->unique_register_count++;
        return 0;
    } else {
        printf("Error: Too many registers\n");
        return 1;
    }
}

int profiler_add_displacement(profiler_result_t *result, xed_int64_t disp) {
    if (result->unique_register_count < UINT16_MAX) {
        for (int i = 0; i <= result->unique_displacement_count; i++) {
            if (result->unique_displacements[i] == disp) {
                return 1;
            }
        }
        result->unique_displacements[result->unique_displacement_count] = disp;
        result->unique_displacement_count++;
        return 0;
    } else {
        printf("Error: Too many memory displacements\n");
        return 1;
    }
}

int profiler_set_max_reg_count(profiler_result_t *result, unsigned short reg_count) {
    // Compare the reg_count to the current max_reg_count and set the max_reg_count to the larger of the two
    if (reg_count > result->max_reg_count) {
        result->max_reg_count = reg_count;
        return 0;
    } else {
        return 1;
    }
}

xed_int64_t profiler_displacement_max(profiler_result_t *result) {
    xed_int64_t max = 0;
    for (int i = 0; i < result->unique_displacement_count; i++) {
        if (result->unique_displacements[i] > max) {
            max = result->unique_displacements[i];
        }
    }
    return max;
}

int profiler_process_instruction(profiler_result_t *result, xed_decoded_inst_t *xedd) {
    // Add the opcode to the result
    profiler_add_opcode(result, xed_decoded_inst_get_iclass(xedd), xed_decoded_inst_get_iform_enum_dispatch(xedd));
    const xed_inst_t* xi = xed_decoded_inst_inst(xedd);
    unsigned int noperands = xed_inst_noperands(xi);
    unsigned short reg_count = 0;
    for (int i = 0; i < noperands; i++) {
        // If the operand type is a register (from REG0 to REG9 or BASE0 to BASE1 inclusive), add it to the result
        const xed_operand_t* op = xed_inst_operand(xi,i);
        xed_operand_enum_t op_name = xed_operand_name(op);
        xed_operand_type_enum_t op_type = xed_operand_name(op);
        if (81 < op_type < 93 || 3 < op_type < 6) {
            profiler_add_register(result, xed_decoded_inst_get_reg(xedd, op_name));
            reg_count++;
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
    // Increment the total instruction count
    result->total_instruction_count++;
    // Set the max register count
    profiler_set_max_reg_count(result, reg_count);
    return 0;
}

int profiler_print_result(profiler_result_t *result) {
    printf("--Profiler Result--\n\n");
    printf("Total instructions: ");
    printf("%d\n", result->total_instruction_count);
    printf("Max registers in instruction: ");
    printf("%d\n", result->max_reg_count);
    printf("Opcodes: ");
    printf("%d\n", result->unique_opcode_count);
    printf("Opcode list:\n");
    for (int i = 0; i < result->unique_opcode_count; i++) {
        printf("%-8s (%s)\n", xed_iclass_enum_t2str(result->unique_opcodes[i].iclass), xed_iform_enum_t2str(xed_iform_first_per_iclass(result->unique_opcodes[i].iclass) + result->unique_opcodes[i].iform_offset));
    }
    printf("Registers: ");
    printf("%d\n", result->unique_register_count);
    printf("Register list:\n");
    for (int i = 0; i < result->unique_register_count; i++) {
        printf("%s\n", xed_reg_enum_t2str(result->unique_registers[i]));
    }
    printf("Displacements: ");
    printf("%d\n", result->unique_displacement_count);
    printf("Displacement list:\n");
    for (int i = 0; i < result->unique_displacement_count; i++) {
        printf("0x%02lx\n", result->unique_displacements[i]);
    }
    printf("Max Displacement: ");
    printf("0x%02lx\n", profiler_displacement_max(result));
    return 0;
}