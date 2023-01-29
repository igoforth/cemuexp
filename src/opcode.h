// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Opcodes and utility functions
// -----------------------------------------
/// @file opcode.h

#ifndef OPCODE_H
#define OPCODE_H

enum ScaleFactor {
  times_1 = 0,
  times_2 = 1,
  times_4 = 2,
  times_8 = 3,
  times_int_size = times_4,

  times_half_system_pointer_size = times_2,
  times_system_pointer_size = times_4,

  times_tagged_size = times_4,
};

#endif