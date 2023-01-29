// -----------------------------------------
// A program that decodes and emulates x86 instructions
// Author: Ian Goforth
// 
// Registers and utility functions
// -----------------------------------------
/// @file register.h

#ifndef REGISTER_H
#define REGISTER_H

// 000 001 010 011 100 101 110 111
enum Register {
    eax = 0,
    ecx = 1,
    edx = 2,
    ebx = 3,
    esp = 4,
    ebp = 5,
    esi = 6,
    edi = 7
}

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

#endif