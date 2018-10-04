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

class Instruction
{
    public:
    Instruction();
    ~Instruction();
    unsigned int opcode_;
    std::string command_;
};

class Syscall : public Instruction
{
    public:
    Syscall() { command_ = "syscall"; };
    ~Syscall();
    void printS();
};

class R_Format : public Instruction
{
    public:
    R_Format();
    unsigned int opcode_ = 0;
    unsigned int rs_;
    unsigned int rt_;
    unsigned int rd_;
    unsigned int shamt_;
    unsigned int funct_;
    std::string command_;
  void printR();
  //void setR();
};

struct I_Format
{
  unsigned int opcode_;
  unsigned int rs_;
  unsigned int rt_;
  unsigned int immed_;
  std::string command_;
  void printI();
};

struct J_Format
{
  unsigned int opcode_;
  unsigned int addr_;
  std::string command_;
  void printJ();
};



std::map<unsigned int, std::string> reg
{
  {0, "$zero"},
  {1, "$at"},
  {2, "$v0"},	{3, "$v1"},
  {4, "$a0"}, {5, "$a1"}, {6, "$a2"}, {7, "$a3"},
  {8, "$t0"}, {9, "$t1"}, {10, "$t2"}, {11, "$t3"}, {12, "$t4"}, {13, "$t5"},
  {14, "$t6"}, {15, "$t7"},
  {16, "$s0"}, {17, "$s1"}, {18, "$s2"}, {19, "$s3"}, {20, "$s4"},
  {21, "$s5"}, {22, "$s6"},  {23, "$s7"},
  {24, "$t8"}, {25, "$t9"},
  {26, "$k0"},
  {27, "$k1"},
  {28, "$gp"},
  {29, "$sp"},
  {30, "$fp"},
  {31, "$ra"}
};

std::map<unsigned int, std::string> funct
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

int main()
{
  while(fgets(line, MAXLINE, stdin))
  {
    sscanf(line, "%x", &inst);
       
       Input input;
    
    // Test instruction type and create object
    if (inst >> 26 == 0)
	  {
      if (inst == 0x00000000c)
	    {
            input.type = InstructionType::S;
            t.s_ = { funct[inst] };
            t.s_.printS();
      }

      else
	    {
        input.type = InstructionType::R;
        input.r_ = 
        {
          inst >> 26,
		      inst << 11 >> 27,
		      inst << 6 >> 27,
		      inst << 16 >> 27,
		      inst << 21 >> 27,
		      inst << 26 >> 26,
		      funct[input.r_.funct_]
		    };
        input.r_.printR();
      }
    }
    
    else if (inst == 2 || inst == 3)
	  {
      input.type = InstructionType::J;
      input.j_ =
	    {
        inst >> 26,
	      inst << 7 >> 7,
	      funct[input.j_.opcode_]
      };
      input.j_.printJ();
	  }
	  
    else
    {
	     input.type = InstructionType::I;
	     input.i_ =
       {
         inst >> 26,
	       inst << 11 >> 27,
	       inst << 6 >> 27,
	       inst << 16 >> 16,
	       funct[input.i_.opcode_]
	     };
	     input.i_.printI();
	  }
    }
    return 0;
 }

void R_Format::printR()
{
  std::cout
    << command_ << "\n"
    /*<< std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode_ << "\n"
      << std::setw(8) << std::left << "rs"     << std::hex << std::setw(4) << std::right << rs_     << "\n"
      << std::setw(8) << std::left << "rt"     << std::hex << std::setw(4) << std::right << rt_     << "\n"
      << std::setw(8) << std::left << "rd"     << std::hex << std::setw(4) << std::right << rd_     << "\n"
      << std::setw(8) << std::left << "shamt"  << std::hex << std::setw(4) << std::right << shamt_  << "\n"
      << std::setw(8) << std::left << "funct"  << std::hex << std::setw(4) << std::right << funct_  << "\n"*/
    ;
}

void I_Format::printI()
{
  std::cout
    << command_ << "\n"
    /*<< std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode_ << "\n"
      << std::setw(8) << std::left << "rs"     << std::hex << std::setw(4) << std::right << rs_     << "\n"
      << std::setw(8) << std::left << "rt"     << std::hex << std::setw(4) << std::right << rt_     << "\n"
      << std::setw(8) << std::left << "immed"  << std::hex << std::setw(4) << std::right << immed_  << "\n"*/
    ;
}

void J_Format::printJ()
{
  std::cout
    << command_ << "\n"
    /* << std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode_ << "\n"
       << std::setw(8) << std::left << "addr"     << std::hex << std::setw(4) << std::right << addr_ << "\n"*/;
}

void syscall::printS()
{
  std::cout << std::setw(8) << command_ << std::endl;
}
