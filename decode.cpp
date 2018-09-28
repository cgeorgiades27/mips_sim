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
  unsigned long instruction;
  
  while(fgets(line, MAXLINE, stdin))
    {
      if (sscanf(line, "%x", &instruction) == 1)
	printf("line successfully read: %x\n", instruction);
      else
	printf("parse failed...");

      std::bitset<32> reg (instruction);
      std::cout << reg << std::endl;

      for (size_t i = 32; i > 0; --i)
	std::cout << i % 10;

      std::cout << std::endl;

      for (size_t i = reg.size(); i >= 0 ; --i)
	{
	  if (i <= 5)
	    funct[i] = reg[i];
	  else if (i > 5 && i <= 10)
	    {
	      shamt[i] = reg[i];
	      std::cout << "rd[" << i << "] is: " << rd[i] << "\n";
	    }
	  else if (i > 10 && i <= 15)
	    rd[i] = reg[i];
	  else if (i > 15 && i <= 20)
	    rt[i] = reg[i];
	  else if (i > 20 && i <= 25)
	    rs[i] = reg[i];
	  else
	    opcode[i] = reg[i];
	    }
  
      std::cout << std::setw(8) << std::left << "opcode: " << std::setw(10) << std::right << opcode << "\n"
		<< std::setw(8) << std::left << "rs: "     << std::setw(10) << std::right << rs << "\n"
		<< std::setw(8) << std::left << "rt: "     << std::setw(10) << std::right << rt << "\n"
		<< std::setw(8) << std::left << "rd: "     << std::setw(10) << std::right << rd << "\n"
		<< std::setw(8) << std::left << "shamt: "  << std::setw(10) << std::right << shamt << "\n"
		<< std::setw(8) << std::left << "funct: "  << std::setw(10) << std::right << funct << std::endl;
          
}    
  return 0;
}
