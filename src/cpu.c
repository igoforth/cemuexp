#include <stdio.h>
#include <stdlib.h>

for (;;) {
  if (cycles >= eventTrigger) do_some_timed_event();
  opcode = mem[Reg_PC++];
  cycles = cycles + 1;
  switch (opcode) {

  case TFRregreg:
    operand = mem[Reg_PC++];
    cycles = cycles + 1;
    firstreg = operand>>4;
    secondreg = operand&15;
    *Reg[firstreg] = *Reg[secondreg];
    break;

  case ADDimmediate:
    operand = mem[Reg_PC++];
    cycles = cycles + 1;
    temp = Reg_A + operand + Flag_Carry;
    Flag_Carry = temp >> 8;
    Flag_Overflow = (Reg_A ^ operand ^ (Flag_Carry<<7))>>7;
    Reg_A = temp & 0xff;
    Flag_Negative = Reg_A >> 7;
    Flag_Zero = (Reg_A == 0 ? 1 : 0);
    break;

  case MULdirect:
    ... similar code for all the available opcodes ...

  }
}

