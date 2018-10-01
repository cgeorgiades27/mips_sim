#include <iostream>
#include <iomanip>
#include <stdio.h>

#define MAXLINE 80

char line[MAXLINE];
unsigned int funct_, shamt_, rd_, rs_, rt_, opcode_, addr_, immed_;
unsigned int instruction;

void printR(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int funct);
void printI(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int immed);
void printJ(unsigned int opcode, unsigned int addr);
void findR(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int funct);

int main()
{
  while(fgets(line, MAXLINE, stdin))
    {
      sscanf(line, "%x", &instruction);
      printf("instruction parsed: %x\n", instruction);

      // decode opcode first
      opcode_ = instruction >> 26;

      // determine instruction type...
      if (opcode_ == 0)
	{
	  funct_ = instruction << 26 >> 26;
	  shamt_ = instruction << 21 >> 27;
	  rd_ = instruction << 16 >> 27;
	  rs_ = instruction << 11 >> 27;
	  rt_ = instruction << 6 >> 27;
	  printR(opcode_, rt_, rt_, rd_, shamt_, funct_);
	}

      else if (opcode_ == 2 || opcode_ == 3)
	{
	  addr_ = instruction << 7 >> 7;
	  printJ(opcode_, addr_);
	}
      else
	{
	  rs_ = instruction << 11 >> 27;
	  rt_ = instruction << 6 >> 27;
	  immed_ = instruction << 16 >> 16;
	  printI(opcode_, rs_, rt_, immed_);
	}
    }
  return 0;
} // end main

void printR(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int funct)
{
  std::cout << std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode << "\n"
	    << std::setw(8) << std::left << "rs"     << std::hex << std::setw(4) << std::right << rs     << "\n"
	    << std::setw(8) << std::left << "rt"     << std::hex << std::setw(4) << std::right << rt     << "\n"
	    << std::setw(8) << std::left << "rd"     << std::hex << std::setw(4) << std::right << rd     << "\n"
	    << std::setw(8) << std::left << "shamt"  << std::hex << std::setw(4) << std::right << shamt  << "\n"
	    << std::setw(8) << std::left << "funct"  << std::hex << std::setw(4) << std::right << funct  << "\n";
}

void printI(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int immed)
{
  std::cout << std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode << "\n"
	    << std::setw(8) << std::left << "rs"     << std::hex << std::setw(4) << std::right << rs     << "\n"
	    << std::setw(8) << std::left << "rt"     << std::hex << std::setw(4) << std::right << rt     << "\n"
	    << std::setw(8) << std::left << "immed"  << std::hex << std::setw(4) << std::right << immed  << "\n";
}

void printJ(unsigned int opcode, unsigned int addr)
{
  std::cout << std::setw(8) << std::left << "opcode" << std::hex << std::setw(4) << std::right << opcode << "\n"
	    << std::setw(8) << std::left << "addr"     << std::hex << std::setw(4) << std::right << addr << "\n";
}
