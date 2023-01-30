// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Registers and utility functions
// -----------------------------------------
/// @file register.h

#ifndef REGISTER_H
#define REGISTER_H

#include "cpu.h"

typedef struct dispatch_register_s {
    xed_reg_enum_t reg;
    register_t register_data;
} dispatch_register_t;

typedef union register_u {
    uint8_t  byte;
    uint16_t word;
    uint32_t dword;
    uint64_t qword;
} register_t;

enum Condition {
  // any value < 0 is considered no_condition
  no_condition = -1,

  overflow = 0,
  no_overflow = 1,
  below = 2,
  above_equal = 3,
  equal = 4,
  not_equal = 5,
  below_equal = 6,
  above = 7,
  negative = 8,
  positive = 9,
  parity_even = 10,
  parity_odd = 11,
  less = 12,
  greater_equal = 13,
  less_equal = 14,
  greater = 15,

  // aliases
  carry = below,
  not_carry = above_equal,
  zero = equal,
  not_zero = not_equal,
  sign = negative,
  not_sign = positive
}

uint16_t* register_build_lookup_table(profiler_result_t* profiler_result);

#endif