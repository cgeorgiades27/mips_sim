/******************************
 *
 *  Chris Georgiades
 *  CDA3101 - asg 3
 *
 ******************************/

#include <iostream>
#include <queue>
#include <vector>
#include <map>

#define MAXLINE 80
char line[MAXLINE];
unsigned int funct_, shamt_, rd_, rs_, rt_, opcode_, addr_, immed_;
unsigned int instruction;

enum class InstructionType
{
  R, I, J
};

struct R_Format
{
  std::string command;
  unsigned int opcode_;
  unsigned int rd_;
  unsigned int rs_;
  unsigned int rt_;
  unsigned int funct_;
  unsigned int shamt_;
};

struct I_Format
{
  unsigned int rs_;
  unsigned int rt_;
  unsigned int opcode_;
  unsigned int immed_;
};

struct J_Format
{
  unsigned int opcode_;
  unsigned int targaddr_;
};

struct Input
{
  InstructionType instruction_type;
  union
  {
    R_Format r_;
    I_Format i_;
    J_Format j_;
  }
};

std::map<unsigned int, std::string, unsigned int> reg =
{
  {0, "$zero", 0},
  {1, "$at", 0},
  {2, "$v0", 0},
  {3, "$v1", 0},
  {4, "$a0", 0},
  {5, "$a1", 0},
  {6, "$a2", 0},
  {7, "$a3", 0},
  {8, "$t0", 0},
  {9, "$t1", 0},
  {10, "$t2", 0},
  {11, "$t3", 0},
  {12, "$t4", 0},
  {13, "$t5", 0},
  {14, "$t6", 0},
  {15, "$t7", 0},
  {16, "$s0", 0},
  {17, "$s1", 0},
  {18, "$s2", 0},
  {19, "$s3", 0},
  {20, "$s4", 0},
  {21, "$s5", 0},
  {22, "$s6", 0},
  {23, "$s7", 0},
  {24, "$t8", 0},
  {25, "$t9", 0},
  {26, "$k0", 0},
  {27, "$k1", 0},
  {28, "$gp", 0},
  {29, "$sp", 0},
  {30, "$fp", 0},
  {31, "$ra", 0}
};

std::map<unsigned int, std::string> funct =
{
  {9, "addiu"},
  {33, "addu"},
  {4, "beq"},
  {2, "j"},
  {35, "lw"},
  {42, "slt"},
  {43, "sw"},
  {12, "syscall"}
};

void addu(unsigned int rd, unsigned int rs, unsigned int rt);
void addiu(unsigned int rt, unsigned int rs, unsigned int immed);
void lw(unsigned int rt, int offset, unsigned int a[]);
void sw(unsigned int rt, int offset, unsigned int a[]);
void beq(unsigned int rt, unsigned int rs, std::string label);

void decode(unsigned int);

unsigned int rs_;
unsigned int rt_;
unsigned int opcode_;
unsigned int immed_;
unsigned int opcode_;
unsigned int targaddr_;
unsigned int funct_;

int main()
{
  while(fgets(line, MAXLINE, stdin))
    {
      sscanf(line, "%x", &instruction);
      printf("instruction parsed: %x\n", instruction);

      Input input; // create an input object

      if (instruction >> 26 == 0)
      {
        input.instruction_type = InstructionType::R;
        input.r_ =
          {
            instruction >> 26;
            rd_ = instruction << 16 >> 27;
            rs_ = instruction << 11 >> 27;
            rt_ = instruction << 6 >> 27;
            funct_ = instruction << 26 >> 26;
            shamt_ = instruction << 21 >> 27;
          }
      }
  return 0;
} // end main


void addu(unsigned int rd, unsigned int rs, unsigned int rt)
{
  rd = rs + rt;
}

void addiu(unsigned int rt, unsigned int rs, unsigned int immed)
{
  rs = rt + immed;
}

void lw(unsigned int rt, int offset, unsigned int a[])
{
  rt = a[offset];
}

void sw(unsigned int rt, int offset, unsigned int a[])
{
  a[offset] = rt;
}

void beq(unsigned int rt, unsigned int rs, std::string label)
{
  if (rt == rs)
    goto label;
}
//j
//void slt(
//syscall
