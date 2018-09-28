#include <iostream>
#include <bitset>
#include <iomanip>
#include <stdio.h>

#define MAXLINE 80

std::bitset<6> funct, opcode;
std::bitset<5> shamt, rd, rs, rt;

unsigned short funct_, shamt_, rd_, rs_, rt_, opcode_;
char line[MAXLINE];
unsigned int instruction;

int main()
{
  
  fgets(line, MAXLINE, stdin);
  sscanf(line, "%x", &instruction);
  printf("instruction parsed: %x\n", instruction);
  
  std::bitset<32> reg(instruction);
  std::cout << "reg contents: " << reg << std::endl;    

  for (size_t i = 0; i < reg.size(); ++i)
    std::cout << std::setw(5) << "reg[" << i << "] = " << reg[i] << "\n";


  for (size_t i = 0; i < reg.size(); ++i)
    {
      if (i < 6 && reg.test(i))
	funct.set();
      else if (i > 5 && i < 11)
	shamt[i] = reg[i];
      else if (i > 10 && i < 16)
	rd[reg.size() - i] = reg[i];
      else if (i > 15 && i < 21)
	rt[i] = reg[i];
      else if (i > 20 && i < 26)
	rs[reg.size() - i] - reg[i];
      else if (i > 26 && i < 31)
	opcode[reg.size() - i] = reg[i];
    }
  
  std::cout << std::setw(8) << std::left << "opcode" << std::setw(10) << std::right << opcode << "\n"
	    << std::setw(8) << std::left << "rs"     << std::setw(10) << std::right << rs << "\n"
	    << std::setw(8) << std::left << "rt"     << std::setw(10) << std::right << rt << "\n"
	    << std::setw(8) << std::left << "rd"     << std::setw(10) << std::right << rd << "\n"
	    << std::setw(8) << std::left << "shamt"  << std::setw(10) << std::right << shamt << "\n"
	    << std::setw(8) << std::left << "funct"  << std::setw(10) << std::right << funct << "\n"
	    << "reg[15]: " << reg[15] << std::endl;
  
  return 0;
}
