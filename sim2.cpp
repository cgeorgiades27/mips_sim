/******************************
 *
 *  Chris Georgiades
 *  CDA3101 - asg 3 (MIPS Simulator)
 *  sim.cpp
 *  compile: g++ -o sim.exe sim.cpp
 *
 ******************************/

#include <iostream>
#include <map>
#include <iomanip>
#include <fstream>
#include <vector>

#define MAXLINE 80

enum class InstructionType
{
  R, I, J, S
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

void printAll(Input, int counter, std::vector<unsigned int>); // print report
void printInstructions(Input, int counter);            // print instruction list

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

std::map<unsigned int, std::string> funct
{
  {9, "addiu"},
  {33, "addu"},
  {4, "beq"},
  {2, "j"},
  {35, "lw"},
  {42, "slt"},
  {43, "sw"},
  {12, "syscall"},
  {36, "and"},
  {5, "bne"},
  {26, "div"},
  {16, "mfhi"},
  {18, "mflo"},
  {24, "mult"},
  {37, "or"},
  {34, "subu"}
};

unsigned int inst;
unsigned int word;
unsigned int gp;

std::vector<unsigned int> data(32768);
std::vector<Input> q;
std::vector<unsigned int> regs(34, 0);
std::vector<unsigned int> iList;
Input input;
std::ofstream log;
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

  data.resize(word);

  for (int k = 0; k < data.size(); ++k)
         data[k] = iList[regs[28] + k];

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
          inst << 26 >> 26};
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
            inst << 26 >> 26};
        q.push_back(input);
      }
    }
    else if (inst >> 26 == 2 || inst >> 26 == 3)
    {
      input.type = InstructionType::J;
      input.jData =
        {
          inst >> 26,
          inst << 7 >> 7};
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
          inst << 16 >> 16};
      q.push_back(input);
    }
  } // end parse

  int i = 0;
  log.open("log.txt");

  if (log.is_open())
  {
    log << "isnts:" << std::endl;

    while (i < regs[28])
    {
      printInstructions(q[i], i);
      ++i;
    }

    log << "\n"
        << "data:\n";

    for (int k = 0; k < data.size(); ++k)
    { 
      log << std::setw(4) << std::right << i + k << ":" << std::setw(2) << data[k] << std::endl;
    }

    log << "\n";

    for (int pc = 0; pc < iList.size(); ++pc) // function call loop
    {

    Loop:
      inst = iList[pc];

      switch (inst >> 26) // begin function calls
      {
        case 0:
          {
            unsigned int funct = inst << 26 >> 26;

            if (funct == 33)
            {
              q[pc].rData.add();
            }
            else if (funct == 42)
            {
              q[pc].rData.slt(q[pc].rData.rd_, q[pc].rData.rs_, q[pc].rData.rt_);
            }
            else if (inst == 0xc) // syscalls
            {
              if (regs[2] == 10)
              {
                log
                  << "PC:" << std::setw(3) << std::right << pc << std::endl
                  << "inst: ";
						
                q[pc].sData.printS();
						
                log << "exiting simulator\n";
                exit(EXIT_SUCCESS);
              }
              else if (regs[2] == 5)
              {
                std::cin >> regs[2];
              }
              else
              {
                std::cout << regs[4] << "\n";
              }
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
            printAll(q[pc], pc, data);
            pc = q[pc].jData.addr_;
            goto Loop;
            break;
          }
        case 4:
          {
            if (regs[q[pc].iData.rt_] == regs[q[pc].iData.rs_])
            {
              printAll(q[pc], pc, data);
              pc += q[pc].iData.immed_;
              goto Loop;
            }
            break;
          }
        case 35:
          {
            regs[q[pc].iData.rt_] = data[q[pc].iData.immed_];
            break;
          }
        case 43:
          {
            data[q[pc].iData.immed_] = regs[q[pc].iData.rt_];
            break;
          }
        default:
          break;
      }
      printAll(q[pc], pc, data);
    }
  }
  else
  {
    std::cout << "failed to open file\n";
    exit(EXIT_FAILURE);
  }
  return 0;
} // THE END

void R_Format::slt(unsigned int rd_, unsigned int rs_, unsigned int rt_)
{
  if (regs[reg.find(rs_)->first] < regs[reg.find(rt_)->first])
    regs[reg.find(rd_)->first] = 1;
  else
    regs[reg.find(rd_)->first] = 0;
}

void R_Format::add()
{
  regs[rd_] = regs[rs_] + regs[rt_];
}

void I_Format::addiu()
{
  regs[reg.find(rt_)->first] = regs[reg.find(rs_)->first] + immed_;
}

void R_Format::printR()
{
  log << std::setw(8) << std::left << funct[funct_] << reg[rd_] + "," + reg[rs_] + "," + reg[rt_] << std::endl;
}

void I_Format::printI()
{
  switch (opcode_)
  {
    case 35:
    case 43:
      {
        log
          << std::setw(8)
          << std::left
          << funct[opcode_]
          << reg[rt_] << ","
          << immed_
          << "(" << reg[rs_] << ")" << std::endl;
        break;
      }
    case 4:
      {
        log
          << std::setw(8)
          << std::left
          << funct[opcode_]
          << reg[rs_] << ","
          << reg[rt_] << ","
          << immed_ << std::endl;
        break;
      }
    default:
      {
        log
          << std::setw(8)
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
  log << std::setw(8) << std::left << funct[opcode_] << addr_ << std::endl;
}

void System_Call::printS()
{
  log << "syscall" << std::endl;
}

void printAll(Input input, int counter, std::vector<unsigned int> arr)
{
  log
    << "PC:" << std::setw(3) << std::right << counter << std::endl
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

  log << std::endl
      << "regs:" << std::endl;

  for (int i = 0, j = 1; i < 34; ++i, ++j)
  {
    log
      << std::setw(8) << std::right << reg.find(i)->second
      << " ="
      << std::setw(6) << std::right << regs[i];
    if (j % 4 == 0)
      log << std::endl;
  }
  
  log << "\n\n"
      << "data memory:\n";

  for (int i = 0; i < arr.size(); ++i)
  {
    
    log
      << "   data[" << std::setw(3) << i << "] =" 
      << std::setw(6) << std::right << arr[i]
      << "  ";
  }

  log << "\n\n\n";
}

void printInstructions(Input input, int counter)
{
  log
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
