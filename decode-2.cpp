#include <iostream>
#include <bitset>
#include <iomanip>
#include <stdio.h>

#define MAXLINE 80

std::bitset<6> funct;
std::bitset<5> shamt;
std::bitset<5> rd;
std::bitset<5> rt;
std::bitset<5> rs;
std::bitset<6> opcode;

unsigned short funct_;
unsigned short shamt_;
unsigned short rd_;
unsigned short rt_;
unsigned short rs_;
unsigned short opcode_;

int main()
{
  char line[MAXLINE];
  unsigned int instruction;
  
  fgets(line, MAXLINE, stdin);
  sscanf(line, "%x", &instruction);
  
  std::bitset<32> reg(instruction);
  std::cout << "reg contents: " << reg << std::endl;    
  std::cout << "to_string: " << reg.to_string() << std::endl;
  std::string reg_string = reg.to_string();
  std::bitset<6> functb(reg_string.back() - reg_string[5]);
  std::cout << "funct: " << functb << std::endl;
  
  std::cout << std::setw(8) << std::left << "opcode" << std::setw(10) << std::right << opcode << "\n"
	    << std::setw(8) << std::left << "rs"     << std::setw(10) << std::right << rs << "\n"
	    << std::setw(8) << std::left << "rt"     << std::setw(10) << std::right << rt << "\n"
	    << std::setw(8) << std::left << "rd"     << std::setw(10) << std::right << rd << "\n"
	    << std::setw(8) << std::left << "shamt"  << std::setw(10) << std::right << shamt << "\n"
	    << std::setw(8) << std::left << "funct"  << std::setw(10) << std::right << funct << std::endl;
  
return 0;
}
