/******************************
 *
 *  Chris Georgiades
 *  CDA3101 - asg 3
 *  main.cpp
 *  compile: g++ -o decode.exe decode.cpp
 *
 ******************************/

#include <iostream>
#include <map>
#include <iomanip>
#include <queue>
#include <fstream>
#define MAXLINE 80

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
  void add();
  void slt(unsigned int rd_, unsigned int rs_, unsigned int rt_);
};

struct I_Format
{
  unsigned int opcode_;
  unsigned int rs_;
  unsigned int rt_;
  unsigned int immed_;
  void printI();
  void addiu();
  void lw(unsigned int rt_, unsigned int rs__, unsigned int immed_, unsigned int arr[]);
  void sw(unsigned int rt_, unsigned int rs_, unsigned int immed_, unsigned int arr[]);
  void beq(unsigned int rt_, unsigned int rs_, unsigned int immed_);
};

struct J_Format
{
  unsigned int opcode_;
  unsigned int addr_;
  void printJ();
  void j(unsigned int immed_);
};

struct System_Call
{
  unsigned int opcode_;
  unsigned int funct_;
  void printS();
  void sCall(unsigned int v0);
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

void printAll(Input, int counter, unsigned int arr[]);          // print report
void printInstructions(Input, int counter); // print instruction list

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
								{31, "$ra"},
								  {32, "$lo"},
								    {33, "$hi"}
};

std::map<unsigned int, std::string> funct{
  {9, "addiu"},
    {33, "addu"},
      {4, "beq"},
	{2, "j"},
	  {35, "lw"},
	    {42, "slt"},
	      {43, "sw"},
		{12, "syscall"}
};

unsigned int inst;
unsigned int word;
unsigned int gp;

//std::queue<Input> q;
//unsigned int data[1];
std::vector<Input> q;
std::vector<unsigned int> regs(34, 0);
std::vector<unsigned int> iList;
Input input; // Create input object
char line[MAXLINE];

int main(int argc, char **argv)
{
  if (argc == 1) // test for argument usage
    {
      std::cerr << "usage: " << argv[0] << " <name>.obj\n";
      exit(EXIT_FAILURE);
    }
  
  std::ifstream fin;
  fin.open(argv[1]);
    
  if (!fin.is_open()) // test to see if file exists
    {
      std::cerr << "file " << argv[1] << " cannot be opened\n";
    }

  int counter1 = 0;

  while (fin.getline(line, MAXLINE))
    {
      if (sscanf(line, "%u%u", &gp, &word) == 2)
        {
	  regs[28] = gp; 
	}
      else
        {
	  sscanf(line, "%x", &inst);
	  iList.push_back(inst);
        }
    }
  ++counter1;
  
  unsigned int data[word];
  data[0] = iList[regs[28]];
  
  // parse
  for (int counter2 = 0; counter2 < iList.size() - 1; ++counter2)
    {
      inst = iList[counter2];
      
      if (inst >> 26 == 0)
        {
	  if (inst == 0xc)
            {
	      input.type = InstructionType::S;
	      input.sData = {
		inst >> 26,
		inst << 26 >> 26
	      };
	      q.push_back(input);
            }
	  else
            {
	      input.type = InstructionType::R;
	      input.rData =
		{
		  inst >> 26,
		  inst << 6 >> 27,
		  inst << 11 >> 27,
		  inst << 16 >> 27,
		  inst << 21 >> 27,
		  inst << 26 >> 26
		};
	      q.push_back(input);
            }
        }
      else if (inst >> 26 == 2 || inst >> 26 == 3)
        {
	  input.type = InstructionType::J;
	  input.jData =
	    {
	      inst >> 26,
	      inst << 7 >> 7
	    };
	  q.push_back(input);
        }
      else
        {
	  input.type = InstructionType::I;
	  input.iData =
	    {
	      inst >> 26,
	      inst << 6 >> 27,
	      inst << 11 >> 27,
	      inst << 16 >> 16
	    };
	  q.push_back(input);
        }
    } // end parse of object code
  
  int i = 0;
  
  std::cout << "isnts:" << std::endl;
  
  while (i < q.size())
    {
      printInstructions(q[i], i);
      ++i;
    }
  std::cout << "\n"
	    << "data:\n"
	    << std::setw(4) << std::right << i << ":" << std::setw(2) << *data << "\n"
	    << std::endl;
  
  for (int pc = 0, j = 0; pc < iList.size() - 1; ++pc, ++j)
    {
      inst = iList[pc];
      
      switch (inst >> 26) // begin function calls
        {
        case 0: // R_Format and Syscalls
	  {
	    unsigned int funct = inst << 26 >> 26;
	    
	    if (funct == 33)
	      q[pc].rData.add();
	    else if (funct == 42)
	      q[pc].rData.slt(q[pc].rData.rd_, q[pc].rData.rs_, q[pc].rData.rt_);
            else if (inst  == 0xc)
	      {
		if (regs[2] == 10)      // exit
		  exit(EXIT_SUCCESS);
		else if (regs[2] == 5)  // read input
		  std::cin >> regs[2];
		else                    // print
		  std::cout << regs[2] << "\n";
	      }
	    break;
	      }
	case 9:
	  {
	    q[pc].iData.addiu();
	    break;
	  }
	case 2:
	  {
	    pc = q[pc].jData.addr_;
	    break;
	  }
        case 4:
	  {
	    if (regs[reg.find(q[pc].iData.rt_)->first] == regs[reg.find(q[pc].iData.rs_)->first])
	      {
		pc = q[pc].iData.immed_;
	      }
            break;
	  }
        case 35:
	  {
	    q[pc].iData.lw(q[pc].iData.rt_, q[pc].iData.rs_, q[pc].iData.immed_, data);
	    break;
	  }
	case 43:
	  {
	    q[pc].iData.sw(q[pc].iData.rt_, q[pc].iData.rs_, q[pc].iData.immed_, data);
	    break;
	  }
        default:
	  break;
        }

      printAll(q[pc], j, data);
    }
  return 0;
} // THE END 

void J_Format::j(unsigned int immed_)
{
  
}


void System_Call::sCall(unsigned int v0)
{
  /*
  if (v0 == 10) // exit
    {
      std::cout << "calling syscall 10\n";
      exit(EXIT_SUCCESS);
    }
  else if (v0 == 5) // read input
    {
      std::cout << "calling syscall 5\n";
		   std::cin >> v0;
    }
  else // print
    {
      std::cout << "calling syscall 1\n";
      std::cout << v0 << "\n";
    }
  */
}

void I_Format::lw(unsigned int rt_, unsigned int rs_, unsigned int immed_, unsigned int arr[])
{
  
  regs[reg.find(rt_)->first] = arr[immed_ / 4];
  //regs[reg.find(rt_)->first] = *(gp + immed_);
}

void I_Format::sw(unsigned int rt_, unsigned int rs_, unsigned int immed_, unsigned int arr[])
{
  arr[immed_ / 4] = regs[reg.find(rt_)->first];
  //*(gp + 4 * immed_) = regs[reg.find(rt_)->first];
}

void I_Format::beq(unsigned int rt_, unsigned int rs_, unsigned int immed_)
{
  //  if (regs[reg.find(rt_)->first] == regs[reg.find(rs_)->first])
  //  q[pc] = q[immed_];
}


void R_Format::slt(unsigned int rd_, unsigned int rs_, unsigned int rt_)
{
  if (regs[reg.find(rs_)->first] < regs[reg.find(rt_)->first])
    regs[reg.find(rd_)->first] = 1;
  else
    regs[reg.find(rd_)->first] = 0;
}

void R_Format::add()
{
  regs[reg.find(rd_)->first] = regs[reg.find(rs_)->first] + regs[reg.find(rt_)->first];
}

void I_Format::addiu()
{
  regs[reg.find(rt_)->first] = regs[reg.find(rs_)->first] + immed_;
}

void R_Format::printR()
{
  std::cout << std::setw(10) << std::left << funct[funct_] << reg[rd_] + "," + reg[rs_] + "," + reg[rt_] << std::endl;
}

void I_Format::printI()
{
  switch (opcode_)
    {
    case 35:
    case 43:
      {
        std::cout
	  << std::setw(10)
	  << std::left
	  << funct[opcode_]
	  << reg[rt_] << ","
	  << immed_
	  << "(" << reg[rs_] << ")" << std::endl;
        break;
      }
    case 4:
      {
        std::cout
	  << std::setw(10)
	  << std::left
	  << funct[opcode_]
	  << reg[rs_] << ","
	  << reg[rt_] << ","
	  << immed_ << std::endl;
        break;
      }
    default:
      {
        std::cout
	  << std::setw(10)
	  << std::left
	  << funct[opcode_]
	  << reg[rt_] << ","
	  << reg[rs_] << ","
	  << immed_ << std::endl;
        break;
      }
    }
}

void J_Format::printJ()
{
  std::cout << std::setw(10) << std::left << funct[opcode_] << addr_ << std::endl;
}

void System_Call::printS()
{
  std::cout << "syscall" << std::endl;
}

void printAll(Input input, int counter, unsigned int arr[])
{
  std::cout
    << "PC:" << std::right << std::setw(3) << counter << std::endl
    << "inst: ";

  switch (input.type)
    {
    case InstructionType::R:
      {
	input.rData.printR();
	break;
      }
    case InstructionType::S:
      {
        input.sData.printS();
        break;
      }
    case InstructionType::I:
      {
        input.iData.printI();
        break;
      }
    default:
      {
        input.jData.printJ();
        break;
      }
    } // end of switch

  std::cout << std::endl
	    << "regs:" << std::endl;

  for (int i = 0, j = 1; i < 34; ++i, ++j)
    {
      std::cout
	<< std::setw(8) << std::right << reg.find(i)->second
	<< " ="
	<< std::setw(6) << std::right << regs[i];
      if (j % 4 == 0)
	std::cout << std::endl;
    }
  std::cout << "\n\n"
	    << "data memory:\n"
	    << "   data[ 0]: " << std::setw(6) << std::right << arr[0] 
	    << std::endl;

  printf("\n\n");
}

void printInstructions(Input input, int counter)
{
  std::cout
    << std::setw(2) << std::right << counter << std::setw(2) << std::left << ":";

  switch (input.type)
    {
    case InstructionType::R:
      {
        input.rData.printR();
        break;
      }
    case InstructionType::S:
      {
        input.sData.printS();
        break;
      }
    case InstructionType::I:
      {
        input.iData.printI();
        break;
      }
    default:
      {
        input.jData.printJ();
        break;
      }
    }
}