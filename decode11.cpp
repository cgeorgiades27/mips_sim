
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
    void lw(unsigned int rt_, unsigned int immed_, unsigned int *gp);
    void sw(unsigned int rt_, unsigned int immed_, unsigned int *gp);
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
void parse(Input, unsigned int instruction);

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

std::queue<Input> q;
unsigned int data[1];
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
            regs[28] = gp; // set the value of the gp
        }
        else // begin parse of object code
        {
            sscanf(line, "%x", &inst);
            iList.push_back(inst);
        }
    }
    ++counter1;

    // parse
    for (int counter2 = 0; counter2 < iList.size() - 1; ++counter2)
    {
        inst = iList[counter2];

        if (inst >> 26 == 0)
        {
            if (inst == 0xc)
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
        }
    } // end parse of object code

    std::queue<Input> q2(q); // copy the queue for contents are not lost
    int i = 0;

    // begin printing the instruction list
    std::cout << "isnts:" << std::endl;
    while (!q2.empty())
    {
        printInstructions(q2.front(), i);
        q2.pop();
        ++i;
    }

    std::cout
        << "\n";
    std::cout << "data:\n"
              << std::setw(4) << std::right << i << ":" << std::setw(2) << *data << "\n"
              << std::endl;

    for (int pc = 0; pc < iList.size() - 1; ++pc)
    {
        inst = iList[pc];

        switch (inst >> 26) // begin function calls
        {
        case 0: // R_Format and Syscalls
        {
            if (input.rData.funct_ == 33)
                input.rData.add();
            else if (input.rData.funct_ == 42)
                input.rData.slt(input.rData.rd_, input.rData.rs_, input.rData.rt_);
            else
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
            break;
        }
        case 4:
        {
            break;
        }
        case 35:
        {
        }
        default:
            break;
        }

        int j = 0;

        printAll(q.front(), j);
        q.pop();
        ++j;
    }
    return 0;
} // end of program

void parse(Input, unsigned int instruction)
{
    unsigned int inst = instruction;

    if (inst >> 26 == 0)
    {
        if (inst == 0xc)
        {
            input.type = InstructionType::S;
            input.sData = {inst >> 26};
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
        }
    }
    else if (inst >> 26 == 2 || inst >> 26 == 3)
    {
        input.type = InstructionType::J;
        input.jData =
            {
                inst >> 26,
                inst << 7 >> 7};
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
    }
}

void System_Call::sCall(unsigned int v0)
{
    switch (v0)
    {
    case 10: // exit
    {
        exit(EXIT_SUCCESS);
        break;
    }
    case 5: // read input
    {
        scanf("%u", &v0);
        break;
    }
    default: // print
    {
        std::cout << v0 << "\n";
        break;
    }
    }
}

void I_Format::lw(unsigned int rt_, unsigned int immed_, unsigned int *gp)
{
    regs[reg.find(rt_)->first] = *(gp + immed_);
}

void I_Format::sw(unsigned int rt_, unsigned int immed_, unsigned int *gp)
{
    *(gp + immed_) = regs[reg.find(rt_)->first];
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

    printf("\n");
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
