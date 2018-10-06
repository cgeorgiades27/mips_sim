/******************************
 *
 *  Chris Georgiades
 *  CDA3101 - asg 3
 *  main.cpp
 *
 ******************************/

#include <iostream>
#include <map>
#include <iomanip>
#include <queue>

#define MAXLINE 80

char line[MAXLINE];
unsigned int inst;
unsigned int regs[32];
unsigned int word;

int pc = 0;

//void printRegs(Input);

enum class InstructionType
{
  R,
  I,
  J,
  S
};

struct R_Format
{
  unsigned int opcode_;
  unsigned int rs_;
  unsigned int rt_;
  unsigned int rd_;
  unsigned int shamt_;
  unsigned int funct_;
  void printR();
  void setR();
};

struct I_Format
{
  unsigned int opcode_;
  unsigned int rs_;
  unsigned int rt_;
  unsigned int immed_;
  void printI();
};

struct J_Format
{
  unsigned int opcode_;
  unsigned int addr_;
  void printJ();
};

struct System_Call
{
  unsigned int opcode_;
  unsigned int funct_;
  void printS();
};

struct Input
{
  InstructionType type;
  union {
    R_Format rData;
    I_Format iData;
    J_Format jData;
    System_Call sData;
  };
};

std::map<unsigned int, std::string> reg{
    {0, "$zero"},
    {1, "$at"},
    {2, "$v0"},
    {3, "$v1"},
    {4, "$a0"},
    {5, "$a1"},
    {6, "$a2"},
    {7, "$a3"},
    {8, "$t0"},
    {9, "$t1"},
    {10, "$t2"},
    {11, "$t3"},
    {12, "$t4"},
    {13, "$t5"},
    {14, "$t6"},
    {15, "$t7"},
    {16, "$s0"},
    {17, "$s1"},
    {18, "$s2"},
    {19, "$s3"},
    {20, "$s4"},
    {21, "$s5"},
    {22, "$s6"},
    {23, "$s7"},
    {24, "$t8"},
    {25, "$t9"},
    {26, "$k0"},
    {27, "$k1"},
    {28, "$gp"},
    {29, "$sp"},
    {30, "$fp"},
    {31, "$ra"}};

std::map<unsigned int, std::string> funct{
    {9, "addiu"},
    {33, "addu"},
    {4, "beq"},
    {2, "j"},
    {35, "lw"},
    {42, "slt"},
    {43, "sw"},
    {12, "syscall"}};

std::queue<Input> q;

int main()
{
  Input input; // Create input object

  while (fgets(line, MAXLINE, stdin))
  {
    /*
    if (sscanf(line, "%u, %u", &regs[28], &word) == 2)
    {
      cout << word;
    }
    else
    {
      */

    sscanf(line, "%x", &inst);

    if (inst >> 26 == 0)
    {
      if (inst == 0x00000000c)
      {
        input.type = InstructionType::S;
        input.sData = {inst >> 26};
        input.sData.printS();
        q.push(input);
      }

      else
      {
        input.type = InstructionType::R;
        input.rData =
            {
                inst >> 26,
                inst << 11 >> 27,
                inst << 6 >> 27,
                inst << 16 >> 27,
                inst << 21 >> 27,
                inst << 26 >> 26};
        input.rData.printR();
      }
    }

    else if (inst == 2 || inst == 3)
    {
      input.type = InstructionType::J;
      input.jData =
          {
              inst >> 26,
              inst << 7 >> 7};
      input.jData.printJ();
    }

    else
    {
      input.type = InstructionType::I;
      input.iData =
          {
              inst >> 26,
              inst << 11 >> 27,
              inst << 6 >> 27,
              inst << 16 >> 16};
      input.iData.printI();
    }
    // store object data on queue
    //printRegs(input);
    ++pc; // increment program counter
  }
  while (!q.empty())
  {
    size_t i = 0;
    
    std::cout << i << ": ";

    if (input.type == InstructionType::R)
      q.front().rData.printR();
    else if (input.type == InstructionType::S)
      q.front().sData.printS();
    else if (input.type == InstructionType::I)
      q.front().iData.printI();
    else
      q.front().jData.printJ();
    q.pop();
  }
  return 0;
}

void R_Format::printR()
{
  std::cout
      << std::setw(10) << std::left << funct[funct_] << reg[rd_] + "," + reg[rt_] + "," + reg[rs_] << std::endl;
  /*
      
      FOR TESTING ONLY....

      << std::setw(10) << "opcode" << opcode_ << "\n"
      << std::setw(10) << "rs" << rs_ << "\n"
      << std::setw(10) << "rt" << rt_ << "\n"
      << std::setw(10) << "rd" << rd_ << "\n"
      << std::setw(10) << "shamt" << shamt_ << "\n"
      << std::setw(10) << "funct" << std::endl*/
}

void I_Format::printI()
{
  std::cout
      << std::setw(10) << std::left << funct[opcode_] << reg[rt_] << "," << reg[rs_] << "," << immed_ << std::endl;
  /*

    FOR TESTING ONLY...

      << std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode_ << "\n"
      << std::setw(8) << std::left << "rs" << std::hex << std::setw(4) << std::right << rs_ << "\n"
      << std::setw(8) << std::left << "rt" << std::hex << std::setw(4) << std::right << rt_ << "\n"
      << std::setw(8) << std::left << "immed" << std::hex << std::setw(4) << std::right << immed_ << "\n"*/
}

void J_Format::printJ()
{
  std::cout << std::setw(10) << funct[opcode_] << addr_;
  /*
      << std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode_ << "\n"
      << std::setw(8) << std::left << "addr" << std::hex << std::setw(4) << std::right << addr_ << "\n";*/
}

void System_Call::printS()
{
  std::cout << "syscall" << std::endl;
  /*
  std::cout << std::setw(8) << "opcode" << opcode_ << std::endl
            << std::setw(8) << "funct" << funct_ << std::endl;*/
}

void printRegs(Input i)
{

  std::cout << std::setw(4) << std::left << "PC:" << pc << std::endl; //  PC: xx
  /*
  if (i.type == InstructionType::R)
  {
    std::cout << setw(10) << left << funct[funct_] << reg[rd_] + "," + reg[rs_] + "," + reg[rt_] << "\n" << endl;
  }


inst: addiu     $v0,$zero,5

regs:
   $zero =     0     $at =     0     $v0 =     5     $v1 =     0
     $a0 =     0     $a1 =     0     $a2 =     0     $a3 =     0
     $t0 =     0     $t1 =     0     $t2 =     0     $t3 =     0
     $t4 =     0     $t5 =     0     $t6 =     0     $t7 =     0
     $s0 =     0     $s1 =     0     $s2 =     0     $s3 =     0
     $s4 =     0     $s5 =     0     $s6 =     0     $s7 =     0
     $t8 =     0     $t9 =     0     $k0 =     0     $k1 =     0
     $gp =    18     $sp =     0     $fp =     0     $ra =     0
     $lo =     0     $hi =     0

data memory:
   data[  0] =     0
*/
}
