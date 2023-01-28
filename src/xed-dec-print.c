/* BEGIN_LEGAL 

Copyright (c) 2022 Intel Corporation

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
  
END_LEGAL */
/// @file xed-dec-print.c
// decode and print

#include "xed/xed-interface.h"
#include "xed-examples-util.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define TRUE  1
#define FALSE 0

typedef int8_t bool;

int main(int argc, char** argv);

int 
main(int argc, char** argv)
{
    xed_error_enum_t xed_error;
    xed_decoded_inst_t xedd;
#define BUFLEN  1000
    char buffer[BUFLEN];
    xed_bool_t ok;
    xed_machine_mode_enum_t mmode;
    xed_address_width_enum_t stack_addr_width;

    char *memory = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        perror("failed to allocate memory");
        exit(1);
    }

    // Assembly
    // square:
    //     push    ebp
    //     mov     ebp, esp
    //     mov     DWORD PTR [rbp-4], edi
    //     mov     eax, DWORD PTR [rbp-4]
    //     imul    eax, eax
    //     pop     ebp
    //     ret

    // Disassembly
    // 0:  55                      push   ebp
    // 1:  89 e5                   mov    ebp,esp
    // 3:  89 3d fc ff ff ff       mov    DWORD PTR ds:0xfffffffc,edi
    // 9:  a1 fc ff ff ff          mov    eax,ds:0xfffffffc
    // e:  0f af c0                imul   eax,eax
    // 11: 5d                      pop    ebp
    // 12: c3                      ret

    // Bytecode
    // 5589E5893DFCFFFFFFA1FCFFFFFF0FAFC05DC3
    // Write bytecode to memory
    unsigned char bytecode[] = {
        0x55,
        0x89, 0xe5,
        0x89, 0x3d, 0xfc, 0xff, 0xff, 0xff,
        0xa1, 0xfc, 0xff, 0xff, 0xff,
        0x0f, 0xaf, 0xc0,
        0x5d,
        0xc3
    };

    for (int i = 0; i < sizeof(bytecode); i++) {
        memory[i] = bytecode[i];
    }

    xed_tables_init();
    mmode=XED_MACHINE_MODE_LEGACY_32;
    stack_addr_width =XED_ADDRESS_WIDTH_32b;

    // Decode and print
    bool end = FALSE;
    int count = 0;
    while(!end) {
        memory += count;
        xed_decoded_inst_zero(&xedd);
        xed_decoded_inst_set_mode(&xedd, mmode, stack_addr_width);
        xed_error = xed_decode(&xedd, (xed_uint8_t*)memory, 15);
        if (xed_error == XED_ERROR_NONE) {
            ok = xed_format_context(XED_SYNTAX_INTEL, &xedd, buffer, BUFLEN, 0, 0, 0);
            if (ok) {
                printf("%s\n", buffer);
            }
            else {
                printf("Error disassembling\n");
                return 1;
            }
        }
        else {
            printf("Decoding error\n");
            return 1;
        }
        count += xed_decoded_inst_get_length(&xedd);
        if (count > 0x1000) {
            end = TRUE;
        }
    }

    return 0;
    (void) argv; (void)argc;
}
