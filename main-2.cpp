#include <iostream>
#include <bitset>
#include <iomanip>
#include <stdio.h>

#define MAXLINE 80


void Decode_R(std::bitset<32> reg, unsigned long funct_, shamt_, rd_, rs_, rt_, opcode_)
{
  //counters
  size_t j = 0;
  size_t k = 0;
  size_t l = 0;
  size_t m = 0;
  size_t n = 0;

  unsigned int instruction;
  std::bitset<6> funct, opcode;
  std::bitset<5> shamt, rd, rs, rt;

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
  // unsigned long conversions
  funct_ = funct.to_ulong();
  shamt_ = shamt.to_ulong();
  rd_ = rd.to_ulong();
  rs_ = rs.to_ulong();
  rt_ = rt.to_ulong();
  opcode_ = opcode.to_ulong();  
};

