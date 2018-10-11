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
#include <vector>
#include <array>

#define MAXLINE 80

// global variables
char line[MAXLINE];
unsigned int inst;
unsigned int word;
unsigned int gp;
int pc = 0;

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
    void addu(unsigned int rd_, unsigned int rs_, unsigned int rt_) { rd_ = rs_ + rt_; };
};

struct I_Format
{
    unsigned int opcode_;
    unsigned int rs_;
    unsigned int rt_;
    unsigned int immed_;
    void printI();
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

std::map<unsigned int, std::string> funct{
    {9, "addiu"},
    {33, "addu"},
    {4, "beq"},
    {2, "j"},
    {35, "lw"},
    {42, "slt"},
    {43, "sw"},
    {12, "syscall"}};

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

unsigned int regs[34];
std::vector<unsigned int> iList;
std::queue<Input> q;

int main()
{
    Input input; // Create input object


    while (fgets(line, MAXLINE, stdin))
    {
        if (sscanf(line, "%u%u", &gp, &word) == 2)
        {
            regs[28] = gp;

            std::cout << "regs:" << std::endl;

            for (int i = 0, j = 1; i < 34; ++i, ++j)
            {
                std::cout
                    << std::setw(8) << std::right << reg.find(i)->second 
                    << " =" 
                    << std::setw(6) << std::right << regs[i];
                if (j % 4 == 0)
                    std::cout << std::endl;
            }
        }
        else
        {
            sscanf(line, "%x", &inst);

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
                //std::cout << "sum: " << funct[input.rData.funct_](input.rData.rd_, input.rData.rs_, input.rData.rt_);
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

            std::string command;

            std::cout << std::setw(2) << std::right << pc << std::setw(2) << std::left << ":";

            if (input.type == InstructionType::R)
            {
                command = q.back().rData.printR();
                std::cout << command << std::endl;
            }
            else if (input.type == InstructionType::S)
            {
                q.back().sData.printS();
            }
            else if (input.type == InstructionType::I)
            {
                q.back().iData.printI();
            }
            else
            {
                q.back().jData.printJ();
            }
            ++pc;
        }
    }
    return 0;
}

void R_Format::printR()
{
    std::cout << std::setw(10) << std::left << funct[funct_] << reg[rd_] + "," + reg[rs_] + "," + reg[rt_] << std::endl;
}

void I_Format::printI() // has to accomodate multiple printing formats
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