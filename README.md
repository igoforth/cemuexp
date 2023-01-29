# C Emulation Experiments

Experiments with ia32 CPU Emulation in C. A frankenstein of a repository attempting to make sense of code across the web. No sense trying to reinvent the wheel for some aspects, such as decoding bytecode.

# Inspiration
http://www.gtoal.com/sbt/  
https://github.com/spencertipping/jit-tutorial  
https://github.com/v8/v8/blob/master/src/codegen/x64/assembler-x64.h  
https://opensource.apple.com/source/clang/clang-421.11.65/src/lib/Target/X86/Disassembler/  

# Build
```bash
./build.sh
OR
gcc src/main.c -lxed -Llib
```

# Clean
```bash
./clean.sh
```