// Boilerplate that maps a memory space, writes bytecode to it, calls the LLVM decoder, and prints the instruction that would be executed.

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "../lib/LLVMX86Disassembler/X86DisassemblerDecoder.h"

#define TRUE  1
#define FALSE 0

typedef int8_t bool;

int main() {
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

    // Setup instruction
    struct InternalInstruction *insn = malloc(sizeof(struct InternalInstruction));

    /*
    * byteReader_t - Type for the byte reader that the consumer must provide to
    *   the decoder.  Reads a single byte from the instruction's address space.
    * @param arg     - A baton that the consumer can associate with any internal
    *                  state that it needs.
    * @param byte    - A pointer to a single byte in memory that should be set to
    *                  contain the value at address.
    * @param address - The address in the instruction's address space that should
    *                  be read from.
    * @return        - -1 if the byte cannot be read for any reason; 0 otherwise.
    */
    // typedef int (*byteReader_t)(void* arg, uint8_t* byte, uint64_t address);

    // Setup reader
    byteReader_t reader;

    /*
    * dlog_t - Type for the logging function that the consumer can provide to
    *   get debugging output from the decoder.
    * @param arg     - A baton that the consumer can associate with any internal
    *                  state that it needs.
    * @param log     - A string that contains the message.  Will be reused after
    *                  the logger returns.
    */
    // typedef void (*dlog_t)(void* arg, const char *log);

    // Setup logger
    dlog_t logger;

    /* decodeInstruction - Decode one instruction and store the decoding results in
    *   a buffer provided by the consumer.
    * @param insn      - The buffer to store the instruction in.  Allocated by the
    *                    consumer.
    * @param reader    - The byteReader_t for the bytes to be read.
    * @param readerArg - An argument to pass to the reader for storing context
    *                    specific to the consumer.  May be NULL.
    * @param logger    - The dlog_t to be used in printing status messages from the
    *                    disassembler.  May be NULL.
    * @param loggerArg - An argument to pass to the logger for storing context
    *                    specific to the logger.  May be NULL.
    * @param startLoc  - The address (in the reader's address space) of the first
    *                    byte in the instruction.
    * @param mode      - The mode (16-bit, 32-bit, 64-bit) to decode in.
    * @return          - Nonzero if there was an error during decode, 0 otherwise.
    */
    // int decodeInstruction(struct InternalInstruction* insn,
    //                       byteReader_t reader,
    //                       void* readerArg,
    //                       dlog_t logger,
    //                       void* loggerArg,
    //                       void* miiArg,
    //                       uint64_t startLoc,
    //                       DisassemblerMode mode);

    // Decode instruction
    return decodeInstruction(insn, reader, NULL, logger, NULL, NULL,(uint64_t) &memory[0], MODE_32BIT);
}