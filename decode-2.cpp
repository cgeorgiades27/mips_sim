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
  
  while(fgets(line, MAXLINE, stdin))
    {
      sscanf(line, "%x", &instruction);
      printf("instruction parsed: %x\n", instruction);
      std::bitset<32> reg(instruction);
      std::cout << reg << std::endl;    

      for (size_t i = 32; i > 0; --i)
	std::cout << i % 10;

      std::cout << std::endl;
      
      size_t j = 0;
      size_t k = 0;
      size_t l = 0;
      size_t m = 0;
      size_t n = 0;
      
      for (size_t i = 0; i < reg.size(); ++i)
	{
	  if (i < 6 && reg.test(i))
	    funct.set(i);
	  else if (i > 5 && i < 11 && reg.test(i))
	    {
	      shamt.set(j);
	      ++j;
	    }
	  else if (i > 10 && i < 16 && reg.test(i))
	    {
	      rd.set(k);
	      ++k;
	    }
	  else if (i > 15 && i < 21 && reg.test(i))
	    {
	      rt.set(l);
	      ++l;
	    }
	  else if (i > 20 && i < 26 && reg.test(i))
	    {
	      rs.set(m);
	      ++m;
	    }
	  else if (i > 26 && i < 31 && reg.test(i))
	    {
	      opcode.set(n);
	      ++n;
	    }
	}
      
      funct_ = funct.to_ulong();
      shamt_ = shamt.to_ulong();
      rd_ = rd.to_ulong();
      rs_ = rs.to_ulong();
      rt_ = rt.to_ulong();
      opcode_ = opcode.to_ulong();  
      
      std::cout << std::setw(8) << std::left << "opcode" << std::setw(10) << std::right << std::hex << opcode_ << "\n"
		<< std::setw(8) << std::left << "rs"     << std::setw(10) << std::right << std::hex << rs_ << "\n"
		<< std::setw(8) << std::left << "rt"     << std::setw(10) << std::right << std::hex << rt_ << "\n"
		<< std::setw(8) << std::left << "rd"     << std::setw(10) << std::right << std::hex <<  rd_ << "\n"
		<< std::setw(8) << std::left << "shamt"  << std::setw(10) << std::right << std::hex << shamt_ << "\n"
		<< std::setw(8) << std::left << "funct"  << std::setw(10) << std::right <<std::hex << funct_ << "\n";
    }

  return 0;
}
