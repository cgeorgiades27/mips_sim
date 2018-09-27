#include <iostream>
#include <bitset>
#include <iomanip>
#include <stdio.h>

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
  unsigned long instruction;
  char line[80] = {0};
  
  while(fgets(line, 80, stdin))
    {
      std::cout << "Enter hex instruction: ";
      std::cin >> std::hex >> instruction;
      std::bitset<32> reg (instruction);
      std::cout << reg << "\n";
      std::cout << "instruction: " << instruction << std::endl;
      
      for (size_t i = 0; i < reg.size(); ++i)
	{
	  if (i <= 5)
	    funct[i] = reg[i];
	  else if (i > 5 && i <= 10)
	    shamt[i] = reg[i];
	  else if (i > 10 && i <= 15)
	    rd[i] = reg[i];
	  else if (i > 15 && i <= 20)
	    rt[i] = reg[i];
	  else if (i > 20 && i <= 25)
	    rs[i] = reg[i];
	  else
	    opcode[i] = reg[i];
	}
      std::cout << std::setw(8) << std::left << "opcode: " << std::setw(10) << std::right << opcode.to_ulong() << "\n"
		<< std::setw(8) << std::left << "rs: "     << std::setw(10) << std::right << rs.to_ulong()      << "\n"
		<< std::setw(8) << std::left << "rt: "     << std::setw(10) << std::right << rt.to_ulong()     << "\n"
		<< std::setw(8) << std::left << "rd: "     << std::setw(10) << std::right << rd.to_ulong()    << "\n"
		<< std::setw(8) << std::left << "shamt: "  << std::setw(10) << std::right << shamt.to_ulong()  << "\n"
		<< std::setw(8) << std::left << "funct: "  << std::setw(10) << std::right << std::hex << funct.to_ulong()  << std::endl;
    }    
  return 0;
}
