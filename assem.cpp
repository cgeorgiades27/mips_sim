/*************************************
 *
      Chris Georgiades
 *    CDA 3101
      asg2 - 09/17/18
 *    g++ -o assem.exe assem.cpp

 *************************************/

#include <iostream>
#include <queue>
#include <map>

#define MAXLINE 80
#define MAXREG 5

std::map<std::string, unsigned int> reg =
{
  {"zero", 0x0},
  {"at", 0x1},
  {"v0", 0x2},
  {"v1", 0x3},
  {"a0", 0x4},
  {"a1", 0x5},
  {"a2", 0x6},
  {"a3", 0x7},
  {"t0", 0x8},
  {"t1", 0x9},
  {"t2", 0xa},
  {"t3", 0xb},
  {"t4", 0xc},
  {"t5", 0xd},
  {"t6", 0xe},
  {"t7", 0xf},
  {"s0", 0x10},
  {"s1", 0x11},
  {"s2", 0x12},
  {"s3", 0x13},
  {"s4", 0x14},
  {"s5", 0x15},
  {"s6", 0x16},
  {"s7", 0x17},
  {"t8", 0x18},
  {"t9", 0x19},
  {"k0", 0x1a},
  {"k1", 0x1b},
  {"gp", 0x1c},
  {"sp", 0x1d},
  {"fp", 0x1e},
  {"ra", 0x1f}
};

std::map<std::string, unsigned int> funct =
{
  {"addiu", 0x9},
  {"addu", 0x21},
  {"and", 0x24},
  {"beq", 0x4},
  {"bne", 0x5},
  {"div", 0x1a},
  {"j", 0x2},
  {"lw", 0x23},
  {"mfhi", 0x10},
  {"mflo", 0x12},
  {"mult", 0x18},
  {"or", 0x25},
  {"slt", 0x2a},
  {"subu", 0x23},
  {"sw", 0x2b},
  {"syscall", 0xc}
};

enum class InstructionType
{
  R, I, J
};

struct R_Format
{
  unsigned int rd_:5;
  unsigned int rs_:5;
  unsigned int rt_:5;
  unsigned int funct_:6;
  unsigned int opcode_:6;
}; 

struct I_Format
{
  unsigned int rs_:5;
  unsigned int rt_:5;
  unsigned int opcode_:6;
  unsigned int immed_:16;
};

struct J_Format
{
  unsigned int opcode_:6;
  unsigned int targaddr_:26;
};

struct Input
{
  InstructionType instruction_type;

  union
  {
    R_Format r_;
    I_Format i_;
    J_Format j_;
  };
};
 

int main()
{
  
  int counter = 0;
  std::string opcode, rd, rs, rt, funct;
  unsigned int immed;
  std::string line;
  std::queue<Input> q;
  
  while (fgets(line, MAXLINE, stdin))
    {
      // R Format Parse
      if (sscanf(line, "%s $%[^,], $%[^,], $%s", opcode, rd, rs, rt) == 4)
	{
	  printf("opcode: %s\n", opcode);
	  
	  Input input;
	  input.instruction_type = InstructionType::R;
	  input.r_ = 
	    {
	      reg.find(rd)->second,
	      reg.find(rs)->second,
	      reg.find(rt)->second,
	      funct.find(opcode)->second,
	      0x0
	    };
	  q.push(input);
	}
      // I Format
      else if (sscanf(line, "%s $%[^,], %u ($%[^)]", opcode, rt, &immed, rs) == 4)
	{
	  Input input;
	  input.instruction_type = InstructionType::I;
	  input.i_ = 
	    {
	      reg.find(rs)->second,
	      reg.find(rt)->second,
	      funct.find(opcode)->second,
	      immed
	    };
	  q.push(input);
	}
      ++counter;	 
    }
  return 0;
}
