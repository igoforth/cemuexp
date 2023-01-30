// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Main program
// -----------------------------------------
/// @file main.c

#include "xed/xed-interface.h"
#include "xed-examples-util.h"
#include "profiler.h"
#include "cpu.h"
#include <sys/mman.h>

#ifndef FALSE
#define FALSE (0!=0)
#define TRUE  (0==0)
#endif

#define BUFLEN  1000

int main(int argc, char** argv);

int decode(xed_decoded_inst_t* xedd, xed_uint8_t* itext, int offset, int line_size, xed_machine_mode_enum_t mmode, xed_address_width_enum_t stack_addr_width) {
    xed_error_enum_t xed_error;
    xed_decoded_inst_zero(xedd);
    xed_decoded_inst_set_mode(xedd, mmode, stack_addr_width);
    xed_error = xed_decode(xedd, &itext[offset], line_size);
    if (xed_error != XED_ERROR_NONE) {
        printf("Decoding error\n");
        return 1;
    }
    return 0;
}

int print(xed_decoded_inst_t* xedd, char* buffer, int offset, int line_size, xed_uint8_t* itext) {
    xed_bool_t ok;
    ok = xed_format_context(XED_SYNTAX_INTEL, xedd, buffer, BUFLEN, 0, 0, 0);
    if (ok) {
        // Print yellow
        printf("\033[0;33m");
        printf("%-8d", offset + line_size);
        int i;
        for(i = 0; i < line_size; i++) {
            printf("%02x ", itext[offset + i]);
        }
        for(int j = 0; j < 23 - (i * 3); j++) {
            printf(" ");
        }
        printf("%s\n", buffer);
        // Reset color
        printf("\033[0m");
        xed_decoded_inst_dump(xedd, buffer, BUFLEN);
        printf("%s\n", buffer);
    }
    else {
        printf("Error disassembling\n");
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    xed_error_enum_t xed_error;
    xed_decoded_inst_t xedd;
    char buffer[BUFLEN];
    xed_bool_t ok;
    xed_machine_mode_enum_t mmode;
    xed_address_width_enum_t stack_addr_width;
    profiler_result_t* result = profiler_init_profile();

    // Assembly
    // square:
    // push   ebp
    // mov    ebp,esp
    // mov    eax,DWORD PTR [ebp+0x8]
    // imul   eax,eax
    // pop    ebp
    // ret    

    // Disassembly
    // 0:  55                      push   ebp
    // 1:  89 e5                   mov    ebp,esp
    // 3:  8b 45 08                mov    eax,DWORD PTR [ebp+0x8]
    // 6:  0f af c0                imul   eax,eax
    // 9:  5d                      pop    ebp
    // a:  c3                      ret

    // Bytecode
    // 5589E58B45080FAFC05DC3

    // Write bytecode to itext
    xed_uint8_t itext[XED_MAX_INSTRUCTION_BYTES];
    unsigned char bytecode[] = { 0x55, 0x89, 0xE5, 0x8B, 0x45, 0x08, 0x0F, 0xAF, 0xC0, 0x5D, 0xC3 };

    for (int i = 0; i < sizeof(bytecode); i++) {
        itext[i] = bytecode[i];
    }

    // Set parameters
    xed_tables_init();
    mmode = XED_MACHINE_MODE_LEGACY_32;
    stack_addr_width = XED_ADDRESS_WIDTH_32b;
    int offset = 0;
    int length = 1;
    xed_uint_t line_size;

    // Main loop
    printf("%-8s%-23s%s\n", "Offset", "Bytecode", "Disassembly");
    while (offset != sizeof(bytecode)) {
        // Decode instruction
        if (decode(&xedd, itext, offset, sizeof(bytecode), mmode, stack_addr_width)) {
            return 1;
        }

        // Profile instruction
        if (profiler_process_instruction(result, &xedd)) {
            return 1;
        }

        // Get length of instruction
        length = xed_decoded_inst_get_length(&xedd);

        // Print disassembly
        if (print(&xedd, buffer, offset, length, itext)) {
            return 1;
        }

        // Increment offset
        offset += length;
    }
    printf("\n");

    // Print profiling results
    profiler_print_result(result);

    // Free malloc'd memory
    profiler_free_profile(result);

    return 0;
    (void) argv; (void)argc;
}
