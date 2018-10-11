
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
#include <fstream>

#define MAXLINE 80

// global variables

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
    void lw(unsigned int rt_, unsigned int immed_, std::vector<unsigned int> a);
    void sw(unsigned int rt_, unsigned int immed_, std::vector<unsigned int> a);
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

void printAll(Input, int counter);          // print report
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
    {33, "$hi"}};

std::map<unsigned int, std::string> funct{
    {9, "addiu"},
    {33, "addu"},
    {4, "beq"},
    {2, "j"},
    {35, "lw"},
    {42, "slt"},
    {43, "sw"},
    {12, "syscall"}};

unsigned int inst;
unsigned int word;
unsigned int gp;
int pc = 0;

std::queue<Input> q;
std::vector<unsigned int> regs(34, 0);
std::vector<unsigned int> data(32768);
std::vector<unsigned int> iList;
Input input; // Create input object
char line[MAXLINE];

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cerr << "Usage: " << argv[0] << " filename\n";
        exit(EXIT_FAILURE);
    }

    std::ifstream fin;
    fin.open(argv[1]);

    if (!fin.is_open())
    {
        std::cerr << "Unable to open file\n";
    }

    while (fin.getline(line, MAXLINE))
    {
        if (sscanf(line, "%u%u", &gp, &word) == 2)
        {
            regs[28] = gp;
            data.resize(word);
        }
        else // begin parse of o code
        {
            sscanf(line, "%x", &inst);
            iList.push_back(inst);
        }
    }

    // parse
    for (int pc = 0; pc < iList.size(); ++pc)
    {
        inst = iList[pc];

        if (inst >> 26 == 0)
        {
            if (inst == 0x00000000c)
            {
                input.type = InstructionType::S;
                input.sData = {inst >> 26};
                q.push(input);
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
                q.push(input);
            }
        }
        else if (inst >> 26 == 2 || inst >> 26 == 3)
        {
            input.type = InstructionType::J;
            input.jData =
                {
                    inst >> 26,
                    inst << 7 >> 7};
            q.push(input);
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
            q.push(input);
        } // end parse of o code

        // begin function calls
        switch (inst >> 26)
        {
        case 0:
        {
            if (input.rData.funct_ == 33)
                input.rData.add();
            else if (input.rData.funct_ == 42)
                input.rData.slt(input.rData.rd_, input.rData.rs_, input.rData.rt_);
            else
                break;
        }
        case 12:
        {
            input.sData.sCall(regs[2]); // hard coded to $v0
            break;
        }
        case 9:
        {
            input.iData.addiu();
            break;
        }
        case 2:
        {
        }
        default:
            break;
        }
    }

    std::queue<Input> q2(q);
    int i = 0;

    std::cout << "isnts:" << std::endl;
    while (i < q2.size())
    {
        printInstructions(q2.front(), i);
        q2.pop();
        ++i;
    }

    std::cout << "\n";

    int j = 0;

    while (!q.empty())
    {
        printAll(q.front(), j);
        q.pop();
        ++j;
    }

    return 0;
}

void System_Call::sCall(unsigned int v0)
{
    switch (v0)
    {
    case 10:
    {
        break;
    }
    case 5:
    {
        std::cin >> v0;
        break;
    }
    default:
    {
        std::cout << v0 << "\n";
        break;
    }
    }
}

void I_Format::lw(unsigned int rt_, unsigned int immed_, std::vector<unsigned int> a)
{
    regs[reg.find(rt_)->first] = a[immed_];
}

void I_Format::sw(unsigned int rt_, unsigned int immed_, std::vector<unsigned int> a)
{
    a[immed_] = regs[reg.find(rt_)->first];
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

void printAll(Input input, int counter)
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
    std::cout << std::endl
              << std::endl;

    for (int i = 0; i < data.size(); ++i)
    {
        printf("data memory:\n   data[%3d] =%6d\n", i, data[i]);
    }
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
